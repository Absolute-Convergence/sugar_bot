﻿#include "csv.h"
#include "utils.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <string_view>
#include <cctype>

namespace sugar {

                                                                                                                // Minimal CSV splitter:
                                                                                                                // - Handles double quotes to allow commas inside quoted fields.
                                                                                                                // - Does not handle escaped quotes inside quoted fields
    std::vector<std::string> split_csv_line(std::string_view sv) {
        std::vector<std::string> out;                                                                           // vector of string objects to be returned later 
        std::string cur;                                                                                        // temp string to read and store characters into a string
        bool in_quotes = false;                                                                                 // bool condition 'in_quotes' for the loop
        for (char c : sv) {                                                                                     // iterate over every element in sv and store that element in char c                                                       
            if (c == '"') { in_quotes = !in_quotes; continue; }                                                 // read c, if sv has reached a '"' symbol, toggle in_quotes, skip the quote
            if (c == ',' && !in_quotes) {                                                                       // if a comma is reached, and not in_quotes
                    out.push_back(cur);                                                                         // store cur(string) in last element of out
                    cur.clear(); }                                                                              // clear the string cur
            else { cur.push_back(c); }                                                                          // add each element read into c, into the string cur 
        }
        out.push_back(cur);                                                                                     // store the final string (possibly empty) read from sv into cur through c into the final element of out
        return out;                                                                                             // return the vector of strings out
    }

                                                                                                                // Tiny heuristic: if the first column name contains "date", treat first row as header.
    static bool maybe_header(const std::vector<std::string>& cols) {                                            // accepts a vector of strings, and checks to see if it fits a header pattern
        if (cols.empty()) return true;                                                                          // first line check, if the columns vector is empty, skip it and exit the function
        std::string first = cols[0];                                                                            // string, initialized with the first string in function parameter
        std::transform(first.begin(), first.end(), first.begin(),                                               // STL transform function, iterates over 'first' from .begin to .end, destination start at .begin 
            [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });                              // lambda variable 'unsigned char ch', lambda function: return tolower(ch) for every char in string variable first
        return first.find("date") != std::string::npos || first.find("time") != std::string::npos;              // once every character in the header string is lowercase, search for 'date' or 'time' return a position/index to that location, or npos
    }

                                                                                                                // Read CSV file into a vector<Candle>.
                                                                                                                // Required columns by index: 0=Date, 1=Open, 2=High, 3=Low, 4=Close, 5=Volume (optional)
    std::vector<Candle> load_candles_csv(const std::string& path) {                                             // the actual candle loader function, accepts a compatable string object argument, passed by const reference
                                                                                                                // Two-step open avoids MSVC "vexing parse" and name-collision weirdness.
        std::ifstream ifs;                                                                                      // in file stream object, to be used later for reading the csv
        ifs.open(path, std::ios::in);                                                                           // opens file stream for reading path
        if (!ifs.is_open()) {                                                                                   // test with implicit verification, if fail, exit early with error code
            throw std::runtime_error("Failed to open CSV: " + path);                                            // exemption, the error code, and feedback on the invalid path, exit the program 
        }

        std::vector<Candle> rows;                                                                               // vector of candle objects called 'rows' (to be returned later)
        std::string line;                                                                                       // temp string called 'line'
        bool first_line = true;                                                                                 // bool variable to used with the 'maybe_header' function later

        while (std::getline(ifs, line)) {                                                                       // create a while loop dependant on the implicit pass/fail state of the getline() function, pass each line from ifs into line.
            if (line.empty()) continue;                                                                         // if line is empty, skip this loop and restart it
            if (!line.empty() && line.back() == '\r') line.pop_back();                                          // strip a trailing \r if present (CRLF files)

            auto cols = split_csv_line(line);                                                                   // a vector of strings, populated with each element of a line from the csv file
            if (first_line) {                                                                                   // test if it is the first line of the csv file
                first_line = false;                                                                             // set to false after first case of true
                if (maybe_header(cols)) continue;                                                               // test if first line is a header, if true, skip the rest of the loop
            }
            if (cols.size() < 5) continue;                                                                      // not enough fields → skip

            Candle c{};                                                                                         // candle object 
            c.date = parse_yyyymmdd(cols[0]);                                                                   // store date from string in cols[0] as an int, accaptable formats: 1) "YYYYMMDD" 2) "YYYY-MM-DD" 3) "YYYY/MM/DD" 4) "MM/DD/YYYY"
            if (c.date < 0) continue;                                                                           // skip row with date less than zero
            c.open = std::stod(cols[1]);                                                                        // store open
            c.high = std::stod(cols[2]);                                                                        // store high
            c.low = std::stod(cols[3]);                                                                         // store low
            c.close = std::stod(cols[4]);                                                                       // store close
            c.volume = (cols.size() >= 6) ? std::stod(cols[5]) : 0.0;                                           // if the column count is ≥ 6, parse cols[5], else set 0.0

            rows.push_back(c);                                                                                  // store each candle object in rows
        }
        return rows;                                                                                            // return vector of candles in rows
    }

} // namespace sugar
