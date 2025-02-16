#include "game.h"

#include "types.h"
#include "../core/math.h"
#include <bitset>

bool is_playing;
float game_tick_time;
ppp_puyo::Player human;
const int32 BEAN_PLACED = -999;
Texture2D puyo_texture;

static ppp_puyo::Bean& IndexGrid(ppp_puyo::Player& player, const int32 x_idx, const int32 y_idx) {
	return player.grid[x_idx + (ppp_puyo::GRID_COLS * y_idx)];
}

static void GetNewBeans(ppp_puyo::Player& player) {
	// Init pivot bean
	player.pivot_bean.connections = 0;
	player.pivot_bean.type = (ppp_puyo::BeanType)(
		GetRandomValue(0, ppp_puyo::BeanType::NUM_COLORS - 1)
		);
	player.pivot_bean.x_pos = ppp_puyo::GRID_COLS >> 1;
	player.pivot_bean.y_pos = 0;

	// Init edge bean
	player.edge_bean.connections = 0;
	player.edge_bean.type = (ppp_puyo::BeanType)(
		GetRandomValue(0, ppp_puyo::BeanType::NUM_COLORS - 1)
		);
	player.edge_bean.x_pos = player.pivot_bean.x_pos - 1;
	player.edge_bean.y_pos = 0;

	// Check loss condition
	if (
		IndexGrid(player, player.pivot_bean.x_pos, player.pivot_bean.y_pos).type != ppp_puyo::BeanType::EMPTY_BEAN ||
		IndexGrid(player, player.edge_bean.x_pos, player.edge_bean.y_pos).type != ppp_puyo::BeanType::EMPTY_BEAN
	) {
		is_playing = false;
	}
}

static void InitPlayer(ppp_puyo::Player& player) {
	// Get first beans
	GetNewBeans(player);

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

	// Init state
	player.rotation = 0b10;
	player.can_move = true;
}

void ppp_puyo::StartGame() {
	// Init players
	InitPlayer(human);

	// Init game state
	game_tick_time = 0.5f;
	is_playing = true;

	// Load textures
	puyo_texture = LoadTexture("./assets/puyo.png");
}

static void HandleBlockMovement(ppp_puyo::Player& player, const int8 x_move, const int8 y_move) {
	int32 pivot_x = player.pivot_bean.x_pos + x_move;
	int32 pivot_y = player.pivot_bean.y_pos + y_move;

	int32 edge_x = player.edge_bean.x_pos + x_move;
	int32 edge_y = player.edge_bean.y_pos + y_move;

	// X Movement
	if (
		pivot_x > -1 && pivot_x < ppp_puyo::GRID_COLS &&
		edge_x > -1 && edge_x < ppp_puyo::GRID_COLS
	) {
		if (
			IndexGrid(player, pivot_x, player.pivot_bean.y_pos).type == ppp_puyo::BeanType::EMPTY_BEAN &&
			IndexGrid(player, edge_x, player.edge_bean.y_pos).type == ppp_puyo::BeanType::EMPTY_BEAN
		) {
			player.pivot_bean.x_pos = pivot_x;
			player.edge_bean.x_pos = edge_x;
		}
	}

	// Y Movement
	if (pivot_y < ppp_puyo::GRID_ROWS && edge_y < ppp_puyo::GRID_ROWS) {
		if (
			IndexGrid(player, player.pivot_bean.x_pos, pivot_y).type == ppp_puyo::BeanType::EMPTY_BEAN &&
			IndexGrid(player, player.edge_bean.x_pos, edge_y).type == ppp_puyo::BeanType::EMPTY_BEAN
			) {
			player.pivot_bean.y_pos = pivot_y;
			player.edge_bean.y_pos = edge_y;
		}
		else {
			player.can_move = false;
		}
	}
	else {
		player.can_move = false;
	}
}

static bool IsConnected(
	ppp_puyo::Player& player,
	const ppp_puyo::BeanType root_type,
	const int32 gx, const int32 gy
) {
	if (gx < 0 || gy < 0 || gx >= ppp_puyo::GRID_COLS || gy >= ppp_puyo::GRID_ROWS) {
		return false;
	}

	if (IndexGrid(player, gx, gy).type == root_type) {
		return true;
	}

	return false;
}

static void SetConnections(ppp_puyo::Player& player, ppp_puyo::Bean& bean, const int32 gx, const int32 gy) {
	// CONNECTIONS 0 - Right; 1 - Up; 2 - Left; 3 - Down
	bean.connections = (
		(uint8)IsConnected(player, bean.type, gx + 1, gy) |
		((uint8)IsConnected(player, bean.type, gx, gy - 1) << 1) |
		((uint8)IsConnected(player, bean.type, gx - 1, gy) << 2) |
		((uint8)IsConnected(player, bean.type, gx, gy + 1) << 3)
	);
}

static void AddToGrid(ppp_puyo::Player& player, ppp_puyo::Bean& bean) {
	ppp_puyo::Bean& grid_bean = IndexGrid(player, bean.x_pos, bean.y_pos);
	grid_bean.type = bean.type;
	SetConnections(player, grid_bean, bean.x_pos, bean.y_pos);

	bean.x_pos = BEAN_PLACED;
}

static int16 CountRecursive(
	ppp_puyo::Player& player,
	const ppp_puyo::BeanType root_type,
	const int32 gx, const int32 gy,
	std::bitset<ppp_puyo::GRID_SIZE>& visited
) {
	if (gx < 0 || gy < 0 || gx >= ppp_puyo::GRID_COLS || gy >= ppp_puyo::GRID_ROWS) {
		return 0;
	}

	int32 idx = gx + gy * ppp_puyo::GRID_COLS;
	if (visited[idx]) {
		return 0;
	}

	visited[idx] = 1;

	if (IndexGrid(player, gx, gy).type == root_type) {
		return 1 + (
			CountRecursive(player, root_type, gx + 1, gy, visited) +
			CountRecursive(player, root_type, gx - 1, gy, visited) +
			CountRecursive(player, root_type, gx, gy + 1, visited) +
			CountRecursive(player, root_type, gx, gy - 1, visited)
		);
	}

	return 0;
}

static void DestroyRecursive(
	ppp_puyo::Player& player,
	const ppp_puyo::BeanType root_type,
	const int32 gx, const int32 gy,
	std::bitset<ppp_puyo::GRID_SIZE>& visited
) {
	if (gx < 0 || gy < 0 || gx >= ppp_puyo::GRID_COLS || gy >= ppp_puyo::GRID_ROWS) {
		return;
	}

	int32 idx = gx + gy * ppp_puyo::GRID_COLS;
	if (visited[idx]) {
		return;
	}

	visited[idx] = 1;

	ppp_puyo::Bean& bean = IndexGrid(player, gx, gy);
	if (bean.type != root_type) {
		return;
	}
	
	bean.connections = 0;
	bean.type = ppp_puyo::BeanType::EMPTY_BEAN;

	DestroyRecursive(player, root_type, gx + 1, gy, visited);
	DestroyRecursive(player, root_type, gx - 1, gy, visited);
	DestroyRecursive(player, root_type, gx, gy + 1, visited);
	DestroyRecursive(player, root_type, gx, gy - 1, visited);
}

static void DropFloaters(ppp_puyo::Player& player) {
	for (int32 gx = 0; gx < ppp_puyo::GRID_COLS; gx++) {
		for (int32 gy = ppp_puyo::GRID_ROWS - 2; gy > -1; gy--) {
			ppp_puyo::Bean& cur_bean = IndexGrid(player, gx, gy);

			if (cur_bean.type == ppp_puyo::BeanType::EMPTY_BEAN) continue;

			int32 new_gy = gy;
			while (new_gy < ppp_puyo::GRID_ROWS) {
				int32 target = new_gy + 1;

				if (IndexGrid(player, gx, target).type != ppp_puyo::BeanType::EMPTY_BEAN) {
					break;
				}

				new_gy = target;
			}

			if (gy == new_gy) continue;

			ppp_puyo::Bean& new_bean = IndexGrid(player, gx, new_gy);
			new_bean.type = cur_bean.type;
			cur_bean.type = ppp_puyo::BeanType::EMPTY_BEAN;
			cur_bean.connections = 0;
			SetConnections(player, new_bean, gx, new_gy);
		}
	}
}

static void EvaluateGrid(ppp_puyo::Player& player) {
	bool has_destroyed = false;

	for (int32 gx = 0; gx < ppp_puyo::GRID_COLS; gx++) {
		for (int32 gy = 0; gy < ppp_puyo::GRID_ROWS; gy++) {
			ppp_puyo::Bean& root = IndexGrid(player, gx, gy);
			if (root.type == ppp_puyo::BeanType::EMPTY_BEAN) continue;

			// Destroy chain
			std::bitset<ppp_puyo::GRID_SIZE> count_set;
			if (CountRecursive(player, root.type, gx, gy, count_set) > 3) { // Chains of 4 or longer
				std::bitset<ppp_puyo::GRID_SIZE> destroy_set;
				DestroyRecursive(player, root.type, gx, gy, destroy_set);
				has_destroyed = true;
			}

			// Drop floating blocks
			DropFloaters(player);
		}
	}

	if (has_destroyed) {
		EvaluateGrid(player);
		return;
	}

	for (int32 gx = 0; gx < ppp_puyo::GRID_COLS; gx++) {
		for (int32 gy = 0; gy < ppp_puyo::GRID_ROWS; gy++) {
			ppp_puyo::Bean& root = IndexGrid(player, gx, gy);
			if (root.type == ppp_puyo::BeanType::EMPTY_BEAN) continue;

			SetConnections(player, root, gx, gy);
		}
	}
}

static void BeanFalling(ppp_puyo::Player& player, ppp_puyo::Bean& bean) {
	int32 gy = bean.y_pos + 1;

	if (bean.x_pos == BEAN_PLACED) {
		return;
	}

	if (gy < ppp_puyo::GRID_ROWS) {
		if (IndexGrid(player, bean.x_pos, gy).type == ppp_puyo::BeanType::EMPTY_BEAN) {
			bean.y_pos = gy;
		}
		else {
			AddToGrid(player, bean);
		}
	}
	else {
		AddToGrid(player, bean);
	}
}

static void HandleBlockFalling(ppp_puyo::Player& player) {
	if (player.rotation == 0b01) {
		BeanFalling(player, player.edge_bean);
		BeanFalling(player, player.pivot_bean);
	}
	else {
		BeanFalling(player, player.pivot_bean);
		BeanFalling(player, player.edge_bean);
	}

	if (player.pivot_bean.x_pos == BEAN_PLACED && player.edge_bean.x_pos == BEAN_PLACED) {
		GetNewBeans(player);
		EvaluateGrid(player);
		player.can_move = true;
	}
}

static void HandleBlockRotation(ppp_puyo::Player& player) {
	uint8 target_rotation = (player.rotation + 1) % 4;

	int32 gx = player.pivot_bean.x_pos, gy = player.pivot_bean.y_pos;
	switch (target_rotation) {
	case 0:
		gx += 1;
		break;
	case 1:
		gy += 1;
		break;
	case 2:
		gx -= 1;
		break;
	case 3:
		gy -= 1;
		break;
	}

	if (gx < 0 || gy < 0 || gx >= ppp_puyo::GRID_COLS || gy >= ppp_puyo::GRID_ROWS) {
		return;
	}

	if (IndexGrid(player, gx, gy).type == ppp_puyo::BeanType::EMPTY_BEAN) {
		player.edge_bean.x_pos = gx;
		player.edge_bean.y_pos = gy;
		player.rotation = target_rotation;
	}
}

bool ppp_puyo::UpdateGame(const float delta_time) {
	// Update fall timers
	human.fall_timer += delta_time;
	game_tick_time = MAX(game_tick_time - 0.1f * delta_time / 10.0f, 0.1f);

	bool y_input = false;
	if (human.fall_timer >= game_tick_time) {
		y_input = true;
	}

	if (human.can_move) {
		int8 x_input = (
			(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) -
			(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
		);
		y_input = y_input || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN);
		bool spin_input = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);

		// Rest fall timer on input given
		if (y_input) {
			human.fall_timer = 0.0f;
		}

		// Move blocks
		if (spin_input) HandleBlockRotation(human);
		HandleBlockMovement(human, x_input, y_input);
	}
	else if (y_input) {
		human.fall_timer = 0.0f;
		HandleBlockFalling(human);
	}

	return is_playing;
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

		DrawTexturePro(
			puyo_texture,
			{ 0.0f, 0.0f, 32.0f, 32.0f },
		{
			x_origin + (float)bean.x_pos,
			y_origin + (float)bean.y_pos,
			ppp_puyo::BEAN_SIZE, ppp_puyo::BEAN_SIZE },
			{ 0.0f, 0.0f },
			0.0f,
			ppp_puyo::BeanColors[bean.type]
			);
	}

	// Draw moving beans
	DrawTexturePro(
		puyo_texture,
		{ 0.0f, 0.0f, 32.0f, 32.0f },
		{ 
			x_origin + (float)player.pivot_bean.x_pos * ppp_puyo::BEAN_SIZE,
			y_origin + (float)player.pivot_bean.y_pos * ppp_puyo::BEAN_SIZE,
			ppp_puyo::BEAN_SIZE, ppp_puyo::BEAN_SIZE },
		{ 0.0f, 0.0f },
		0.0f,
		ppp_puyo::BeanColors[player.pivot_bean.type]
	);
	DrawTexturePro(
		puyo_texture,
		{ 0.0f, 0.0f, 32.0f, 32.0f },
		{
			x_origin + (float)player.edge_bean.x_pos * ppp_puyo::BEAN_SIZE,
			y_origin + (float)player.edge_bean.y_pos * ppp_puyo::BEAN_SIZE,
			ppp_puyo::BEAN_SIZE, ppp_puyo::BEAN_SIZE },
		{ 0.0f, 0.0f },
		0.0f,
		ppp_puyo::BeanColors[player.edge_bean.type]
	);
}

void ppp_puyo::DrawGame() {
	DrawPlayer(human, ppp_puyo::GRID_OFFSET_X, GRID_OFFSET_Y);
}
