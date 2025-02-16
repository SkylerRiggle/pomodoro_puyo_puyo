#include "clock.h"

#include <raylib/raylib.h>

float max_time, anim_timer = 0.0f, pomo_timer;

void ppp_pomo::Start(const float total_time) {
	max_time = total_time;
	pomo_timer = 0.0f;
}

bool ppp_pomo::Update(const float delta_time) {
	pomo_timer += delta_time;
	anim_timer += delta_time;

	return pomo_timer < max_time;
}

void ppp_pomo::Draw() {
	DrawText("POMODORO PART!", 0, 0, 18, RED);
}
