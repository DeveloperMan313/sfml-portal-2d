#pragma once

#include "../Sprite/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "../GUI/Menu.hpp"
#include <vector>
#include <functional>

namespace game {

enum class renderModes { gameMode, menuMode };

class Graphics {
public:
  Graphics();

  ~Graphics();

  void render(const renderModes& renderMode, const std::vector<Sprite*> &sprites);

  void renderSprites(const std::vector<Sprite*> &sprites);

  void changeDisplayResolution(std::vector<int> resolution);

  bool pollEvent(sf::Event&);

  void closeWindow();

  void setLevelSize(const sf::Vector2i& size);

  void setPlayHandler(const std::function<void(void)>& ph);

  void setSettingsHandler(const std::function<void(void)>& sh);

  void setExitHandler(const std::function<void(void)>& eh);

private:  
	MainMenu* menu;

	Textures* textures;

	sf::RenderWindow* window;

  sf::View view;

};

} // namespace game
