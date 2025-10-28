#include "indicators_composite.h"
#include "indicators_roc.h"


namespace sugar {


	std::vector<double> ROCOfIndicator::compute(const CandleSeries& series) const {				// derived compute function definition
																								 
																								
		const auto base_vals = base_->compute(series);											// compute base indicator first, but we need its raw vector -> fabricate a temp series of closes, 
																									// Trick: build a synthetic CandleSeries with close = base_value to reuse roc_over_series

		return roc_over_series(base_vals, k_);													// dates/prices not needed here; reuse roc_over_series directly
	}


} // namespace sugar