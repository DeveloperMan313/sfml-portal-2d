#pragma once

#include "SFML/Graphics/Texture.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace game {

class Textures {
public:
  static Textures *textures;

  Textures();

  ~Textures();

  sf::Texture *getTexturePointer(const std::string &textureName) const;

private:
  static std::vector<std::string> textureNames;
  std::unordered_map<std::string, sf::Texture *> textureNamePointer;
};

} // namespace game
