#pragma once

#include <functional>
#include <vector>

namespace game {

template <typename Tevent> class Emitter {
public:
  typedef std::function<void(const Tevent &event)> Tsubscriber;

  void subscribe(const Tsubscriber &subscriber) {
    this->subscribers.push_back(subscriber);
  }

  void emit(const Tevent &event) const {
    for (const Tsubscriber &sub : this->subscribers) {
      sub(event);
    }
  }

private:
  std::vector<Tsubscriber> subscribers;
};

} // namespace game
