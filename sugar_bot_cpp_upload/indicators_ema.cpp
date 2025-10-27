#include "indicators_ema.h"
#include <limits>
#include <cmath>


namespace sugar {


	static inline double qnan() { return std::numeric_limits<double>::quiet_NaN(); }							// <limits>::quiet_NaN() small helper, wrapper function 


	std::vector<double> EMAIndicator::compute(const CandleSeries& series) const {								// Override of Indicator::compute; const must match header declaration,
																													// returns vector of doubles, accepts CandleSeries by const ref,
																													// Note: override keyword appears only in the class (header), not here

		const auto v = series.closes();																			// extract closing data from series, store in v
		std::vector<double> out(v.size(), qnan());																// call vector constructor, reserve v.size() elements, fill with quiet_NaN
		const std::size_t n = n_;																				// local copy of private data member n_
		if (n == 0 || v.empty()) return out;																	// guard: empty series param exit early 


		const double alpha = 2.0 / (static_cast<double>(n) + 1.0);												// math for EMA setup, alpha -> α = 2/(n+1)​
																														// EMA(t)​ = α * [(j=0)∑(k)] { ​[(1−α)j * x(t−j)] ​+ [(1−α)(k+1) * EMA(t−k−1)] }, 
																														// xt is the close price at index t
																														// [(j=0)∑(k)] is the sum from j=0 to k

		if (v.size() >= n) {																					// heuristic: test if k is smaller than or equal to n for xn (it usually is) 
			double seed = 0.0;																					// seed for rolling sum (starts at zero)
			for (std::size_t i = 0; i < n; ++i) seed += v[i];													// sum through n - 1 terms in the Candle Series (v)
			seed /= static_cast<double>(n);																		// generate the first valid value for the EMA, store in seed 
			out[n - 1] = seed;																					// initialize the first valid EMA value in out[n - 1]
			for (std::size_t i = n; i < v.size(); ++i)															// loop through the rest of v
				out[i] = alpha * v[i] + (1.0 - alpha) * out[i - 1];												// set each value of out[i] according to the formula for an EMA series
		}
		else {																									// heuristic: else if k is greater than n, (usually user is stupid, but this can still return something useful 
			out[0] = v[0];																						// set the first element of out[0] to v[0]. because there's not enough data for an SMA seed  
			for (std::size_t i = 1; i < v.size(); ++i)															// iterate for all the data given
				out[i] = alpha * v[i] + (1.0 - alpha) * out[i - 1];												// generate an EMA without an SMA seed, instead using the first element in the candle series,
																													// Note: this is an edge case usually only done in broad parameter sweeps. 
		}
		return out;																								// return the generated EMA vector
	}


} // namespace sugar