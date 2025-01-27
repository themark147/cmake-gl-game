#pragma once

#include <string>
#include <vector>

using namespace std;

namespace GLGame {
	struct Texture {
		unsigned int id;
		string name;

		Texture(int id, string name): id(id), name(name) {}
	};

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
};