#include "Material.h"

using namespace GLGame;

namespace GLGame {
	Material::Material() {}

	Material::Material(vector<Texture*>& textures) : textures(textures)
	{
	}

	void Material::setTextures(vector<Texture*> &textures) {
		this->textures = textures;
	}

	vector<Texture*> Material::getTextures()
	{
		return this->textures;
	}
}