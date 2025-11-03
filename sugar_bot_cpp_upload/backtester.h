#pragma once
#include "strategy.h"


namespace sugar {


	class Backtester {																//
	public:																			//
		BacktestResult run(const CandleSeries& data, IStrategy& strategy) {			//
			return strategy.run(data);												//
		}
	};


} // namespace sugar