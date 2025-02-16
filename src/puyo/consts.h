#ifndef PPP_PUYO_CONSTS_H
#define PPP_PUYO_CONSTS_H

#include "../core/types.h"
#include "../core/consts.h"
#include <raylib/raylib.h>

namespace ppp_puyo {
	const uint32 BEAN_SIZE = 24;
	const uint32 GRID_COLS = 10;
	const uint32 GRID_ROWS = 16;

	constexpr uint32 GRID_SIZE = GRID_COLS * GRID_ROWS;
	constexpr uint32 GRID_WIDTH_PX = BEAN_SIZE * GRID_COLS;
	constexpr uint32 GRID_HEIGHT_PX = BEAN_SIZE * GRID_ROWS;

	constexpr float FREE_FALL = BEAN_SIZE * GRID_ROWS;

	const uint32 GRID_OFFSET_X = 10;
	constexpr uint32 GRID_OFFSET_Y = (ppp_core::GAME_SCREEN_HEIGHT - GRID_HEIGHT_PX) >> 1;

	enum BeanType : uint8 {
		RED_BEAN,
		GREEN_BEAN,
		BLUE_BEAN,
		YELLOW_BEAN,

		NUM_COLORS,

		EMPTY_BEAN
	};

	Color BeanColors[BeanType::NUM_COLORS] = {
		RED,
		GREEN,
		BLUE,
		YELLOW
	};
}

#endif // ! PPP_PUYO_CONSTS_H