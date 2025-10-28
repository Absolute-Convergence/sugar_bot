#pragma once
#include <cstddef>


namespace sugar {


	struct BacktestResult {
		double pnl{};											// cumulative % return across closed trades
		std::size_t trades{};									// round-trip trades
		double max_drawdown{};									// max peak-to-trough drawdown (%)
		int best_start_date{};									// first usable signal date (YYYYMMDD)
	};


} // namespace sugar