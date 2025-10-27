#pragma once
#include <vector>
#include <memory>
#include "series.h"


namespace sugar {


	class Indicator {
	public:
		virtual ~Indicator() = default;
		// Compute an output vector aligned to the input series
		virtual std::vector<double> compute(const CandleSeries& series) const = 0;
	};


	using IndicatorPtr = std::shared_ptr<Indicator>;


} // namespace sugar