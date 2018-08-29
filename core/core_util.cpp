#include "core_util.hpp"

#include <algorithm>
#include <cstdint>


namespace core {

	//------------------------------------------------------------
	// Operations on rectangles
	//------------------------------------------------------------


	SDL_Rect scale(SDL_Rect rect, float amount) {
		if (amount == 1.f) return rect;
		if (amount <= 0.f) return {rect.x, rect.y, 0, 0};

		int cx_original = centerX(rect);
		int cy_original = centerY(rect);

		float fl_x = static_cast<float>(rect.x);
		float fl_y = static_cast<float>(rect.y);
		float fl_w = static_cast<float>(rect.w);
		float fl_h = static_cast<float>(rect.h);

		fl_x *= amount;
		fl_y *= amount;
		fl_w = (fl_x + fl_w)*amount - fl_x;
		fl_h = (fl_y + fl_h)*amount - fl_y;

		rect.x = static_cast<int>(fl_x);
		rect.y = static_cast<int>(fl_y);
		rect.w = static_cast<int>(fl_w);
		rect.h = static_cast<int>(fl_h);

		rect.x += cx_original - centerX(rect);
		rect.y += cy_original - centerY(rect);

		return rect;
	}


	bool isInside(int x, int y, const SDL_Rect& rect) {
		if (x < rect.x) return false;
		if (y < rect.y) return false;
		if (x > rect.x + rect.w) return false;
		if (y > rect.y + rect.h) return false;

		return true;
	}


	//------------------------------------------------------------
	// Extra
	//------------------------------------------------------------


	float rsqrt(float x) {
		static constexpr int MAGIC = 0x5f3759df;

		float half_x = x * 0.5f;
		int ieee = *(int*) &x;				// x's IEEE 32 bit representation as an int (pointers required to avoid conversion)

		ieee = MAGIC - (ieee >> 1);			// initial guess of Newton's method (Yup, I'm just as lost as you are!)
		x = *(float*) &ieee;				// cast back to float

		x = x * (1.5f - half_x*x*x); 		// 1st iteration of Newton's method
		//x = x * (1.5f - half_x*x*x); 		// 2nd iteration of Newton's method (for better estimation)
		return x;
	}

} // namespace core
