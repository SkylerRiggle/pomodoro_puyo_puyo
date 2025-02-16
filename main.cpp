#include "src/core/app.h"

int main(void) {
	ppp_core::InitApp(1280, 720, "Pomodoro Puyo Puyo - Hacklahoma 2025");
	ppp_core::RunApp();
	ppp_core::CloseApp();
	return 0;
}