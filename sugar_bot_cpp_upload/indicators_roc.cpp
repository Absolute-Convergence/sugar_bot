#include "indicators_roc.h"
#include <limits>
#include <cmath>


namespace sugar {

																									
	static inline double qnan() {																	// File-local helper; static = internal linkage
		return std::numeric_limits<double>::quiet_NaN();											// Uses quiet NaN sentinels to mark warm-up indices; returns an output aligned to input size, 
	}				
																										

	std::vector<double> roc_over_series(const std::vector<double>& v, std::size_t k) {				// Rate-of-change over k steps:
		std::vector<double> out(v.size(), qnan());													// count–value ctor: prefill with NaN (warm-up)
		if (k == 0 || v.size() <= k) return out;													// Guards: undefined lookback or no usable indices yet → return NaN-filled vector
		for (std::size_t i = k; i < v.size(); ++i) {												// Loop over closes vector v (not CandleSeries directly)
			const double prev = v[i - k];															// compare to value k steps back
			if (prev == 0.0) {																		// Division-by-zero guard at i-k → NaN (undefined return).
				out[i] = qnan();																	// undef
				continue;																			// cont
			}																						
			if (!std::isfinite(prev) || !std::isfinite(v[i])) {										// Guards: bad data 
				out[i]=qnan();																		// set out[i] to undef NaN 
				continue;																			// skip remainer of the loop, iterate i
			}
			out[i] = (v[i] / prev - 1.0) * 100.0;													// out[i] = ((v[i] / v[i - k]) - 1) * 100 for i >= k; NaN for i < k
		}
		return out;																					// return result vector 
	}


	std::vector<double> ROCIndicator::compute(const CandleSeries& series) const {					// Thin adapter: feed closes into the ROC kernel with this instance's k_
		return roc_over_series(series.closes(), k_);												// definition of the virtual override declared in the header.
	}


} // namespace sugar