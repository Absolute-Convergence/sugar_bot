#pragma once
#include "indicator.h"


namespace sugar {


	class EMAIndicator final : public Indicator {
	public:
		explicit EMAIndicator(std::size_t period) : n_(period) {}
		std::vector<double> compute(const CandleSeries& series) const override;
		std::size_t period() const { return n_; }
	private:
		std::size_t n_{};
	};


} // namespace sugar