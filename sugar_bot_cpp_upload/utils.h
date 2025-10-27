#pragma once
#include <string_view>
#include <cstdint>

namespace sugar {

    // Parse "YYYYMMDD" or "YYYY-MM-DD" or ISO-8601 "YYYY-MM-DDTHH:MM[:SS][Z|±HH:MM]"
    // Returns YYYYMMDD as int, or -1 on failure.
    int parse_yyyymmdd(std::string_view s);

    // Format YYYYMMDD -> "YYYYMMDD" (8 chars).
    // Caller owns the fixed 9-byte buffer (8 chars; no null required for printing).
    void format_yyyymmdd(int yyyymmdd, char out[9]);

    // --- NEW (optional for intraday) ------------------------------------------
    // Parse ISO-8601 datetime strings like:
    //   2024-10-09T01:00:00-06:00
    //   2025-09-12T17:30:00-06:00
    //   2025-10-03T11:09:15-06:00
    // Seconds are optional; timezone can be 'Z' or ±HH:MM. Offset minutes negative
    // for west of UTC (e.g., -06:00 -> -360).
    //
    // On success, returns true and fills:
    //   yyyymmdd            -> date as int YYYYMMDD
    //   seconds_since_mid   -> seconds since local *clock time* in the string (00:00:00 = 0)
    //   tz_offset_minutes   -> minutes offset from UTC (e.g., -360 for -06:00). If no TZ
    //                           present, set to 0.
    // On failure, returns false and leaves outputs unspecified.
    bool parse_iso8601_datetime(std::string_view s,
        int& yyyymmdd,
        int& seconds_since_mid,
        int& tz_offset_minutes);

} // namespace sugar
