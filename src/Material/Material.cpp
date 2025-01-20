#include "Material.h"

namespace GLGame {
	Material::Material() {}

	void Material::setTextures(vector<Texture*> &textures) {
		this->textures = textures;
	}

	vector<Texture*> Material::getTextures()
	{
		return this->textures;
	}
}