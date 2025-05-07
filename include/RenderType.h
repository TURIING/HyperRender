/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/7 14:05
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef RENDERTYPE_H
#define RENDERTYPE_H

#include <cstdint>

namespace HyperRender
{
    struct Size {
        uint32_t width = 0;
        uint32_t height = 0;

    	bool operator==(const Size& rhs) const {
    		return width == rhs.width && height == rhs.height;
    	}
    };

	struct Offset2D {
		int32_t x = 0;
		int32_t y = 0;

		bool operator==(const Offset2D &rhs) const {
			return x == rhs.x && y == rhs.y;
		}
	};

	struct Area {
		Offset2D offset;
		Size	 size;

		bool operator==(const Area& rhs) const {
			return offset == rhs.offset && size == rhs.size;
		}
	};

	struct Color {
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 0;
	};

    constexpr Color White = Color(1.0, 1.0, 1.0, 1.0);
    constexpr Color Red = Color(1.0, 0.0, 0.0, 1.0);
    constexpr Color Black = Color(0.0, 0.0, 0.0, 1.0);
	constexpr Color Green = Color(0.0, 1.0, 0.0, 1.0);
	constexpr Color Blue = Color(0.0, 0.0, 1.0, 1.0);
}

#endif //RENDERTYPE_H
