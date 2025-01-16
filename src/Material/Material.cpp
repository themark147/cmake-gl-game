#include "Material.h"

namespace GLGame {
	Material::Material(Texture diffuse, Texture normal, Texture metallic) : diffuse(diffuse), normal(normal), metallic(metallic) {
		this->textures.push_back(diffuse);
		this->textures.push_back(normal);
		this->textures.push_back(metallic);
	}

	vector<Texture> Material::getTextures()
	{
		return this->textures;
	}
}