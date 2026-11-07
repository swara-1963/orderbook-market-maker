# C++ Limit Order Book — Phase 1

A small Level 2 aggregated price-level order book in C++17.

## Included
- Snapshot loading
- Bid/ask insert, update and delete (zero quantity deletes)
- Best bid, best ask, midprice and spread
- Top-N depth and top-N depth imbalance
- Basic unit tests

## Price representation
Prices are integer ticks, not `double` map keys. In the demo one tick = 0.01 USDT:
90,000.00 USDT = 9,000,000 ticks. When connecting to an exchange, use the instrument's actual tick size. Quantities use `double` for this learning version; fixed-point quantities can be introduced later.

## Build (C++17 + CMake 3.16+)
```bash
cmake -S . -B build
cmake --build build
```

Run demo:
```bash
./build/orderbook_demo
```
On Windows, try `build\\Debug\\orderbook_demo.exe` or `build\\orderbook_demo.exe`, depending on the CMake generator.

Run tests:
```bash
ctest --test-dir build --output-on-failure
```

## Limitations
This is not yet connected to an exchange. It does not implement exchange sequence IDs, snapshot/delta synchronization, reconnect handling, exact queue position, or an execution simulator. It is Level 2 aggregated data, not Level 3 individual orders. It also does not yet validate crossed books after updates.

## Next phase
Add an exchange-independent feed interface and a Bybit adapter. Keep exchange-specific JSON parsing and update sequencing outside the `OrderBook` class.
