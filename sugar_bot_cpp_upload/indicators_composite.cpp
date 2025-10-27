#include "indicators_composite.h"
#include "indicators_roc.h"


namespace sugar {


	std::vector<double> ROCOfIndicator::compute(const CandleSeries& series) const {
		// compute base indicator first, but we need its raw vector -> fabricate a temp series of closes
		// Trick: build a synthetic CandleSeries with close = base_value to reuse roc_over_series
		const auto base_vals = base_->compute(series);


		// We don’t need dates/prices here; reuse roc_over_series directly
		return roc_over_series(base_vals, k_);
	}


} // namespace sugar