#pragma once
#include <vector>
#include <memory>
#include "series.h"


namespace sugar {
																							 
																							
	class Indicator {																		 
	public:																					// public API domain for Indicator class
		virtual ~Indicator() = default;														// virtual destructor for proper clean up
																							// Compute an output vector aligned to the input series 
		virtual std::vector<double> compute(const CandleSeries& series) const = 0;			// virtual base with contract definition for compute function
	};


	using IndicatorPtr = std::shared_ptr<Indicator>;										// type alias for smart pointer operations


} // namespace sugar