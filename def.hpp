#pragma once

// todo: lowk delete this header file...

#define wP 0x1
#define wN 0x2
#define wK 0x3
#define wQ 0x4
#define wB 0x5
#define wR 0x6

#define bP 0x7
#define bN 0x8
#define bK 0x9
#define bQ 0xa
#define bB 0xb
#define bR 0xc

// todo: change later
typedef struct {
    uint8_t from;
    uint8_t to;
    uint8_t promote;
} move;