/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/7 13:27
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef COMMON_H
#define COMMON_H

#define USING_RENDER_NAMESPACE_BEGIN namespace HyperRender {
#define USING_RENDER_NAMESPACE_END }

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Render.h"
#include "RenderType.h"
#include "LogManager.h"
#include "DrawUnit.h"

template <typename T>
uint32_t TO_U32(T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be numeric");
    return static_cast<uint32_t>(value);
}

template <typename T>
int32_t TO_I32(T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be numeric");
    return static_cast<int32_t>(value);
}

#endif //COMMON_H
