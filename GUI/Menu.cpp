#include "Menu.hpp"

namespace game {

    game::MainMenu::MainMenu(sf::RenderWindow& wind, const Textures& textures) : window(wind) {
        setupMenu(textures);

    };

  void MainMenu::run(){
      processEvent();
      draw();

  }

  void MainMenu::draw()
  {
    window.clear();

    for (int i = 0; i < 3; i++) {
      window.draw(*buttonsPointers[i]->getSpritePointer());
      window.draw(*buttonsPointers[i]->getTextPointer());
    }

    window.display();
    
  }

  void MainMenu::processEvent(){
    sf::Event event;
    sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
          exitHandler();
      }
      else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        for (int i = 0; i < 3; i++)
          buttonsPointers[i]->update(sf::Mouse::getPosition(window), false);
        if (buttonsPointers[0]->getSpritePointer()->getGlobalBounds().contains(mousePosition))
            playHandler();
        if (buttonsPointers[1]->getSpritePointer()->getGlobalBounds().contains(mousePosition))
            settingHandler();
        if (buttonsPointers[2]->getSpritePointer()->getGlobalBounds().contains(mousePosition)) {
            exitHandler();
        }

      }
      if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        for (int i = 0; i < 3; i++)
          buttonsPointers[i]->update(sf::Mouse::getPosition(window), true);
    }

  }

  void MainMenu::setupMenu(const Textures& textures){
    font.loadFromFile("data/fonts/UbuntuSansMono.ttf");
    window.clear();


    buttonsPointers[0] = new Button(sf::Vector2f(window.getSize().x / 2 - 150, window.getSize().y / 1.7f - 50), sf::Vector2i(300, 75.f), "pressedButton", "unpressedButton", "START", textures);
    buttonsPointers[1] = new Button(sf::Vector2f(window.getSize().x / 2 - 150, window.getSize().y / 1.7f + 50), sf::Vector2i(300, 75.f), "pressedButton", "unpressedButton", "OPTIONS", textures);
    buttonsPointers[2] = new Button(sf::Vector2f(window.getSize().x / 2 - 150, window.getSize().y / 1.7f + 150), sf::Vector2i(300, 75.f), "pressedButton", "unpressedButton", "EXIT", textures);

  }

}