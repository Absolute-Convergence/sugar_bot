#include <iostream>
#include <iomanip>
#include <vector>
#include <tuple>
#include <cmath>
#include <chrono>

#include "csv.h"
#include "utils.h"
#include "series.h"
#include "backtester.h"
#include "strategy_roc_sma.h"
#include "sweep.h"


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



int main(int argc, char** argv) {
	try {
		const std::string path = (argc >= 2) ? argv[1] : "C:/Dev/sugar_Bot/data/BTCUSD_420.csv";
		auto candles = sugar::load_candles_csv(path);
		sugar::CandleSeries series{ std::move(candles) };


		// Print tail to verify parse
		std::cout << std::fixed << std::setprecision(2);
		const auto& rows = series.rows();
		std::size_t count = std::min<std::size_t>(rows.size(), 20);
		for (std::size_t i = rows.size() - count; i < rows.size(); ++i) {
			const auto& c = rows[i]; char buf[9]; sugar::format_yyyymmdd(c.date, buf);
			std::cout << std::string_view(buf, 8)
				<< ", " << c.open
				<< ", " << c.high
				<< ", " << c.low
				<< ", " << c.close
				<< ", vol=" << c.volume << "\n";
		}
		std::cout << "Loaded " << rows.size() << " candle(s) from '" << path << "'\n";


		// Run OOP strategy via Backtester
		sugar::RocSmaCrossoverStrategy strat{/*fast=*/10, /*slow=*/30, /*roc=*/5, /*thresh=*/0.25 };
		sugar::Backtester bt;
		auto res = bt.run(strat, series);


		std::cout << "\nStrategy ROC(SMA) crossover results:\n";
		std::cout << " PnL: " << res.pnl << "%\n";
		std::cout << " Trades: " << res.trades << "\n";
		std::cout << " Max DD: " << res.max_drawdown << "%\n";
		if (res.best_start_date > 0) {
			char b[9]; sugar::format_yyyymmdd(res.best_start_date, b);
			std::cout << " Start usable date: " << std::string_view(b, 8) << "\n";
		}


		// Sweep example
		auto fasts = make_range(45, 55, 1);
		auto slows = make_range(55,65, 1);
		auto rocs = make_range(95, 105, 1);
		auto thresholds = make_drange(0.10, 0.20, 0.01);

		// naive count (upper bound)
		const std::size_t naive =
			fasts.size() * slows.size() * rocs.size() * thresholds.size();

		// effective fast/slow pairs where fast < slow
		std::size_t pairs = 0;
		for (auto s : slows) for (auto f : fasts) if (f < s) ++pairs;

		const std::size_t effective = pairs * rocs.size() * thresholds.size();
		std::cerr << "[Info] Grid size: naive=" << naive
			<< " combos, effective=" << effective << " combos\n";

		const std::size_t combos =
			fasts.size() * slows.size() * rocs.size() * thresholds.size();

		if (combos > 1000000) {
			std::cerr << "[Note] Large grid (" << combos
				<< " combos). Consider narrowing ranges or steps.\n"
				<< "Continue anyway? y/n \n";
				char a = 'n';
				std::cin >> a;
				if (a != 'y') {
					std::cerr << "Exiting program with no calculation.\n";
					return 0;
				};
		}

		auto t0 = std::chrono::high_resolution_clock::now();
		auto best = sugar::sweep_roc_sma(series, fasts, slows, rocs, thresholds);
		auto& [bf, bs, br, btval] = best.params;
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> dt = t1 - t0;
		std::cout << "\nSweep took " << dt.count() << "s across "
			<< effective << " combos.\n";		


		std::cout << "\nBest sweep:\n";
		std::cout << " fast=" << bf << " slow=" << bs
			<< " roc=" << br << " thresh=" << btval << "%\n";
		std::cout << " PnL: " << best.best.pnl << "%, Trades: " << best.best.trades
			<< ", Max DD: " << best.best.max_drawdown << "%\n";


		return 0;
	}
	catch (const std::exception& ex) {
		std::cerr << "Error: " << ex.what() << "\n"; return 1;
	}
}