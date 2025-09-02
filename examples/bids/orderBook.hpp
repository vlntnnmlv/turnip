#pragma once

#include <unordered_map>
#include <vector>

enum class OrderType { BUY = 0, SELL = 1 };

struct Order {
    size_t id = 0;
    float amount = 0.0f;
};

class OrderBook {
public:
    OrderBook() {
        m_Orders[OrderType::BUY] = std::vector<Order>();
        m_Orders[OrderType::SELL] = std::vector<Order>();
    };
    ~OrderBook() = default;

    void AddOrder(OrderType _OrderType, float _Amount) {
        m_Orders.at(_OrderType).push_back(Order{.amount = _Amount, .id = m_NextOrderID++});
    }

private:
    std::unordered_map<OrderType, std::vector<Order>> m_Orders{};
    size_t m_NextOrderID = 1;
};
