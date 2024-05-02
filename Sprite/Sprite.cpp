#include "Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace game {

	Sprite::Sprite(const std::string& textureName, const Textures& textures)
	{
		this->setTexture(*textures.getTexturePointer(textureName));
	}

}