#include "Frame.hpp"

namespace game {

void Frame::add(const sf::Drawable *drawable) {
  this->drawables.push_back(drawable);
}

void Frame::render(sf::RenderTarget &target) const {
  for (const sf::Drawable *drawable : this->drawables) {
    target.draw(*drawable);
  }
}

} // namespace game
