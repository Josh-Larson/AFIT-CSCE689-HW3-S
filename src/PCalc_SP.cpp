#include <PCalc_SP.h>

PCalc_SP::PCalc_SP(unsigned int arraySize) : PCalc(arraySize) {

}

void PCalc_SP::markNonPrimes() {
	const auto maxSize = static_cast<long>(array_size());
	at(0) = false;
	at(1) = false;
	bool * primes = &at(0);
	for (long i = 2; ; i++) {
		if (primes[i]) {
			auto composite = i*i;
			if (composite < i || composite >= maxSize)
				break;
			while (composite < maxSize) {
				primes[composite] = false;
				composite += i;
			}
		}
	}
}
