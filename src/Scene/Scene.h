#pragma once

#include <vector>
#include <reactphysics3d/reactphysics3d.h>

#include "../Object/Object.h"
#include "../Player/Player.h"
#include "../Scene/ShadowMap.h"

#include "../Shader.h"
#include "../Camera.h"
#include "../Model/Model.h"
#include "../Physics/Physics.h"

#include "../Debug/DebugRender.h"

#include "../ObjectSpawner.h"

using namespace reactphysics3d;

extern std::string vertexShaderMain, fragmentShaderMain,
	vertexShaderRender, fragmentShaderRender,
	vertexShaderDebug, fragmentShaderDebug,
	vertexShaderSimpleDepth, fragmentShaderSimpleDepth,
	vertexWater, fragmentWater,
	vertexHeight, fragmentHeight;

namespace GLGame {
	class Scene {
	public:
		Scene();
		void render();
		
		GLGame::Player& getPlayer() {
			return player;
		}

		GLGame::Physics& getPhysics() {
			return physics;
		}

	private:
		GLGame::Physics physics;
		// glm::mat4 playerModelTransform;

		GLGame::ShadowMap shadowMap;

		GLGame::DebugRender debugRender = GLGame::DebugRender();

		// TODO: object should contain model
		std::vector<Object> objects;

		GLGame::Player player = GLGame::Player(
			glm::vec3(0.0f, 0.0f, 15.0f),
			Model(std::string("first_person_arms_ue5_5.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(.02f))
		);

		Camera& camera = Camera();

		Shader debugShader = Shader(std::string("debug.vs").insert(0, GLGame::SHADER_PATH).c_str(), std::string("debug.fs").insert(0, GLGame::SHADER_PATH).c_str());
		Shader mainShader = Shader(std::string("mainWShadow.vs").insert(0, GLGame::SHADER_PATH).c_str(), std::string("mainWShadow.fs").insert(0, GLGame::SHADER_PATH).c_str());
		Shader simpleDepthShader = Shader(std::string("simpleDepth.vs").insert(0, GLGame::SHADER_PATH).c_str(), std::string("simpleDepth.fs").insert(0, GLGame::SHADER_PATH).c_str());
		Shader simpleWaterShader = Shader(std::string("water.vs").insert(0, GLGame::SHADER_PATH).c_str(), std::string("water.fs").insert(0, GLGame::SHADER_PATH).c_str());
		Shader heightShader = Shader(std::string("height.vs").insert(0, GLGame::SHADER_PATH).c_str(), std::string("height.fs").insert(0, GLGame::SHADER_PATH).c_str());
		//Shader mainShader = Shader(vertexShaderMain, fragmentShaderMain);
		//Shader simpleDepthShader = Shader(vertexShaderSimpleDepth, fragmentShaderSimpleDepth);
	};
}