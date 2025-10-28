#pragma once
#include <cstdint>

namespace sugar {

    // Candle is the atomic data row your indicators/strategies consume.
    // Keeping date as an int (YYYYMMDD) avoids time libs during Phase 2.
    struct Candle {
        std::int32_t date;   // e.g. 20250101
        double open{};
        double high{};
        double low{};
        double close{};
        double volume{};
    };

} // namespace sugar
