#pragma once
#include <vector>
#include "candle.h"


namespace sugar {																				
																								// adding more to namespace::sugar
																								
	class CandleSeries {																		// CandleSeries is the class that stores the data loaded from load_candles_csv
	public:																						// accessors
		CandleSeries() = default;																// set constructor to default 
		explicit CandleSeries(std::vector<Candle> rows) : rows_(std::move(rows)) {}				// set parameters for Pass By Value class object initialization (member initializer list)


		const std::vector<Candle>& rows() const { return rows_; }								// CandleSeries member function to return a read only const ref to data member 'rows_'
		std::size_t size() const { return rows_.size(); }										// CandleSeries member function to return a const size_t copy of vector::size()


																								// Convenience extractor for indicator inputs (close-only for now)
		std::vector<double> closes() const {													// returns a const vector of close values 
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
