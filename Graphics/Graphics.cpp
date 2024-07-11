#include "Graphics.hpp"
#include "Textures.hpp"

namespace game {

game::GraphicsIns::GraphicsIns() {
  this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Portal 2D");
  Textures::createInstance();
  this->menu = new MainMenu(*window);
}

GraphicsIns::~GraphicsIns() {
  this->window->close();
  delete this->window;
  Textures::deleteInstance();
}

void GraphicsIns::render(const renderModes& renderMode, const std::vector<Sprite*>& sprites){
  window->clear();
  if (renderMode == renderModes::menuMode) {
      menu->run();
        
  }
  else if (renderMode == renderModes::gameMode) {
    this->renderSprites(sprites);
  }

}

void GraphicsIns::renderSprites(const std::vector<Sprite*>& sprites) {
  this->window->clear();
  for (const Sprite *sprite : sprites) {
    this->window->draw(*sprite);
  }
  this->window->display();

}

void GraphicsIns::changeDisplayResolution(std::vector<int> resolution){
  this->window->close();
  delete this->window;
  this->window = new sf::RenderWindow(sf::VideoMode(resolution[0], resolution[1]), "Portal 2D");

}

bool GraphicsIns::pollEvent(sf::Event& event){
    return this->window->pollEvent(event);

}

void GraphicsIns::closeWindow(){
  this->window->close();
  delete this->window;

}

void GraphicsIns::setLevelSize(const sf::Vector2i& size){
  view.setSize(size.x, size.y);
}

void GraphicsIns::setPlayHandler(const std::function<void(void)>& ph){
  menu->playHandler = ph;

}

void GraphicsIns::setSettingsHandler(const std::function<void(void)>& sh){
  menu->settingHandler = sh;

}

void GraphicsIns::setExitHandler(const std::function<void(void)>& eh){
  menu->exitHandler = eh;

}

void GraphicsIns::setGameMode(const renderModes& gameMode)
{
  this->gameMode = gameMode;
  if (gameMode == renderModes::menuMode)
    this->window->setView(this->window->getDefaultView());
  else 
    this->window->setView(this->view);
}

void GraphicsIns::operator delete(void *ptr) noexcept {};

} // namespace game
