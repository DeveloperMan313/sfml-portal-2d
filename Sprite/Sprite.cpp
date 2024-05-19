#include "Sprite.hpp"

namespace game {

Sprite::Sprite(const std::string &textureName, const Textures &textures, int layIndex) : layerIndex(layIndex) {
  this->setTexture(*textures.getTexturePointer(textureName));
}

void Sprite::setLayerIndex(int& index) {
  this->layerIndex = index;
}

} // namespace game
