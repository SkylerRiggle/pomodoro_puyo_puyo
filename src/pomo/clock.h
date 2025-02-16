#ifndef PPP_POMO_CLOCK_H
#define PPP_POMO_CLOCK_H

namespace ppp_pomo {
	void Start(const float total_time);
	bool Update(const float delta_time);
	void Draw();
}

#endif // ! PPP_POMO_CLOCK_H