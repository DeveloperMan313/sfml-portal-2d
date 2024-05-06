#include "Graphics.hpp"

namespace game {

game::Graphics::Graphics() {
  this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Portal 2D");
}

Graphics::~Graphics() {
  this->window->close();
  delete this->window;
}

void Graphics::render() {
  this->window->clear();
  for (const Sprite *sprite : this->sprites) {
    this->window->draw(*sprite);
  }
  this->window->display();
}

} // namespace game
