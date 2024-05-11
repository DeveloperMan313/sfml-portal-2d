#include "Graphics.hpp"

namespace game {

game::Graphics::Graphics() {
  this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Portal 2D");
}

Graphics::~Graphics() {
  this->window->close();
  delete this->window;
}

void Graphics::render(const std::vector<Sprite*>& sprites) {
  this->window->clear();
  for (const Sprite *sprite : sprites) {
    this->window->draw(*sprite);
  }
  this->window->display();
}

void Graphics::changeDisplayResolution(std::vector<int> resolution){
  this->window->close();
  delete this->window;
  this->window = new sf::RenderWindow(sf::VideoMode(resolution[0], resolution[1]), "Portal 2D");
}

} // namespace game
