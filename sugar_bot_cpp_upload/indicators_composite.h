#pragma once
#include "indicator.h"


namespace sugar {


																						// Compose indicators: apply ROC to the output of another Indicator
	class ROCOfIndicator final : public Indicator {										// 
	public:																				// 
		ROCOfIndicator(IndicatorPtr base, std::size_t k)								// uses base class indicator sharedptr data member alias, and window
			: base_(std::move(base)), k_(k) {											// move sharedptr base to private data member base_
		}


		std::vector<double> compute(const CandleSeries& series) const override;			// derived override declaration for compute function


	private:																			
		IndicatorPtr base_;																// smart pointer data member
		std::size_t k_{};																// window
	};


} // namespace sugar