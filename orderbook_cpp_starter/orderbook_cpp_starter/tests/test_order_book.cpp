#include "order_book.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

int main() {
    ob::OrderBook book;
    assert(book.empty());
    assert(!book.best_bid());
    assert(!book.best_ask());
    assert(!book.midprice());

    book.load_snapshot({{10000, 2.0}, {9900, 3.0}, {9800, 1.5}},
                       {{10200, 1.5}, {10300, 2.5}, {10400, 1.0}});
    assert(book.best_bid().value() == 10000);
    assert(book.best_ask().value() == 10200);
    assert(std::abs(book.midprice().value() - 10100.0) < 1e-9);
    assert(book.spread().value() == 200);

    book.update_bid(10000, 4.0);              // Update existing level.
    assert(book.bid_depth(1).front().quantity == 4.0);
    book.update_bid(10100, 1.0);              // Insert a better bid.
    assert(book.best_bid().value() == 10100);
    book.update_bid(10100, 0.0);              // Delete it.
    assert(book.best_bid().value() == 10000);

    auto asks = book.ask_depth(2);
    assert(asks.size() == 2 && asks[0].price == 10200 && asks[1].price == 10300);
    const double expected = (4.0 - 1.5) / (4.0 + 1.5);
    assert(std::abs(book.depth_imbalance(1).value() - expected) < 1e-9);

    bool threw = false;
    try { book.update_ask(10500, -1.0); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Failed snapshot must not partially replace the existing book.
    threw = false;
    try { book.load_snapshot({{11000, 1.0}, {-1, 2.0}}, {{11200, 1.0}}); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw && book.best_bid().value() == 10000);

    std::cout << "All order book tests passed.\n";
}
