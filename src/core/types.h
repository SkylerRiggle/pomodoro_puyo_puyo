#ifndef PPP_CORE_TYPES_H
#define PPP_CORE_TYPES_H

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64;

static_assert(sizeof(int8) == 1, "Int 8 should be 1 byte!");
static_assert(sizeof(int16) == 2, "Int 16 should be 2 bytes!");
static_assert(sizeof(int32) == 4, "Int 16 should be 4 bytes!");
static_assert(sizeof(int64) == 8, "Int 16 should be 8 bytes!");

static_assert(sizeof(uint8) == 1, "UInt 8 should be 1 byte!");
static_assert(sizeof(uint16) == 2, "UInt 16 should be 2 bytes!");
static_assert(sizeof(uint32) == 4, "UInt 16 should be 4 bytes!");
static_assert(sizeof(uint64) == 8, "UInt 16 should be 8 bytes!");

#endif // ! PPP_CORE_TYPES_H