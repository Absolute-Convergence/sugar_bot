#pragma once
#include "indicator.h"


namespace sugar {


	class SMAIndicator final : public Indicator {										// SMAIndicator class, derived from 'Indicator' class, final inheritance
	public:																				// public API
		explicit SMAIndicator(std::size_t period) : n_(period) {}						// SMAIndicator class member function declaration and data member (n_) initialization, 
																							// explicit prevents implicit conversions [eg- only allow 'SMAIndicator s(5)'],
																							// disallow -> ex:SMAIndicator s = 5;
																						
		std::vector<double> compute(const CandleSeries& series) const override;			// derived compute function parameter as const override for derived class SMAInicatior, trailing const part of signature & must match base, 
																							// override = compiler checked override of base virtual, 
																							// trailing const part of signature & must match base 

		std::size_t period() const { return n_; }										// return const size_t SMAIndicator data member representing polling period


	private:																			
		std::size_t n_{};																// private data member of SMAIndicator class, configured via ctor
	};


} // namespace sugar