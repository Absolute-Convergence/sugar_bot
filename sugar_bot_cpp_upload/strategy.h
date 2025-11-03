#pragma once
#include <memory>
#include "series.h"
#include "metrics.h"


namespace sugar {


	class IStrategy {													// abstract class, can not be referenced directly
	public:																
		virtual ~IStrategy() = default;									// call destructor in derived classes through pointer operations
		virtual BacktestResult run(const CandleSeries& data) = 0;		// virtual base function "run" with contract definition 
	};


	using StrategyPtr = std::shared_ptr<IStrategy>;						// type alias for IStrategy smart pointer operations  


} // namespace sugar