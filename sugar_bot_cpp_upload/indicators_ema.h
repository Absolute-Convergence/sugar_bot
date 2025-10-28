#pragma once
#include "indicator.h"


namespace sugar {																			// adding more to namespace sugar


	class EMAIndicator final : public Indicator {											// EMAIndicator class, derived from 'Indicator' class, final inheritance
	public:																					// public API
		explicit EMAIndicator(std::size_t period) : n_(period) {}							// explicit prevents implicit conversion (eg. only allow EMAIndicator e(5))
																								// initialize data member n_ with period

		std::vector<double> compute(const CandleSeries& series) const override;				// derived override of virtual function call compute 
																								// Note: const must match virtual declaration
		std::size_t period() const { return n_; }											// return immutable size_t copy
	private:
		std::size_t n_{};																	// private data member: unsigned int (n_)
	};


} // namespace sugar