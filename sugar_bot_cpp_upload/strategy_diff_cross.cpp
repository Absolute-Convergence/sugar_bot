#include "strategy_diff_cross.h"
#include <algorithm>
#include <cmath>

namespace sugar {

    BacktestResult DiffCrossStrategy::run(const CandleSeries& data) {
        BacktestResult r{};
        if (data.size() == 0 || !a_ || !b_) return r;

        const auto av = a_->compute(data);
        const auto bv = b_->compute(data);
        const auto closes = data.closes();

        const std::size_t n = std::min({ av.size(), bv.size(), closes.size() });
        if (n == 0) return r;

        // Find first index where both indicators are usable (not NaN)
        auto is_nan = [](double x) { return std::isnan(x); };
        std::size_t i0 = 0;
        for (; i0 < n; ++i0) {
            if (!is_nan(av[i0]) && !is_nan(bv[i0])) break;
        }
        if (i0 >= n) return r;
        r.best_start_date = data[i0].date;

        bool long_on = false;
        double entry = 0.0;
        double equity = 0.0;
        double peak = 0.0;

        auto diff_at = [&](std::size_t i) { return av[i] - bv[i]; };

        for (std::size_t i = i0; i < n; ++i) {
            const double d = diff_at(i);

            // Enter long when diff >= +thresh
            if (!long_on && d >= thresh_) {
                long_on = true;
                entry = closes[i];
            }
            // Exit long (or flip to flat) when diff <= -thresh
            else if (long_on && d <= -thresh_) {
                const double trade_ret = (closes[i] / entry - 1.0) * 100.0;
                equity += trade_ret;
                ++r.trades;
                peak = std::max(peak, equity);
                r.max_drawdown = std::max(r.max_drawdown, peak - equity);
                long_on = false;
            }
        }

        // Close any open position at the last bar
        if (long_on) {
            const double trade_ret = (closes[n - 1] / entry - 1.0) * 100.0;
            equity += trade_ret;
            ++r.trades;
            peak = std::max(peak, equity);
            r.max_drawdown = std::max(r.max_drawdown, peak - equity);
        }

        r.pnl = equity;
        return r;
    }

} // namespace sugar
