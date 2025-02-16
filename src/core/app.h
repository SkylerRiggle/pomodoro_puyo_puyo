#ifndef PPP_CORE_APP_H
#define PPP_CORE_APP_H

#include "types.h"

namespace ppp_core {
	void InitApp(const uint32 default_width, const uint32 default_height, const char* title);
	void RunApp();
	void CloseApp();
}

#endif // ! PPP_CORE_APP_H