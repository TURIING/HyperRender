/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/12 13:47
* @version: 1.0
* @description: 
********************************************************************************/
#include "Camera.h"
USING_RENDER_NAMESPACE_BEGIN

Camera::Camera() {
    this->SetLookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::SetLookAt(const glm::vec3& eye, const glm::vec3& look, const glm::vec3& up) {
    m_view = glm::lookAt(eye, look, up);
}

void Camera::SetOrtho(float left, float right, float bottom, float top, float near, float far) {
    m_projection = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::SetFrustum(float left, float right, float bottom, float top, float near, float far) {
    m_projection = glm::frustumRH_ZO(left, right, bottom, top, near, far);
}

USING_RENDER_NAMESPACE_END