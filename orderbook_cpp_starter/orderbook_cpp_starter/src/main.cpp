#include "order_book.hpp"
#include <iomanip>
#include <iostream>

int main() {
    ob::OrderBook book;
    constexpr double tick_size = 0.01; // Demo instrument: one price tick = 0.01 USDT.

    book.load_snapshot(
        {{9000000, 1.20}, {8999500, 2.50}, {8999000, 0.80}, {8998500, 3.10}},
        {{9001000, 0.90}, {9001500, 1.80}, {9002000, 2.20}, {9002500, 1.10}}
    );

    auto print_book = [&]() {
        std::cout << "\nASKS (lowest first)\n";
        for (const auto& x : book.ask_depth(5))
            std::cout << std::fixed << std::setprecision(2) << x.price * tick_size
                      << " | " << std::setprecision(4) << x.quantity << '\n';
        std::cout << "-------------------\nBIDS (highest first)\n";
        for (const auto& x : book.bid_depth(5))
            std::cout << std::fixed << std::setprecision(2) << x.price * tick_size
                      << " | " << std::setprecision(4) << x.quantity << '\n';
        auto b = book.best_bid(); auto a = book.best_ask();
        auto m = book.midprice(); auto s = book.spread();
        if (b && a && m && s) {
            std::cout << "Best bid: " << *b * tick_size
                      << "\nBest ask: " << *a * tick_size
                      << "\nMidprice: " << *m * tick_size
                      << "\nSpread: " << *s * tick_size << '\n';
        }
        auto imbalance = book.depth_imbalance(3);
        if (imbalance) std::cout << "Top-3 depth imbalance: " << *imbalance << '\n';
    };

    std::cout << "INITIAL SNAPSHOT";
    print_book();
    std::cout << "\nAPPLYING DELTA: update 90000.00, delete 89995.00, insert 89992.50\n";
    book.update_bid(9000000, 3.00);
    book.update_bid(8999500, 0.00);
    book.update_bid(8999250, 2.00);
    print_book();
}
