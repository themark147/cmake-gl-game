#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <iostream>

namespace GLGame {
	class CollisionEventListener : public reactphysics3d::EventListener {
	public:
		void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) override {
            for (int i = 0; i < callbackData.getNbOverlappingPairs(); i++) {
                const reactphysics3d::OverlapCallback::OverlapPair& overlapPair = callbackData.getOverlappingPair(i);
                
                // Get the colliders involved in the overlap
                reactphysics3d::Collider* collider1 = overlapPair.getCollider1();
                reactphysics3d::Collider* collider2 = overlapPair.getCollider2();
            }
		}

        virtual void onContact(const CollisionCallback::CallbackData& callbackData) override {}
	};
}