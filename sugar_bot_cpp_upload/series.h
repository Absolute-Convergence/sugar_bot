#pragma once
#include <vector>
#include "candle.h"
#include <span>


namespace sugar {																				
																								// adding more to namespace::sugar
																								
	class CandleSeries {																		// stores the data loaded from load_candles_csv
	public:																						// accessors
		CandleSeries() = default;																// set constructor to default 
																								// set parameters for Pass By Value class object initialization (member initializer list) moves into 'rows'
		explicit CandleSeries(std::vector<Candle> rows) : rows_(std::move(rows)) {}				// rows is a temp std::vector<Candle> object for calculation
																								

		const std::vector<Candle>& rows() const { return rows_; }								// CandleSeries member function to return a read only const ref to data member 'rows_'
		std::size_t size() const { return rows_.size(); }										// CandleSeries member function to return a size_t copy of vector::rows_.size() by value, will not mutate (const)


																								// Convenience extractor for indicator inputs (close-only for now)
		std::vector<double> closes() const {													// returns a vector of close values by value.
			std::vector<double> out;															// temp vec to build and return by value
			out.reserve(rows_.size());															// set capacity
			for (const auto& c : rows_) out.push_back(c.close);									// loop to build the closing values
			return out;																			// return close values as vector
		}


		const Candle& operator[](std::size_t i) const { return rows_[i]; }						// overload the [] operator to return an indexed candle value.


	private:																					// encapsulation
		std::vector<Candle> rows_;																// CandleSeries (only) Data member
	};


} // namespace sugar
