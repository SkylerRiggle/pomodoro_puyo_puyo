#include <stdio.h>
#include <raylib/raylib.h>

int main(void) {
	InitWindow(400, 400, "Test Window!");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(0, 0);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}