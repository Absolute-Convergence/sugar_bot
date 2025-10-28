#pragma once
#include "indicator.h"


namespace sugar {


																									// ROC over k steps on raw closes
	class ROCIndicator final : public Indicator {													// class declaration, using public Indicator API, final inheritance 
	public:
		explicit ROCIndicator(std::size_t k) : k_(k) {}												// explicit prohibits implicit class declaration, size_t parameter k initalized with private data member k_
		std::vector<double> compute(const CandleSeries& series) const override;						// Contract: returns a vector aligned to input length; indices < k are NaN (warm-up)
		std::size_t lookback() const { return k_; }													// Lookback accessor; note: k_ is private and set in ctor (no setter => effectively immutable)
	private:
		std::size_t k_{};																			// size_t private data member k_
	};


																									// Utility: ROC over an arbitrary vector<double> (exposed for composites)
	std::vector<double> roc_over_series(const std::vector<double>& v, std::size_t k);				// function declaration for polymorphic behavior so ROC can be applied to EMA or SMA indicators


} // namespace sugar