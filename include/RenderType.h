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
    };

	struct Offset2D {
		int32_t x = 0;
		int32_t y = 0;
	};

	struct Area {
		Offset2D offset;
		Size	 size;
	};

	}

#endif //RENDERTYPE_H
