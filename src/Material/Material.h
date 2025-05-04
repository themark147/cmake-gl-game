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
		Material(vector<Texture*>& textures);
		void setTextures(vector<Texture*> &textures);
		vector<Texture*> getTextures();
	private:
		vector<Texture*> textures;
	};
}