#pragma once

#define wP 0x1
#define wN 0x2
#define wK 0x3
#define wQ 0x4
#define wB 0x5
#define wR 0x6

#define bP 0x80
#define bN 0x81
#define bK 0x82
#define bQ 0x83
#define bB 0x84
#define bR 0x85

typedef struct {
    uint8_t from;
    uint8_t to;
    uint8_t promote;
} move;