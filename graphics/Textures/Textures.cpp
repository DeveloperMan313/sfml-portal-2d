#include "Textures.hpp"

namespace game {

std::vector<std::string> TexturesIns::textureNames = {
    "placeholder", "pressedButton", "unpressedButton", "wall",
    "portalBlue",  "portalRed",     "companionCube",   "player"};

TexturesIns::TexturesIns() {
  for (const std::string &textureName : TexturesIns::textureNames) {
    sf::Texture *texture = new sf::Texture;
    texture->loadFromFile("data/textures/" + textureName + ".png");
    this->textureNamePointer.insert({textureName, texture});
  }
}

TexturesIns::~TexturesIns() {
  for (const std::string &textureName : TexturesIns::textureNames) {
    delete this->getTexturePointer(textureName);
  }
}

sf::Texture *
TexturesIns::getTexturePointer(const std::string &textureName) const {
  return this->textureNamePointer.at(textureName);
}

void TexturesIns::operator delete(void *ptr) noexcept {};

} // namespace game
