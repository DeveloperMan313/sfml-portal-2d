#include "Textures.hpp"
#include <stdexcept>

namespace game {

Textures *Textures::textures = nullptr;

std::vector<std::string> Textures::textureNames = {
    "placeholder", "pressedButton", "unpressedButton", "wall",
    "portalBlue",  "portalRed",     "companionCube",   "player"};

Textures::Textures() {
  if (Textures::textures != nullptr) {
    throw std::runtime_error("Textures can only have one instance");
  }
  for (const std::string &textureName : Textures::textureNames) {
    sf::Texture *texture = new sf::Texture;
    texture->loadFromFile("data/textures/" + textureName + ".png");
    this->textureNamePointer.insert({textureName, texture});
  }
  Textures::textures = this;
}

Textures::~Textures() {
  for (const std::string &textureName : Textures::textureNames) {
    delete this->getTexturePointer(textureName);
  }
  Textures::textures = nullptr;
}

sf::Texture *Textures::getTexturePointer(const std::string &textureName) const {
  return this->textureNamePointer.at(textureName);
}

} // namespace game
