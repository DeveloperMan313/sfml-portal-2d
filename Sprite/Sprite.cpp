#include "Sprite.hpp"

namespace game {

Sprite::Sprite(const std::string &textureName, size_t layerIdx)
    : layerIdx(layerIdx) {
  this->setTexture(*Textures::textures->getTexturePointer(textureName));
}

void Sprite::setLayerIdx(size_t idx) { this->layerIdx = idx; }

} // namespace game
