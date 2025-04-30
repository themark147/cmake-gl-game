#include "Animator.h"

namespace GLGame {
	void Animator::animateSkeleton(Shader& shader, GLGame::AnimationNode& animation, GLGame::Bone& skeleton) {
		if (!animation.boneTransforms.empty()) {
			int time = (int(glfwGetTime() * 1000.0f) - 1000) % int(std::floor(animation.duration / 1000.0f));

			getPose(animation, skeleton, float(time), currentPose, identity, globalInverseTransform);
			shader.setMat4Array("bone_transforms", currentPose, currentPose.size());
		}
	}
}