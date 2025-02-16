#include "clock.h"

#include <raylib/raylib.h>
#include "../core/consts.h"
#include <stdio.h>

float max_time, anim_timer = 0.0f, walk_timer = 0.0f, pomo_timer;

Texture2D bush, reptile, octo, woods;

static void LoadTextures() {
	bush = LoadTexture("./assets/bush.png");
	reptile = LoadTexture("./assets/reptile.png");
	octo = LoadTexture("./assets/walk_octocat.png");
	woods = LoadTexture("./assets/woods.png");
}

void ppp_pomo::Start(const float total_time) {
	max_time = total_time;
	pomo_timer = 0.0f;

	LoadTextures();
}

bool ppp_pomo::Update(const float delta_time) {
	pomo_timer += delta_time;
	anim_timer += delta_time;
	walk_timer += delta_time;

	if (anim_timer > 10.0f) {
		anim_timer -= 10.0f;
	}

	if (walk_timer > 1.0f) {
		walk_timer -= 1.0f;
	}

	printf("Time Elapsed: %.2f / %.2f\n", pomo_timer, max_time);

	return pomo_timer < max_time;
}

static float Lerp(float val_a, float val_b, float t) {
	return val_a + (val_b - val_a) * t;
}

void ppp_pomo::Draw() {
	// Draw Woods
	DrawTexturePro(
		woods,
		{ 0.0f, 0.0f, 3000.0f, 2000.0f },
		{
			Lerp(0.0f, -ppp_core::GAME_SCREEN_WIDTH_F, anim_timer / 10.0f),
			0.0f,
			ppp_core::GAME_SCREEN_WIDTH_F, ppp_core::GAME_SCREEN_HEIGHT_F },
		{ 0.0f, 0.0f },
		0.0f,
		WHITE
	);
	DrawTexturePro(
		woods,
		{ 0.0f, 0.0f, 3000.0f, 2000.0f },
		{
			Lerp(ppp_core::GAME_SCREEN_WIDTH_F, 0.0f, anim_timer / 10.0f),
			0.0f,
			ppp_core::GAME_SCREEN_WIDTH_F, ppp_core::GAME_SCREEN_HEIGHT_F },
		{ 0.0f, 0.0f },
		0.0f,
		WHITE
	);

	// Draw reptile
	DrawTexturePro(
		reptile,
		{ 0.0f, 0.0f, 800.0f, 800.0f },
		{
			Lerp(2000.0f, ppp_core::GAME_SCREEN_WIDTH_F - 100.0f, pomo_timer / max_time),
			300.0f,
			100.0f, 100.0f },
		{ 0.0f, 0.0f },
		0.0f,
		WHITE
	);

	// Draw octocat
	float x_idx = (
		(walk_timer >= 0.0f && walk_timer < 1.0f / 6.0f) || (walk_timer >= 0.5f && walk_timer < 2.0f / 3.0f) ? 0.0f : (
			(walk_timer >= 1.0f / 3.0 && walk_timer < 0.5f) || (walk_timer >= 2.0f / 3.0f && walk_timer < 5.0f / 6.0f) ? 1000.0f :
			2000.0f
		)
	);
	float y_idx = (walk_timer > 0.5f) ? 1000.0f : 0.0f;
	DrawTexturePro(
		octo,
		{ x_idx, y_idx, 1000.0f, 1000.0f },
		{
			100.0f, 300.0f,
			120.0f, 120.0f },
		{ 0.0f, 0.0f },
		0.0f,
		WHITE
	);
}
