#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Common/OpenGL.h"
#include "../Shader.h"

#include <vector>
#include <unordered_map>
#include <utility> // for std::pair

namespace GLGame {
    // structure to hold bone tree (skeleton)
    struct Bone {
        int id = 0; // position of the bone in final upload array
        std::string name = "";
        glm::mat4 offset = glm::mat4(1.0f);
        std::vector<Bone> children = {};
    };

    // sturction representing an animation track
    struct BoneTransformTrack {
        std::vector<float> positionTimestamps = {};
        std::vector<float> rotationTimestamps = {};
        std::vector<float> scaleTimestamps = {};

        std::vector<glm::vec3> positions = {};
        std::vector<glm::quat> rotations = {};
        std::vector<glm::vec3> scales = {};
    };

    struct AnimationNode {
        float duration = 0.0f;
        float ticksPerSecond = 1.0f;
        std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
    };

	class Animator {
	public:
		void animateSkeleton(Shader& shader, GLGame::AnimationNode& animation, GLGame::Bone& skeleton);

		void getPose(AnimationNode& animation, Bone& skeleton, float dt, std::vector<glm::mat4>& output, glm::mat4& parentTransform, glm::mat4& globalInverseTransform) {
			BoneTransformTrack& btt = animation.boneTransforms[skeleton.name];
			dt = fmod(dt, animation.duration);
			std::pair<unsigned int, float> fp;
			//calculate interpolated position
			fp = getTimeFraction(btt.positionTimestamps, dt);

			glm::vec3 position1 = btt.positions[fp.first - 1];
			glm::vec3 position2 = btt.positions[fp.first];

			glm::vec3 position = glm::mix(position1, position2, fp.second);

			//calculate interpolated rotation
			fp = getTimeFraction(btt.rotationTimestamps, dt);
			glm::quat rotation1 = btt.rotations[fp.first - 1];
			glm::quat rotation2 = btt.rotations[fp.first];

			glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);

			//calculate interpolated scale
			fp = getTimeFraction(btt.scaleTimestamps, dt);
			glm::vec3 scale1 = btt.scales[fp.first - 1];
			glm::vec3 scale2 = btt.scales[fp.first];

			glm::vec3 scale = glm::mix(scale1, scale2, fp.second);

			glm::mat4 positionMat = glm::mat4(1.0),
				scaleMat = glm::mat4(1.0);


			// calculate localTransform
			positionMat = glm::translate(positionMat, position);
			glm::mat4 rotationMat = glm::toMat4(rotation);
			scaleMat = glm::scale(scaleMat, scale);
			glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
			glm::mat4 globalTransform = parentTransform * localTransform;

			output[skeleton.id] = globalInverseTransform * globalTransform * skeleton.offset;
			//update values for children bones
			for (Bone& child : skeleton.children) {
				getPose(animation, child, dt, output, globalTransform, globalInverseTransform);
			}

			// std::cout << dt << " => " << position.x << ":" << position.y << ":" << position.z << ":" << std::endl;
		}

		void load(const aiScene* scene, GLGame::AnimationNode& animation) {
			globalInverseTransform = assimpToGlmMatrix(scene->mRootNode->mTransformation);
			globalInverseTransform = glm::inverse(globalInverseTransform);

			//currentPose is held in this vector and uploaded to gpu as a matrix array uniform
			currentPose.resize(100, identity); // TODO cannot be hardcoded 

			//loading first Animation
			if (scene->HasAnimations()) {
				aiAnimation* anim = scene->mAnimations[0];

				if (anim->mTicksPerSecond != 0.0f)
					animation.ticksPerSecond = anim->mTicksPerSecond;
				else
					animation.ticksPerSecond = 1;

				animation.duration = anim->mDuration * anim->mTicksPerSecond;
				animation.boneTransforms = {};

				// load positions rotations and scales for each bone
				// each channel represents each bone
				for (int i = 0; i < anim->mNumChannels; i++) {
					aiNodeAnim* channel = anim->mChannels[i];
					GLGame::BoneTransformTrack track;
					for (int j = 0; j < channel->mNumPositionKeys; j++) {
						track.positionTimestamps.push_back(channel->mPositionKeys[j].mTime);
						track.positions.push_back(assimpToGlmVec3(channel->mPositionKeys[j].mValue));
					}
					for (int j = 0; j < channel->mNumRotationKeys; j++) {
						track.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
						track.rotations.push_back(assimpToGlmQuat(channel->mRotationKeys[j].mValue));

					}
					for (int j = 0; j < channel->mNumScalingKeys; j++) {
						track.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
						track.scales.push_back(assimpToGlmVec3(channel->mScalingKeys[j].mValue));

					}
					animation.boneTransforms[channel->mNodeName.C_Str()] = track;
				}
			}
		}

		std::pair<unsigned int, float> getTimeFraction(std::vector<float>& times, float& dt) {
			auto it = std::lower_bound(times.begin() + 1, times.end(), dt);
			unsigned int segment = std::distance(times.begin(), it);

			float start = times[segment - 1];
			float end = times[segment];
			float frac = (dt - start) / (end - start);

			return std::make_pair(segment, frac);
		}

	private:
		glm::mat4 globalInverseTransform = glm::mat4(1.0);
		std::vector<glm::mat4> currentPose = {};
		glm::mat4 identity = glm::mat4(1.0);

		inline glm::vec3 assimpToGlmVec3(aiVector3D vec) {
			return glm::vec3(vec.x, vec.y, vec.z);
		}

		inline glm::quat assimpToGlmQuat(aiQuaternion quat) {
			glm::quat q;
			q.x = quat.x;
			q.y = quat.y;
			q.z = quat.z;
			q.w = quat.w;

			return q;
		}

		inline glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat) {
			glm::mat4 m;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					m[x][y] = mat[y][x];
				}
			}
			return m;
		}
	};
}