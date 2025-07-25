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

#include "HyperGpu.h"

#include "Render.h"
#include "RenderType.h"
#include "LogManager.h"

template <typename T>
uint32_t TO_U32(T value) {
    static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "T must be numeric or enum");
    return static_cast<uint32_t>(value);
}

template <typename T>
int32_t TO_I32(T value) {
    static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "T must be numeric or enum");
    return static_cast<int32_t>(value);
}

template <typename T>
int32_t TO_F32(T value) {
    static_assert(std::is_arithmetic_v<T>, "T must be numeric");
    return static_cast<float>(value);
}

template <typename T>
struct Point {
    T x;
    T y;
};
using PointF = Point<float>;

#define BEGIN_CMD_DEBUG_LABEL(cmd, name) cmd->BeginDebugUtilsLabel(name);
#define END_CMD_DEBUG_LABEL(cmd, name) cmd->EndDebugUtilsLabel();
#define NODISCARD [[nodiscard]]

#endif //COMMON_H
