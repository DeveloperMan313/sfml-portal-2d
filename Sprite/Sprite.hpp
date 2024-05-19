#pragma once

#include "../Textures/Textures.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include <string>

namespace game {

class Sprite : public sf::Sprite {
public:
  Sprite(const std::string &textureName, const Textures &textures, int layerInd = 0);

  void setLayerIndex(int& index);
  
private:
  int layerIndex;

};

} // namespace game
