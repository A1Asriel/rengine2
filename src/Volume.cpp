#include "Volume.h"
#include <vector>

void REngine::Plane::setNormalAndDistance(const glm::mat4& projView, uint8_t axis, bool positive) {
    this->normal.x = projView[0][3] + (projView[0][axis] * (positive ? 1 : -1));
    this->normal.y = projView[1][3] + (projView[1][axis] * (positive ? 1 : -1));
    this->normal.z = projView[2][3] + (projView[2][axis] * (positive ? 1 : -1));
    this->distance = projView[3][3] + (projView[3][axis] * (positive ? 1 : -1));
}

REngine::Frustum::Frustum(const Camera& camera, float aspect) {
    const glm::mat4 proj = camera.getProjectionMatrix();
    const glm::mat4 view = camera.getViewMatrix();
    const glm::mat4 projView = proj * view;

    left.setNormalAndDistance(projView, 0, false);
    right.setNormalAndDistance(projView, 0, true);
    bottom.setNormalAndDistance(projView, 1, false);
    top.setNormalAndDistance(projView, 1, true);
    near.setNormalAndDistance(projView, 2, false);
    far.setNormalAndDistance(projView, 2, true);
}

bool REngine::Frustum::isBoxInFrustum(const glm::vec3& min, const glm::vec3& max) const {
    const Plane planes[6] = {top, bottom, left, right, near, far};
    
    for (const Plane& plane : planes) {
        glm::vec3 p = min;
        
        if (plane.normal.x >= 0) p.x = max.x;
        if (plane.normal.y >= 0) p.y = max.y;
        if (plane.normal.z >= 0) p.z = max.z;

        if (glm::dot(plane.normal, p) + plane.distance < 0.0f) {
            return false;
        }
    }
    return true;
}
