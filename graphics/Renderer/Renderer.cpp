#include "Renderer.hpp"
#include "Textures.hpp"

namespace game {

game::RendererIns::RendererIns() {
  this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Portal 2D");
  Textures::createInstance();
  this->menu = new MainMenu(*window);
}

RendererIns::~RendererIns() {
  this->window->close();
  delete this->window;
  Textures::deleteInstance();
}

void RendererIns::render(const renderModes& renderMode, const std::vector<RigidBody*>& rbs){
  window->clear();
  if (renderMode == renderModes::menuMode) {
      menu->run();
        
  }
  else if (renderMode == renderModes::gameMode) {
    this->renderRBs(rbs);
  }

}

void RendererIns::renderRBs(const std::vector<RigidBody*> &rbs) {
  this->window->clear();
  for (const RigidBody *rb : rbs) {
    this->window->draw(*rb);
  }
  this->window->display();

}

void RendererIns::changeDisplayResolution(std::vector<int> resolution){
  this->window->close();
  delete this->window;
  this->window = new sf::RenderWindow(sf::VideoMode(resolution[0], resolution[1]), "Portal 2D");

}

bool RendererIns::pollEvent(sf::Event& event){
    return this->window->pollEvent(event);

}

void RendererIns::closeWindow(){
  this->window->close();
  delete this->window;

}

void RendererIns::setLevelSize(const sf::Vector2i& size){
  view.setSize(size.x, size.y);
}

void RendererIns::setPlayHandler(const std::function<void(void)>& ph){
  menu->playHandler = ph;

}

void RendererIns::setSettingsHandler(const std::function<void(void)>& sh){
  menu->settingHandler = sh;

}

void RendererIns::setExitHandler(const std::function<void(void)>& eh){
  menu->exitHandler = eh;

}

void RendererIns::setGameMode(const renderModes& gameMode)
{
  this->gameMode = gameMode;
  if (gameMode == renderModes::menuMode)
    this->window->setView(this->window->getDefaultView());
  else 
    this->window->setView(this->view);
}

void RendererIns::operator delete(void *ptr) noexcept {};

} // namespace game
