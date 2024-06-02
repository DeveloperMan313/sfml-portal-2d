#pragma once

#include <functional>
#include <vector>

namespace game {

template <typename Tevent> class Emitter {
public:
  typedef std::function<void(const Tevent &event)> TsubscriberCallback;
  struct Tsubscriber {
    size_t ownerId;
    TsubscriberCallback callback;
  };

  void subscribe(size_t ownerId,
                 const TsubscriberCallback &subscriberCallback) {
    this->subscribers.push_back({ownerId, subscriberCallback});
  }

  void unsubscribeOwner(size_t ownerId) {
    std::erase_if(this->subscribers, [&ownerId](const Tsubscriber &sub) {
      return sub.ownerId == ownerId;
    });
  }

  void emit(const Tevent &event) const {
    for (const Tsubscriber &sub : this->subscribers) {
      sub.callback(event);
    }
  }

private:
  std::vector<Tsubscriber> subscribers;
};

} // namespace game
