#ifndef PCALC_T_H
#define PCALC_T_H

#include <PCalc.h>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>

class PCalc_T : public PCalc {
	std::vector<std::unique_ptr<std::thread>> threads;
	std::vector<std::atomic<long>>            work;
	
	public:
	PCalc_T(unsigned int arraySize, unsigned int threadCount);
	~PCalc_T() override = default;
	
	void markNonPrimes() override;
	
	private:
	unsigned int threadCount;
	
};

#endif
