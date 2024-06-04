#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "Textures.hpp"
#include <cstddef>
#include <string>

namespace game {

class Sprite : public sf::Sprite {
public:
  Sprite(const std::string &textureName, size_t layerIdx = 0);

  void setLayerIdx(size_t idx);

private:
  size_t layerIdx;
};

} // namespace game
