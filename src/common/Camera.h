/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/12 13:47
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera final : public RenderObject {
public:
    Camera();
    [[nodiscard]] glm::mat4 GetViewMatrix() const { return m_view; }
    [[nodiscard]] glm::mat4 GetProjectionMatrix() const { return m_projection; }
    void                    SetLookAt(const glm::vec3& eye, const glm::vec3& look, const glm::vec3& up);
    void                    SetOrtho(float left, float right, float bottom, float top, float near, float far);
    void                    SetFrustum(float left, float right, float bottom, float top, float near, float far);

private:
    glm::mat4 m_view       = glm::mat4(1.0f);
    glm::mat4 m_projection = glm::mat4(1.0f);
};

#endif //CAMERA_H
