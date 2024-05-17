#include "Graphics.hpp"

namespace game {

game::Graphics::Graphics() {
  this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Portal 2D");
  this->textures = new Textures();
  this->menu = new MainMenu(*window, *textures);
}

Graphics::~Graphics() {
  this->window->close();
  delete this->window;
}

void Graphics::render(const renderModes& renderMode){
    window->clear();
    if (renderMode == renderModes::menuMode) {
        menu->run();
        
    }
    else if (renderMode == renderModes::gameMode) {
    }
}

void Graphics::renderSprites(const std::vector<Sprite*>& sprites) {
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

bool Graphics::pollEvent(sf::Event& event){
    return this->window->pollEvent(event);
}

void Graphics::closeWindow()
{
    this->window->close();
    delete this->window;
}

} // namespace game
