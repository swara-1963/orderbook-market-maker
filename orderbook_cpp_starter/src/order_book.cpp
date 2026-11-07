#include "order_book.hpp"
#include <cmath>
#include <stdexcept>

namespace ob {
void OrderBook::validate_level(Price price, Quantity quantity) {
    if (price <= 0) throw std::invalid_argument("Price must be positive integer ticks.");
    if (!std::isfinite(quantity) || quantity < 0.0)
        throw std::invalid_argument("Quantity must be finite and non-negative.");
}
void OrderBook::apply_update(BidMap& side, Price price, Quantity quantity) {
    validate_level(price, quantity);
    if (quantity == 0.0) side.erase(price); else side[price] = quantity;
}
void OrderBook::apply_update(AskMap& side, Price price, Quantity quantity) {
    validate_level(price, quantity);
    if (quantity == 0.0) side.erase(price); else side[price] = quantity;
}
void OrderBook::load_snapshot(const std::vector<Level>& bids, const std::vector<Level>& asks) {
    // Construct temporary maps so invalid snapshots do not partially overwrite the live book.
    BidMap new_bids;
    AskMap new_asks;
    for (const auto& x : bids) {
        validate_level(x.price, x.quantity);
        if (x.quantity > 0.0) new_bids[x.price] = x.quantity;
    }
    for (const auto& x : asks) {
        validate_level(x.price, x.quantity);
        if (x.quantity > 0.0) new_asks[x.price] = x.quantity;
    }
    bids_.swap(new_bids);
    asks_.swap(new_asks);
}
void OrderBook::update_bid(Price p, Quantity q) { apply_update(bids_, p, q); }
void OrderBook::update_ask(Price p, Quantity q) { apply_update(asks_, p, q); }
std::optional<Price> OrderBook::best_bid() const {
    if (bids_.empty()) return std::nullopt;
    return bids_.begin()->first;
}
std::optional<Price> OrderBook::best_ask() const {
    if (asks_.empty()) return std::nullopt;
    return asks_.begin()->first;
}
std::optional<double> OrderBook::midprice() const {
    auto b = best_bid(); auto a = best_ask();
    if (!b || !a) return std::nullopt;
    return (static_cast<double>(*b) + static_cast<double>(*a)) / 2.0;
}
std::optional<Price> OrderBook::spread() const {
    auto b = best_bid(); auto a = best_ask();
    if (!b || !a) return std::nullopt;
    return *a - *b;
}
std::vector<Level> OrderBook::bid_depth(std::size_t n) const {
    std::vector<Level> out; out.reserve(n);
    for (auto it = bids_.begin(); it != bids_.end() && out.size() < n; ++it)
        out.push_back({it->first, it->second});
    return out;
}
std::vector<Level> OrderBook::ask_depth(std::size_t n) const {
    std::vector<Level> out; out.reserve(n);
    for (auto it = asks_.begin(); it != asks_.end() && out.size() < n; ++it)
        out.push_back({it->first, it->second});
    return out;
}
std::optional<double> OrderBook::depth_imbalance(std::size_t n) const {
    if (n == 0) return std::nullopt;
    double bq = 0.0, aq = 0.0;
    for (const auto& x : bid_depth(n)) bq += x.quantity;
    for (const auto& x : ask_depth(n)) aq += x.quantity;
    const double total = bq + aq;
    if (total == 0.0) return std::nullopt;
    return (bq - aq) / total;
}
std::size_t OrderBook::bid_level_count() const noexcept { return bids_.size(); }
std::size_t OrderBook::ask_level_count() const noexcept { return asks_.size(); }
bool OrderBook::empty() const noexcept { return bids_.empty() && asks_.empty(); }
} // namespace ob
