#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include <string>
#include "../Textures/Textures.hpp"

namespace game {

	class Sprite : public sf::Sprite {
	public:
		Sprite(const std::string& textureName, const Textures& textures);

	};

}