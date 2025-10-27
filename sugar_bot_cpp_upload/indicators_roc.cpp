#include "indicators_roc.h"
#include <limits>


namespace sugar {


	static inline double qnan() { return std::numeric_limits<double>::quiet_NaN(); }				// Uses quiet NaN sentinels to mark warm-up indices; returns an output aligned to input size, 
																										// File-local helper; static = internal linkage

	std::vector<double> roc_over_series(const std::vector<double>& v, std::size_t k) {				// vector of doubles for closing values, and backtesting window
		std::vector<double> out(v.size(), qnan());													// call vector constructor
		if (k == 0 || v.size() <= k) return out;													// zero guard
		for (std::size_t i = k; i < v.size(); ++i) {												// step through the CandleSeries object
			const double prev = v[i - k];															// set prev to the previous candle close value
			if (prev == 0.0) { out[i] = qnan(); continue; }											// div by zero guard
			out[i] = (v[i] / prev - 1.0) * 100.0;													// percent
		}
		return out;
	}


	std::vector<double> ROCIndicator::compute(const CandleSeries& series) const {					// polymorphic compute function declaration
		return roc_over_series(series.closes(), k_);												// call roc_over_series(v, k_) function with CandleSeries.closes function
	}


} // namespace sugar