#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <vector>

namespace ob {
using Price = std::int64_t;       // Integer ticks, not floating-point prices.
using Quantity = double;
struct Level { Price price; Quantity quantity; };

class OrderBook {
public:
    using BidMap = std::map<Price, Quantity, std::greater<Price>>;
    using AskMap = std::map<Price, Quantity, std::less<Price>>;

    void load_snapshot(const std::vector<Level>& bids, const std::vector<Level>& asks);
    void update_bid(Price price, Quantity quantity); // zero quantity deletes level
    void update_ask(Price price, Quantity quantity);

    std::optional<Price> best_bid() const;
    std::optional<Price> best_ask() const;
    std::optional<double> midprice() const;
    std::optional<Price> spread() const;
    std::vector<Level> bid_depth(std::size_t levels) const;
    std::vector<Level> ask_depth(std::size_t levels) const;
    std::optional<double> depth_imbalance(std::size_t levels) const;
    std::size_t bid_level_count() const noexcept;
    std::size_t ask_level_count() const noexcept;
    bool empty() const noexcept;

private:
    BidMap bids_;
    AskMap asks_;
    static void validate_level(Price price, Quantity quantity);
    static void apply_update(BidMap& side, Price price, Quantity quantity);
    static void apply_update(AskMap& side, Price price, Quantity quantity);
};
} // namespace ob
