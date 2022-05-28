#ifndef EG_COLORS_H
#define EG_COLORS_H

#include <stdint.h>

typedef uint32_t eg_color;

// This list of colors was downloaded from lospec.com/palette-list.
// It is a list of 24 hexadecimal numbers in the format ARGB.
// Near the end of the list, there were too many different shades of green,
// so I had to start making up some names for them.
//
// Palette Name: Vinik24
// Description (from lospec.com):
//   Designed as a soft pastel take on the standard supergameboy palette,
//   expanded into a set of connected ramps of four shades, all sharing the
//   same shadow and the same highlight.
#define EG_COLOR_BLACK ((uint32_t)(0xFF000000))
#define EG_COLOR_GRAY ((uint32_t)(0xFF6f6776))
#define EG_COLOR_LIGHT_GRAY ((uint32_t)(0xFF9a9a97))
#define EG_COLOR_GREEN_GRAY ((uint32_t)(0xFFc5ccb8))
#define EG_COLOR_MAUVE ((uint32_t)(0xFF8b5580))
#define EG_COLOR_PINK ((uint32_t)(0xFFc38890))
#define EG_COLOR_LAVENDAR ((uint32_t)(0xFFa593a5))
#define EG_COLOR_INDIGO ((uint32_t)(0xFF666092))
#define EG_COLOR_RED ((uint32_t)(0xFF9a4f50))
#define EG_COLOR_ORANGE ((uint32_t)(0xFFc28d75))
#define EG_COLOR_LIGHT_BLUE ((uint32_t)(0xFF7ca1c0))
#define EG_COLOR_BLUE ((uint32_t)(0xFF416aa3))
#define EG_COLOR_BROWN ((uint32_t)(0xFF8d6268))
#define EG_COLOR_YELLOW ((uint32_t)(0xFFbe955c))
#define EG_COLOR_CYAN ((uint32_t)(0xFF68aca9))
#define EG_COLOR_TEAL ((uint32_t)(0xFF387080))
#define EG_COLOR_OLIVE ((uint32_t)(0xFF6e6962))
#define EG_COLOR_CHARTEUSE ((uint32_t)(0xFF93a167))
#define EG_COLOR_SEA_GREEN ((uint32_t)(0xFF6eaa78))
#define EG_COLOR_FOREST_GREEN ((uint32_t)(0xFF557064))
#define EG_COLOR_MILITARY_GREEN ((uint32_t)(0xFF9d9f7f))
#define EG_COLOR_GREEMP ((uint32_t)(0xFF7e9e99))
#define EG_COLOR_GROOMP ((uint32_t)(0xFF5d6872))
#define EG_COLOR_PURPLE ((uint32_t)(0xFF433455))

// Other colors
#define EG_COLOR_CORNFLOWER_BLUE ((uint32_t)(0xFF6495ED))

#endif