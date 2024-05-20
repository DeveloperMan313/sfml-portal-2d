#include "Textures.hpp"

namespace game {
std::vector<std::string> Textures::textureNames = {
    "placeholder", "pressedButton", "unpressedButton", "wall",
    "portalBlue",  "portalRed",     "companionCube",   "player"};

Textures::Textures() {
  for (const std::string &textureName : Textures::textureNames) {
    sf::Texture *texture = new sf::Texture;
    texture->loadFromFile("data/textures/" + textureName + ".png");
    this->textureNamePointer.insert({textureName, texture});
  }
}

Textures::~Textures() {
  for (const std::string &textureName : Textures::textureNames) {
    delete this->getTexturePointer(textureName);
  }
}

sf::Texture *Textures::getTexturePointer(const std::string &textureName) const {
  return this->textureNamePointer.at(textureName);
}

} // namespace game
