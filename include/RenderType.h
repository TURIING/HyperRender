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

    	bool operator>=(const Size& rhs) const {
    		return width >= rhs.width && height >= rhs.height;
    	}

    	Size operator/(uint32_t v) const {
    		return { width / v, height / v };
    	}

    	Size& operator/=(uint32_t v) {
    		width  /= v;
    		height /= v;
    		return *this;
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

		static Color White;
		static Color Black;
		static Color Red;
		static Color Green;
	};

	enum class BlendType {
		Multiply,			    // 正片叠底
		Darken,					// 变暗
		ColorBurn,			    // 颜色加深
		ColorLinearBurn, 		// 线性加深
		DarkColor,			    // 深色
		Normal,					// 正常
		Lighten,			    // 变亮
		Screen,					// 滤色
		ColorDodge,			    // 颜色减淡
		Add,					// 添加 (线性减淡)
		LightColor,			    // 浅色
		Cover,					// 覆盖 | 叠加
		SoftLight,			  	// 柔光
		HardLight,			  	// 强光
		ViviLight,			  	// 亮光
		LinearLight,			// 线性光
		PinLight,			  	// 点光
		HardMix,			  	// 实色混合
		Difference,			  	// 差值
		Exclusion,			  	// 排除
		Subtract,			  	// 减去
		Divide,					// 划分
		HubHsv,					// 色相
		SaturationHsv,			// 饱和度
		ColorHsv,				// 颜色
		LuminosityHsv,			// 明度
	};

}

#endif //RENDERTYPE_H
