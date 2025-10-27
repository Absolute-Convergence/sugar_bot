# Sugar_Bot (C++)

**An object-oriented C++ framework for quantitative analysis and technical trading.**
Reads financial `.csv` candle data, computes indicator signals, backtests strategies, and performs multi-parameter (M×N) sweeps.

## Quick Start
1. Build with your preferred C++17+ compiler.
2. Run:
   ```bash
   ./sugar_bot_cpp path/to/BTCUSD_420.csv
   ```

## Layout (flat for easy upload)
This repo keeps sources/headers in the root to avoid include-path issues. You can refactor into `src/` and `include/` later.

## Overview:
| Layer              | Role                                                   | Example                                                          |
| :----------------- | :----------------------------------------------------- | :--------------------------------------------------------------- |
| **Data Layer**     | Loads and stores market candles.                       | `CandleSeries`, `load_candles_csv()`                             |
| **Indicators**     | Compute transforms like SMA, EMA, and ROC.             | `SMAIndicator`, `EMAIndicator`, `ROCIndicator`, `ROCOfIndicator` |
| **Strategy Layer** | Defines entry/exit logic from indicator signals.       | `RocSmaCrossoverStrategy`                                        |
| **Backtester**     | Executes a strategy over historical data.              | `Backtester::run()`                                              |
| **Sweeper**        | Performs parameter sweeps to find best configurations. | `sweep_roc_sma()`                                                |


## Repo Structure:
src/
 ├─ indicators/
 │   ├─ indicators_sma.cpp/.h
 │   ├─ indicators_ema.cpp/.h
 │   ├─ indicators_roc.cpp/.h
 │   └─ indicators_composite.cpp/.h
 ├─ strategy/
 │   ├─ strategy_roc_sma.cpp/.h
 │   └─ strategy.h
 ├─ core/
 │   ├─ series.h
 │   ├─ candle.h
 │   ├─ utils.cpp/.h
 │   ├─ csv.cpp/.h
 │   ├─ backtester.h
 │   ├─ metrics.h
 │   └─ sweep.h
 └─ main.cpp


## Architecture:
main.cpp
 ├─ Backtester
 │   └─ Strategy (e.g. RocSmaCrossoverStrategy)
 │        ├─ Indicators (SMA, EMA, ROC, ROCOfIndicator)
 │        └─ CandleSeries (market data)
 └─ Sweep (multi-parameter optimization)


## Implemented Indicators: 
| Indicator        | Description                                                  |
| :--------------- | :----------------------------------------------------------- |
| `SMAIndicator`   | Simple Moving Average                                        |
| `EMAIndicator`   | Exponential Moving Average                                   |
| `ROCIndicator`   | Rate of Change over closes                                   |
| `ROCOfIndicator` | Composes ROC of any base indicator (e.g. ROC(SMA), ROC(EMA)) |


## License
MIT
