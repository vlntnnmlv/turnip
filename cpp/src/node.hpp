// Copyright 2025 Valentin Namleev

#pragma once

#include "anode.hpp"

namespace Turnip {
class Node : public ANode<Node> {
 public:
  Node(const std::string& _ID) : ANode(_ID) { }

  static std::shared_ptr<Node> Create(const std::string& _ID, const std::shared_ptr<Node>& _Parent = nullptr) {
        auto node = std::make_shared<Node>(_ID);
        node->SetParent(_Parent);
        return node;
    }

  ~Node() override = default;
 private:
  
};
}  // namespace Turnip
