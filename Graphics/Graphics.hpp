#pragma once

#include "../Sprite/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <vector>

namespace game {

class Graphics {
public:
  Graphics();

  ~Graphics();

  std::vector<Sprite *> sprites;

  void render();

private:
  sf::RenderWindow *window;
};

} // namespace game
