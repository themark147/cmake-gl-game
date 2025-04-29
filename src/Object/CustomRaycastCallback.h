#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <iostream>
#include <vector>

using namespace reactphysics3d;

namespace GLGame {
	class CustomRaycastCallback : public RaycastCallback {
    public:
        /*struct CustomRaycastInfo {
            Vector3 worldHitPoint;
        };*/

        //std::vector<CustomRaycastInfo> hits;
        
        bool isOnGround = false;

        // CustomRaycastCallback(Vector3 rayOrigin)

		virtual decimal notifyRaycastHit(const RaycastInfo& raycastInfo) override {
            // hits.push_back({raycastInfo.worldPoint});

           // if (raycastInfo.worldPoint.y < raycastInfo..y) {
               // isOnGround = true;
                //groundNormal = raycastInfo.worldNormal;
           // }

            isOnGround = true;

            /*std::cout << "Hit point : " <<
                raycastInfo.worldPoint.x <<
                raycastInfo.worldPoint.y <<
                raycastInfo.worldPoint.z <<
                std::endl;*/

            // Return a fraction of 1.0 to gather all hits
            return decimal(1.0);
		}
	};
}