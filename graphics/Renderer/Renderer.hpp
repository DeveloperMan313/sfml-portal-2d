#pragma once

#include "Menu.hpp"
#include "RigidBody.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Singleton.hpp"
#include <functional>
#include <vector>

namespace game {

enum class renderModes { gameMode, menuMode };

class RendererIns {
  friend class Singleton<RendererIns>;

public:
  RendererIns();

  ~RendererIns();

  void render(const renderModes &renderMode,
              const std::vector<RigidBody *> &rbs);

  void renderRBs(const std::vector<RigidBody *> &rbs);

  void changeDisplayResolution(std::vector<int> resolution);

  bool pollEvent(sf::Event &);

  void closeWindow();

  void setLevelSize(const sf::Vector2i &size);

  void setPlayHandler(const std::function<void(void)> &ph);

  void setSettingsHandler(const std::function<void(void)> &sh);

  void setExitHandler(const std::function<void(void)> &eh);

  void setGameMode(const renderModes &gameMode);

private:
  MainMenu *menu;

  renderModes gameMode;

  sf::RenderWindow *window;

  sf::View view;

  void operator delete(void *ptr) noexcept;
};

using Renderer = Singleton<RendererIns>;

} // namespace game
