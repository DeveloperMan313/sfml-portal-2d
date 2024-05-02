#pragma once

#include "SFML/Graphics/Texture.hpp"
#include <unordered_map>
#include <vector>
#include <string>

namespace game {

	class Textures {
	public:
		Textures();

		~Textures();

		sf::Texture* getTexturePointer(const std::string& textureName) const; 

	private:
		static std::vector<std::string> textureNames;
		std::unordered_map<std::string, sf::Texture*> textureNamePointer;
	};

}