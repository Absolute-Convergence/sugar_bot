#include "strategy_roc_sma.h"
#include "indicators_sma.h"
#include "indicators_composite.h"
#include <algorithm>
#include <cmath>


namespace sugar {


	RocSmaCrossoverStrategy::RocSmaCrossoverStrategy(std::size_t sma_fast,
		std::size_t sma_slow,
		std::size_t roc_len,
		double thresh_percent)
		: sma_fast_(sma_fast), sma_slow_(sma_slow), roc_len_(roc_len), thresh_(thresh_percent) {
	}


	BacktestResult RocSmaCrossoverStrategy::run(const CandleSeries& data) {
		BacktestResult r{};
		if (data.size() == 0 || sma_fast_ == 0 || sma_slow_ == 0 || roc_len_ == 0) return r;


		auto f = std::make_shared<SMAIndicator>(sma_fast_);
		auto s = std::make_shared<SMAIndicator>(sma_slow_);


		ROCOfIndicator f_mom{ f, roc_len_ };
		ROCOfIndicator s_mom{ s, roc_len_ };


		const auto fv = f_mom.compute(data);
		const auto sv = s_mom.compute(data);
		const auto closes = data.closes();


		// find first usable index
		std::size_t i0 = 0; bool found = false;
		for (; i0 < fv.size(); ++i0) {
			if (!std::isnan(fv[i0]) && !std::isnan(sv[i0])) { found = true; break; }
		}
		if (!found) return r;
		r.best_start_date = data[i0].date;


		bool long_on = false; double entry = 0.0; double equity = 0.0; double peak = 0.0;


		for (std::size_t i = i0; i < closes.size(); ++i) {
			const double diff = fv[i] - sv[i];
			if (!long_on && diff >= thresh_) {
				long_on = true; entry = closes[i];
			}
			else if (long_on && diff <= -thresh_) {
				const double trade_ret = (closes[i] / entry - 1.0) * 100.0;
				equity += trade_ret; ++r.trades; peak = std::max(peak, equity);
				r.max_drawdown = std::max(r.max_drawdown, peak - equity);
				long_on = false;
			}
		}


		if (long_on) {
			const double trade_ret = (closes.back() / entry - 1.0) * 100.0;
			equity += trade_ret; ++r.trades;
			peak = std::max(peak, equity);
			r.max_drawdown = std::max(r.max_drawdown, peak - equity);
		}


		r.pnl = equity; return r;
	}


} // namespace sugar