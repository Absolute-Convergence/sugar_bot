#pragma once
#include <memory>
#include "series.h"
#include "metrics.h"


namespace sugar {


	class IStrategy {													//
	public:																//
		virtual ~IStrategy() = default;									//
		virtual BacktestResult run(const CandleSeries& data) = 0;		// 
	};


	using StrategyPtr = std::shared_ptr<IStrategy>;						// 


} // namespace sugar