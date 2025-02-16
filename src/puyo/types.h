#ifndef PPP_PUYO_TYPES_H
#define PPP_PUYO_TYPES_H

#include "consts.h"
#include "../core/types.h"

namespace ppp_puyo {
	struct Bean {
		int32 x_pos, y_pos;
		BeanType type;
		uint8 connections;
	};

	struct Player {
		Bean grid[GRID_SIZE];
		Bean pivot_bean;
		Bean edge_bean;

		float fall_timer;
		uint8 rotation;

		bool can_move;
	};
}

#endif // ! PPP_PUYO_TYPES_H