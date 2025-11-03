#include "indicators_composite.h"

namespace sugar {

    std::vector<double> MapIndicator::compute(const CandleSeries& series) const {
        const auto base_vals = base_->compute(series);
        return fn_(base_vals);
    }

} // namespace sugar
