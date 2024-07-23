#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include <vector>

namespace game {

class Frame {
public:
  void add(const sf::Drawable *drawable);

  void render(sf::RenderTarget &target) const;

private:
  std::vector<const sf::Drawable *> drawables;
};

} // namespace game
