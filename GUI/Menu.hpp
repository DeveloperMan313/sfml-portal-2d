#pragma once

#include "Button.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <functional>

namespace game {

  class MainMenu {
  public:
    MainMenu(sf::RenderWindow& window);

    void run();

    std::function<void(void)> playHandler;

    std::function<void(void)> settingHandler;

    std::function<void(void)> exitHandler;

    void draw();

    Button* buttonsPointers[3];

    void processEvent();

    ~MainMenu() {};

    void setupMenu();

  private:

    sf::RenderWindow &window;

    sf::Font font;

    bool exit;

  };

}