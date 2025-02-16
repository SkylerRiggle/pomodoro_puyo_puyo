#ifndef PPP_CORE_MATH_H
#define PPP_CORE_MATH_H

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CLAMP(val, min, max) MIN(max, MAX(min, val))

#endif // ! PPP_CORE_MATH_H