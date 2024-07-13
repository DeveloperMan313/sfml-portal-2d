#pragma once

#include "SFML/Window/Event.hpp"
#include "Singleton.hpp"
#include <vector>

namespace game {

class UserInputIns {
  friend class Singleton<UserInputIns>;

public:
  UserInputIns();

  bool checkProcessEvent(const sf::Event &event);

private:
  std::vector<bool> keyStatus;

  bool changesKeyStatus(const sf::Event &event);

  void operator delete(void *ptr) noexcept;
};

using UserInput = Singleton<UserInputIns>;

} // namespace game
