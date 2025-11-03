#pragma once
#include <tuple>
#include <vector>
#include <limits>
#include <queue>
#include <iostream>
#include "metrics.h"
#include "strategy_roc_sma.h"
#include "backtester.h"
#include "swing_breakout_strategy.h"



namespace sugar {


	using RocSmaParams = std::tuple<std::size_t, std::size_t, std::size_t, double>;									// fast, slow, roc, thresh


	struct SweepResult {																							// 
		BacktestResult best;																						// 
		RocSmaParams params;																						// 
	};

	inline SweepResult sweep_roc_sma(const CandleSeries& data,														// 
		const std::vector<std::size_t>& fasts,																		// 
		const std::vector<std::size_t>& slows,																		// 
		const std::vector<std::size_t>& rocs,																		// 
		const std::vector<double>& threshes)																		// 
	{
		BacktestResult best{}; RocSmaParams bestp{ 0,0,0,0.0 };														// 
		double best_score = -std::numeric_limits<double>::infinity();												// 

		// ---- progress plumbing ----
		//const std::size_t total_naive = fasts.size() * slows.size() * rocs.size() * threshes.size();				// 
		std::size_t valid_pairs = 0;																				// 
		for (auto s : slows) for (auto f : fasts) if (f < s) ++valid_pairs;											// 
		const std::size_t total = valid_pairs * rocs.size() * threshes.size();										// 

		std::size_t count = 0;																						// 
		constexpr std::size_t progress_every = 100;																	// print every 100 combos 


		// best results storage
		struct Row { double score; BacktestResult r; RocSmaParams p; };												// 
		auto cmp = [](const Row& a, const Row& b) { return a.score > b.score; };									// min-heap
		std::priority_queue<Row, std::vector<Row>, decltype(cmp)> top(cmp);											// 
		constexpr std::size_t K = 5;
		// --------------------------

		size_t displayCounter = fasts.size();																		// simple counter, set to the size outer loop
		std::cerr << "Working now...\n";																			// feedback for user to confirm the program is running correctly

		for (auto f : fasts) {																						// 
			std::cerr << "Still working... current row: " << displayCounter << "\n";								// simple counter feedback to assure the user the program is running 
			--displayCounter;																						// count down
			for (auto s : slows) {																					// 
				if (f >= s) continue;																				// 
				for (auto rlen : rocs) {																			// 
					for (auto th : threshes) {																		// 
						RocSmaCrossoverStrategy strat{ f, s, rlen, th };											// 
						auto res = strat.run(data);																	// 
						double score = res.pnl - 0.25 * res.max_drawdown;											// 
						if (score > best_score) { best_score = score; best = res; bestp = { f, s, rlen, th };		// 
						top.push({ score, res, {f, s, rlen, th} });													// 
						if (top.size() > K) top.pop();																// 
						}
					}
				}
			}

		}

		// Display top results 
		std::vector<Row> topk;																						// 
		while (!top.empty()) { topk.push_back(top.top()); top.pop(); }												// 
		std::reverse(topk.begin(), topk.end());																		// best first

		std::cerr << "\nTop " << K << " combos:\n";																	// 
		for (auto& row : topk) {																					// 
			const auto& [f, s, rlen, th] = row.p;																	// 
			std::cerr << "  score=" << row.score																	// 
				<< " | fast=" << f << " slow=" << s																	// 
				<< " roc=" << rlen << " thresh=" << th																// 
				<< " | PnL=" << row.r.pnl																			// 
				<< "%, DD=" << row.r.max_drawdown																	// 
				<< "%, Trades=" << row.r.trades << "\n";															// 
		}

		// final flush if we didn't land exactly on a multiple
			if (count % progress_every != 0) {																		// 
				std::cerr << "[sweep] " << count << " / " << total << " combos (done)\n";							// 
			}

		return { best, bestp };																						// 
	}



	// ****** Breakout Sweep Plumbing ******

	/*

	// Inclusive integer range: [start, stop] step>0
	static std::vector<std::size_t> make_range(std::size_t start, std::size_t stop, std::size_t step) {
		std::vector<std::size_t> v;
		if (step == 0 || start > stop) return v;
		v.reserve((stop - start) / step + 1);
		for (std::size_t x = start; x <= stop; x += step) v.push_back(x);
		return v;
	}

	// Inclusive double range with tiny epsilon to avoid FP miss on the last point
	static std::vector<double> make_drange(double start, double stop, double step) {
		std::vector<double> v;
		if (step <= 0 || start > stop) return v;
		for (double x = start; x <= stop + 1e-12; x += step) v.push_back(x);
		return v;
	}


	struct SwingBreakoutSweepResult {
		BacktestResult result{};
		std::size_t left{};
		std::size_t right{};
		double pctGain{};
		double maxLoss{};
		double score{};
	};


	// **********************************


	// Breakout Strategy Sweep Logic

	inline SwingBreakoutSweepResult sweep_swing_breakout(const CandleSeries& candles) {
		SwingBreakoutSweepResult best{};

		// coarse ranges – tweak manually
		const std::vector<std::size_t> lefts = make_range(1,10, 1);
		const std::vector<std::size_t> rights = make_range(1, 10, 1);
		const std::vector<double> pctGains = make_drange(3.0, 5.0, 0.1);
		const std::vector<double> losses = make_drange(6.0, 8.0, 0.1);


		size_t displayCounter = lefts.size();																		// simple counter, set to the size outer loop
		std::cerr << "Working now...\n" << displayCounter <<'\n';

		for (auto L : lefts) {
			std::cerr << "Still working... current row: " << displayCounter << "\n";								// simple counter feedback to assure the user the program is running 
			--displayCounter;
			for (auto R : rights)
				for (auto gain : pctGains)
					for (auto loss : losses) {
						SwingBreakoutStrategy strat(L, R, true, 2,													// daysAbove10 set to 2, daysForGain set to 3
							gain, 3, loss);
						Backtester bt;
						auto r = bt.run(candles, strat);

						// simple scoring: reward profit, penalize drawdown
						double score = r.pnl - 0.25 * r.max_drawdown;
						if (score > best.score) {
							best = { r, L, R, gain, loss, score };
						}
					}
		}
		

		std::cout << "Best Swing Breakout combo:\n"
			<< "  leftBars=" << best.left
			<< ", rightBars=" << best.right
			<< ", gain%=" << best.pctGain
			<< ", loss%=" << best.maxLoss << "\n"
			<< "  PnL=" << best.result.pnl
			<< "  DD=" << best.result.max_drawdown
			<< "  Trades=" << best.result.trades
			<< "  Score=" << best.score << "\n\n";

		return best;
	}

	*/

} // namespace sugar