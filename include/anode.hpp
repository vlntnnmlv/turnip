// Copyright 2025 Valentin Namleev

#pragma once

#include <queue>
#include <string>
#include <vector>

namespace turnip {
template <typename T> class ANode : public std::enable_shared_from_this<T> {
public:
  explicit ANode(const std::string &_ID) : m_ID(_ID), m_UID(m_UIDCount++) {}
  virtual ~ANode() = default;

  const std::string &ID() const { return m_ID; }
  const std::size_t UID() const { return m_UID; }
  const std::vector<std::shared_ptr<T>> &Children() const { return m_Children; }
  std::shared_ptr<T> Parent() const { return m_Parent.lock(); }

  void SetParent(std::shared_ptr<T> _Parent) {
    m_Parent = _Parent;

    if (auto p = Parent()) {
      p->LinkChild(this->shared_from_this());
    }
  }

  void LinkChild(std::shared_ptr<T> _Child) {
    m_Children.emplace_back(std::move(_Child));
  }

  int Order() const {
    std::shared_ptr<T> parent = m_Parent.lock();
    if (!parent)
      return 0;

    const auto &siblings = parent->m_Children;

    auto it = std::find_if(
        siblings.begin(), siblings.end(),
        [this](const std::shared_ptr<T> &node) { return node.get() == this; });

    return (it != siblings.end())
               ? static_cast<int>(std::distance(siblings.begin(), it))
               : 0;
  }

  void RemoveImmediate() {
    m_Children.clear();
    if (auto p = m_Parent.lock()) {
      auto &siblings = p->m_Children;
      siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
                                    [this](const std::shared_ptr<T> &_Node) {
                                      return _Node.get() == this;
                                    }),
                     siblings.end());
    }

    m_Parent.reset();
  }

  void Traverse(const std::function<void(std::shared_ptr<T>)> &_Action) {
    if (!_Action)
      return;

    std::queue<std::shared_ptr<T>> q;
    q.push(std::static_pointer_cast<T>(this->shared_from_this()));

    while (!q.empty()) {
      std::shared_ptr<T> current = q.front();
      q.pop();

      _Action(current);

      for (const std::shared_ptr<T> &child : current->m_Children)
        q.push(child);
    }
  }

private:
  const std::string m_ID;
  const std::size_t m_UID;
  static std::size_t m_UIDCount;
  std::vector<std::shared_ptr<T>> m_Children;
  std::weak_ptr<T> m_Parent;
};

template <typename T> std::size_t ANode<T>::m_UIDCount = 0;

} // namespace turnip
