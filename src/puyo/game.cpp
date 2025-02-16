#include "game.h"

#include "types.h"

ppp_puyo::Player human;

static void InitPlayer(ppp_puyo::Player& player) {
	// Init pivot bean
	player.pivot_bean.connections = 0;
	player.pivot_bean.type = (ppp_puyo::BeanType)(
		GetRandomValue(0, ppp_puyo::BeanType::NUM_COLORS - 1)
	);
	player.pivot_bean.x_pos = (ppp_puyo::GRID_COLS >> 1) * ppp_puyo::BEAN_SIZE;
	player.pivot_bean.y_pos = 0.0f;

	// Init edge bean
	player.edge_bean.connections = 0;
	player.edge_bean.type = (ppp_puyo::BeanType)(
		GetRandomValue(0, ppp_puyo::BeanType::NUM_COLORS - 1)
	);
	player.edge_bean.x_pos = player.pivot_bean.x_pos - ppp_puyo::BEAN_SIZE;
	player.edge_bean.y_pos = 0.0f;

	// Init grid
	for (int32 gx = 0; gx < ppp_puyo::GRID_COLS; gx++) {
		for (int32 gy = 0; gy < ppp_puyo::GRID_ROWS; gy++) {
			ppp_puyo::Bean& bean = player.grid[gx + (gy * ppp_puyo::GRID_COLS)];
			bean.connections = 0;
			bean.type = ppp_puyo::BeanType::EMPTY_BEAN;
			bean.x_pos = gx * ppp_puyo::BEAN_SIZE;
			bean.y_pos = gy * ppp_puyo::BEAN_SIZE;
		}
	}

	// Init timers
	player.fall_timer = 0.0f;

	// Init move state
	player.can_move = true;
}

void ppp_puyo::StartGame() {
	// Init players
	InitPlayer(human);
}

void ppp_puyo::UpdateGame(const float delta_time) {
	// Update timer values
	human.fall_timer += delta_time;

	// Get Input
	int8 x_input = (
		(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) -
		(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
	);
	bool y_input = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
	bool spin_input = IsKeyPressed(KEY_SPACE);
}

static void DrawPlayer(
	ppp_puyo::Player& player,
	const int32 x_origin,
	const int32 y_origin
) {
	// Draw backdrop
	DrawRectangle(
		x_origin,
		y_origin,
		ppp_puyo::GRID_WIDTH_PX,
		ppp_puyo::GRID_HEIGHT_PX,
		BLACK
	);

	// Draw grid
	for (uint32 idx = 0; idx < ppp_puyo::GRID_SIZE; idx++) {
		ppp_puyo::Bean& bean = player.grid[idx];
		if (bean.type == ppp_puyo::BeanType::EMPTY_BEAN) continue;

		DrawRectangle(
			bean.x_pos + x_origin,
			bean.y_pos + y_origin,
			ppp_puyo::BEAN_SIZE,
			ppp_puyo::BEAN_SIZE,
			ppp_puyo::BeanColors[bean.type]
		);
	}

	// Draw moving beans
	DrawRectangle(
		player.pivot_bean.x_pos + x_origin,
		player.pivot_bean.y_pos + y_origin,
		ppp_puyo::BEAN_SIZE,
		ppp_puyo::BEAN_SIZE,
		ppp_puyo::BeanColors[player.pivot_bean.type]
	);
	DrawRectangle(
		player.edge_bean.x_pos + x_origin,
		player.edge_bean.y_pos + y_origin,
		ppp_puyo::BEAN_SIZE,
		ppp_puyo::BEAN_SIZE,
		ppp_puyo::BeanColors[player.edge_bean.type]
	);
}

void ppp_puyo::DrawGame() {
	DrawPlayer(human, ppp_puyo::GRID_OFFSET_X, GRID_OFFSET_Y);
}
