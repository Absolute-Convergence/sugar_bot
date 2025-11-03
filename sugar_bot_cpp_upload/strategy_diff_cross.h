#pragma once
#include <utility>
#include "strategy.h"
#include "indicator.h"

namespace sugar {

                                                                                        // Go long when (A - B) crosses up through +thresh.
                                                                                        // Exit (or flip) when it crosses down through -thresh.
                                                                                        // Units: whatever A and B output; choose thresh accordingly (e.g., % points).
    class DiffCrossStrategy final : public IStrategy {
    public:
        DiffCrossStrategy(IndicatorPtr a, IndicatorPtr b, double thresh_percent)
            : a_(std::move(a)), b_(std::move(b)), thresh_(thresh_percent) {
        }

        BacktestResult run(const CandleSeries& data) override;

    private:
        IndicatorPtr a_;
        IndicatorPtr b_;
        double thresh_{};
    };

} // namespace sugar
