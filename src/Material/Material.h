#pragma once

#include <string>
#include <vector>
#include "Texture.h"

using namespace std;
using namespace GLGame;

namespace GLGame {
	// TODO: probably should contain scalar values of roughness / metallic
	class Material {
	public:
		Material();
		void setTextures(vector<Texture*> &textures);
		vector<Texture*> getTextures();
	private:
		//Texture* diffuse;
		//Texture* normal;

		vector<Texture*> textures;
	};
}