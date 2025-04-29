#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

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

		std::pair<unsigned int, float> getTimeFraction(std::vector<float>& times, float& dt) {
			auto it = std::lower_bound(times.begin() + 1, times.end(), dt);
			unsigned int segment = std::distance(times.begin(), it);

			float start = times[segment - 1];
			float end = times[segment];
			float frac = (dt - start) / (end - start);

			return std::make_pair(segment, frac);
		}
	};
}