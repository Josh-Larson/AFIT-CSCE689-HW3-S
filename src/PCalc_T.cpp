#include <PCalc_T.h>

#include <thread>
#include <cmath>
#include <atomic>
#include <zconf.h>

PCalc_T::PCalc_T(unsigned int arraySize, unsigned int threadCount) : PCalc(arraySize),
		threadCount(threadCount),
		threads(),
		work(threadCount) {
	threads.reserve(threadCount);
	bool * primes = &at(0);
	
	for (unsigned int i = 0; i < threadCount; i++) {
		work[i] = 0;
		threads.emplace_back(std::make_unique<std::thread>([&, arraySize, primes, i]() {
			auto maxSize = static_cast<long>(arraySize);
			while (true) {
				long prime = 0;
				while (prime == 0 && work[i].compare_exchange_weak(prime, 0))
					;
				if (prime <= 0)
					break;
				if (!primes[prime])
					continue; // Race condition - we may as well try to catch it
				
				auto composite = prime*prime;
				while (composite < maxSize) {
					primes[composite] = false;
					composite += prime;
				}
				work[i].compare_exchange_strong(prime, 0);
			}
		}));
	}
}

void PCalc_T::markNonPrimes() {
	constexpr auto firstPrimes = std::array<long, 65> {
		2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
		43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
		101, 103, 107, 109, 113, 127, 131, 137, 139, 149,
		151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
		199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
		263, 269, 271, 277, 281, 283, 293, 307, 311, 313
	};
	const auto maxSize = static_cast<long>(array_size());
	const auto stopPrime = static_cast<long>(sqrt(maxSize));
	bool * primes = &at(0);
	
	for (unsigned int i = 0; i < threadCount; i++) {
		work[i] = i < 64 ? firstPrimes[i] : 0;
	}
	
	primes[0] = false;
	primes[1] = false;
	long currentPrime = firstPrimes[threadCount < 64 ? threadCount : 64];
	while (currentPrime <= stopPrime) {
		for (int currentWorkerThread = 0; currentWorkerThread < threadCount; currentWorkerThread++) {
			long expected = 0;
			if (work[currentWorkerThread].compare_exchange_weak(expected, currentPrime)) {
				// Scan for next prime
				do {
					currentPrime++;
				} while (!primes[currentPrime]);
			}
		}
	}
	for (size_t i = 0; i < threadCount; i++) {
		long expected = 0;
		while (!work[i].compare_exchange_weak(expected, -1))
			expected = 0;
	}
	for (unsigned int i = 0; i < threadCount; i++)
		threads[i]->join();
}
