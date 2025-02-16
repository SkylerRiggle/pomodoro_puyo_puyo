#include "app.h"

#include "math.h"
#include "consts.h"
#include "../puyo/game.h"
#include "../pomo/clock.h"
#include <raylib/raylib.h>
#include <stdio.h>

RenderTexture2D render;

void ppp_core::InitApp(const uint32 default_width, const uint32 default_height, const char* title) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(default_width, default_height, title);
	SetWindowMinSize(320, 240);

	render = LoadRenderTexture(ppp_core::GAME_SCREEN_WIDTH, ppp_core::GAME_SCREEN_HEIGHT);
	SetTextureFilter(render.texture, TEXTURE_FILTER_BILINEAR);
}

void ppp_core::RunApp() {
	bool is_pomo = true;
	const float pomo_interval = 5.0f;//1800.0f;
	const int8 max_rounds = 4;
	int8 round_count = 0;

	ppp_pomo::Start(pomo_interval);

	PlayMusicStream(LoadMusicStream("./audio/pomo_track.wav"));

	while (!WindowShouldClose() && round_count < max_rounds) {
		const float delta_time = GetFrameTime();
		const float screen_scale = MIN(
			(float)GetScreenWidth() / ppp_core::GAME_SCREEN_WIDTH_F,
			(float)GetScreenHeight() / ppp_core::GAME_SCREEN_HEIGHT_F
		);
		
		if (is_pomo) {
			if (!ppp_pomo::Update(delta_time)) {
				is_pomo = false;
				ppp_puyo::StartGame();
			}
		}
		else {
			if (!ppp_puyo::UpdateGame(delta_time)) {
				is_pomo = true;
				ppp_pomo::Start(pomo_interval);
				round_count++;
			}
		}

		BeginTextureMode(render);

		ppp_pomo::Draw();
		if (!is_pomo) ppp_puyo::DrawGame();

		EndTextureMode();

		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexturePro(
			render.texture,
			{ 0.0f, 0.0f, (float)render.texture.width, (float)-render.texture.height },
			{
				(GetScreenWidth() - (ppp_core::GAME_SCREEN_WIDTH_F * screen_scale)) * 0.5f,
				(GetScreenHeight() - (ppp_core::GAME_SCREEN_HEIGHT_F * screen_scale)) * 0.5f,
				ppp_core::GAME_SCREEN_WIDTH_F * screen_scale,
				ppp_core::GAME_SCREEN_HEIGHT_F * screen_scale
			},
			{ 0.0f, 0.0f },
			0.0f,
			WHITE
		);
		EndDrawing();
	}
}

void ppp_core::CloseApp() {
	UnloadRenderTexture(render);
	CloseWindow();
}
