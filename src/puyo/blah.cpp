//#include "game.h"
//
//#include "../core/math.h"
//#include "../core/types.h"
//
//#include <bitset>
//#include <raylib/raylib.h>
//
//constexpr uint32 BEAN_SIZE = 24;
//constexpr uint32 NUM_COLS = 11;
//constexpr uint32 NUM_ROWS = 16;
//constexpr uint32 GRID_WIDTH = BEAN_SIZE * NUM_COLS;
//constexpr uint32 GRID_HEIGHT = BEAN_SIZE * NUM_ROWS;
//
//enum BeanTypes : uint8 {
//	RED_BEAN,
//	GREEN_BEAN,
//	BLUE_BEAN,
//	YELLOW_BEAN,
//
//	NUM_COLOR_TYPES,
//
//	EMPTY_BEAN
//};
//
//Color BeanColors[NUM_COLOR_TYPES] = {
//	RED,
//	GREEN,
//	BLUE,
//	YELLOW
//};
//
//struct Bean {
//	BeanTypes type;
//	uint8 x_pos, y_pos;
//};
//
//struct Player {
//	BeanTypes next_type;
//	Bean cur_bean;
//	uint8 stone_queue;
//	BeanTypes grid[NUM_COLS * NUM_ROWS];
//	float timer;
//};
//
//Player human, computer;
//float action_time;
//
//static void InitPlayer(Player& player) {
//	player.next_type = (BeanTypes)GetRandomValue(0, NUM_COLOR_TYPES - 1);
//
//	player.cur_bean.x_pos = NUM_COLS >> 1;
//	player.cur_bean.y_pos = 0;
//	player.cur_bean.type = (BeanTypes)GetRandomValue(0, NUM_COLOR_TYPES - 1);
//
//	player.stone_queue = 0;
//	player.timer = 0.0f;
//
//	for (unsigned int idx = 0; idx < NUM_COLS * NUM_ROWS; idx++) {
//		player.grid[idx] = EMPTY_BEAN;
//	}
//}
//
//void ppp_puyo::StartGame() {
//	InitPlayer(human);
//	InitPlayer(computer);
//
//	action_time = 0.5f;
//}
//
//static bool IsSpaceEmpty(
//	const BeanTypes grid[NUM_COLS * NUM_ROWS],
//	const int8 x_pos, const int8 y_pos
//) {
//	if (x_pos < 0 || x_pos >= NUM_COLS) {
//		return false;
//	}
//
//	if (y_pos < 0 || y_pos >= NUM_ROWS) {
//		return false;
//	}
//
//	return grid[(y_pos * NUM_COLS) + x_pos] == EMPTY_BEAN;
//}
//
//static void SetNextBean(Player& player) {
//	player.cur_bean.x_pos = NUM_COLS >> 1;
//	player.cur_bean.y_pos = 0;
//	player.cur_bean.type = player.next_type;
//
//	player.next_type = (BeanTypes)GetRandomValue(0, NUM_COLOR_TYPES - 1);
//}
//
//void ppp_puyo::UpdateGame(const float delta_time) {
//	human.timer += delta_time;
//	computer.timer += delta_time;
//
//	int8 x_move = (
//		(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) -
//		(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
//		);
//
//	if (IsSpaceEmpty(human.grid, human.cur_bean.x_pos + x_move, human.cur_bean.y_pos)) {
//		human.cur_bean.x_pos += x_move;
//	}
//
//	if (human.timer >= action_time || IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
//		human.timer = 0.0f;
//
//		if (!IsSpaceEmpty(human.grid, human.cur_bean.x_pos, human.cur_bean.y_pos + 1)) {
//			human.grid[(human.cur_bean.y_pos * NUM_COLS) + human.cur_bean.x_pos] = human.cur_bean.type;
//			SetNextBean(human);
//		}
//		else {
//			human.cur_bean.y_pos += 1;
//		}
//	}
//}
//
//static void DrawPlayerGrid(const Player& player, const uint32 x_pos, const uint32 y_pos) {
//	// Draw the grid rectangle
//	DrawRectangle(x_pos, y_pos, GRID_WIDTH, GRID_HEIGHT, BLACK);
//
//	// Draw the grid beans
//	for (uint8 gx = 0; gx < NUM_COLS; gx++) {
//		for (uint8 gy = 0; gy < NUM_ROWS; gy++) {
//			const BeanTypes cur_type = player.grid[(gy * NUM_COLS) + gx];
//			if (cur_type == EMPTY_BEAN) continue;
//
//			DrawRectangle(
//				x_pos + gx * BEAN_SIZE,
//				y_pos + gy * BEAN_SIZE,
//				BEAN_SIZE,
//				BEAN_SIZE,
//				BeanColors[cur_type]
//			);
//		}
//	}
//
//	// Draw the player's current bean
//	DrawRectangle(
//		x_pos + player.cur_bean.x_pos * BEAN_SIZE,
//		y_pos + player.cur_bean.y_pos * BEAN_SIZE,
//		BEAN_SIZE,
//		BEAN_SIZE,
//		BeanColors[player.cur_bean.type]
//	);
//}
//
//void ppp_puyo::DrawGame() {
//	DrawPlayerGrid(human, 10, 10);
//	DrawPlayerGrid(computer, 790 - GRID_WIDTH, 10);
//}
