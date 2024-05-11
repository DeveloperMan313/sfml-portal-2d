#pragma once

#include "../Sprite/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <vector>

namespace game {

class Graphics {
public:
  Graphics();

  ~Graphics();

  void render(const std::vector<Sprite*> &sprites);

  void changeDisplayResolution(std::vector<int> resolution);

private:
  sf::RenderWindow *window;
};

} // namespace game
