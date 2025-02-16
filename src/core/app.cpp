#include "app.h"

#include "math.h"
#include "consts.h"
#include "../puyo/game.h"
#include <raylib/raylib.h>
#include <stdio.h>

RenderTexture2D render;

void ppp_core::InitApp(const uint32 default_width, const uint32 default_height, const char* title) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(default_width, default_height, title);
	SetWindowMinSize(320, 240);

	render = LoadRenderTexture(ppp_core::GAME_SCREEN_WIDTH, ppp_core::GAME_SCREEN_HEIGHT);
	SetTextureFilter(render.texture, TEXTURE_FILTER_POINT);
}

void ppp_core::RunApp() {
	ppp_puyo::StartGame();

	while (!WindowShouldClose()) {
		const float delta_time = GetFrameTime();
		const float screen_scale = MIN(
			(float)GetScreenWidth() / ppp_core::GAME_SCREEN_WIDTH_F,
			(float)GetScreenHeight() / ppp_core::GAME_SCREEN_HEIGHT_F
		);

		ppp_puyo::UpdateGame(delta_time);

		BeginTextureMode(render);
		ClearBackground(SKYBLUE);
		ppp_puyo::DrawGame();
		DrawFPS(0, 0);
		EndTextureMode();

		BeginDrawing();
		ClearBackground(RED);
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
