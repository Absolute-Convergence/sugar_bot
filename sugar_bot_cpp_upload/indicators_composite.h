#pragma once
#include "indicator.h"


namespace sugar {


	// Compose indicators: apply ROC to the output of another Indicator
	class ROCOfIndicator final : public Indicator {
	public:
		ROCOfIndicator(IndicatorPtr base, std::size_t k)
			: base_(std::move(base)), k_(k) {
		}


		std::vector<double> compute(const CandleSeries& series) const override;


	private:
		IndicatorPtr base_;
		std::size_t k_{};
	};


} // namespace sugar