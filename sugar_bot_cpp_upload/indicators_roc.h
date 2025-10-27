#pragma once
#include "indicator.h"


namespace sugar {


																									// ROC on raw closes (k lookback)
	class ROCIndicator final : public Indicator {													// class declaration, using public Indicator API, final inheritance 
	public:
		explicit ROCIndicator(std::size_t k) : k_(k) {}												// explicit prohibits implicit class declaration, size_t parameter k initalized with private data member k_
		std::vector<double> compute(const CandleSeries& series) const override;						// const override of compute function from Indicator
		std::size_t lookback() const { return k_; }													// lookback() function declaration, returns immutable private data member k_
	private:
		std::size_t k_{};																			// size_t private data member k_
	};


																									// Utility: ROC over an arbitrary vector<double> (exposed for composites)
	std::vector<double> roc_over_series(const std::vector<double>& v, std::size_t k);				// function for polymorphic behavior so ROC can be applied to EMA or SMA indicators


} // namespace sugar