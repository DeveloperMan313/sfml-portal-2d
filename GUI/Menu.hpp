#pragma once

#include "SFML/Graphics.hpp"
#include "Button.hpp"
#include <iostream>

namespace game {

  class MainMenu {
  public:
    void start() { std::cout << "...."; };

    MainMenu(sf::RenderWindow*, const Textures& textures);

    void Run();

    void draw();

    Button* buttonsPointers[3];

    void processEvent();

    ~MainMenu() {};

  private:
    void setupMenu(const Textures& textures);

    sf::RenderWindow &window;

    sf::Font font;

    bool exit;

  };

}