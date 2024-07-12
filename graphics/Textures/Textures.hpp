#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "Singleton.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace game {

class TexturesIns {
  friend class Singleton<TexturesIns>;

public:
  TexturesIns();

  ~TexturesIns();

  sf::Texture *getTexturePointer(const std::string &textureName) const;

private:
  static std::vector<std::string> textureNames;
  std::unordered_map<std::string, sf::Texture *> textureNamePointer;

  void operator delete(void *ptr) noexcept;
};

using Textures = Singleton<TexturesIns>;

} // namespace game
