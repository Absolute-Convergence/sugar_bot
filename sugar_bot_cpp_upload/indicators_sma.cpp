#include "indicators_sma.h"
#include <limits>
#include <numeric>


namespace sugar {


	static inline double qnan() {																	// 'static inline' qualifyer for "One Definition Rule" and to provide internal linkage without causing multiple definition errors. 
		return std::numeric_limits<double>::quiet_NaN();											// returns numeric_limits<T> member function quiet_NaN() for doubles
	}																								


	std::vector<double> SMAIndicator::compute(const CandleSeries& series) const {					// inherited function 'compute' in daughter class SMAIndicator, takes an immutable const ref CandleSeries parameter, returns vector of doubles 
		const auto v = series.closes();																// extracts vector of closing data into v
		std::vector<double> out(v.size(), qnan());													// vector count–value constructor: (make a vector of v.size() elements, each initialized to the value qnan())
		const std::size_t n = n_;																	// 
		if (n == 0 || v.size() < n) return out;														// exit early if zero or too small input


		double window_sum = std::accumulate(v.begin(), v.begin() + n, 0.0);							// sum the values in v from begining to n to start the polling period rolling sum
		out[n - 1] = window_sum / static_cast<double>(n);											// 
		for (std::size_t i = n; i < v.size(); ++i) {												// 
			window_sum += v[i] - v[i - n];															// 
			out[i] = window_sum / static_cast<double>(n);											// 
		}
		return out;																					// 
	}


} // namespace sugar