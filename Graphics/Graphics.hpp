#pragma once

#include <vector>
#include "../Sprite/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

namespace game {

	class Graphics {
	public:
		Graphics();

		~Graphics();

		std::vector<Sprite*> sprites;

		void render();

	private:
		sf::RenderWindow* window;
	};

}