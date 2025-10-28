#pragma once
#include "strategy.h"
#include "indicator.h"


namespace sugar {


	class RocSmaCrossoverStrategy final : public IStrategy {					//
	public:																		//
		RocSmaCrossoverStrategy(std::size_t sma_fast,							//
			std::size_t sma_slow,												//
			std::size_t roc_len,												//
			double thresh_percent);												//


		BacktestResult run(const CandleSeries& data) override;					//


	private:																	//
		std::size_t sma_fast_{};												//
		std::size_t sma_slow_{};												//
		std::size_t roc_len_{};													//
		double thresh_{};														//
	};


} // namespace sugar