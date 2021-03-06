/* See LICENSE file for copyright and license details. */
#ifndef LIBSKRIFT_H
#define LIBSKRIFT_H 1

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#if defined(__GNUC__)
# define _LIBSKRIFT_GCC_ONLY(...) __VA_ARGS__
#else
# define _LIBSKRIFT_GCC_ONLY(...)
#endif


#define LIBSKRIFT_RENDERING_STRUCT_VERSION 0

#define LIBSKRIFT_NONE 0

typedef struct libskrift_context LIBSKRIFT_CONTEXT;
typedef struct libskrift_font LIBSKRIFT_FONT;
typedef uint_least32_t libskrift_codepoint_t;


enum libskrift_subpixel_order {
	LIBSKRIFT_OTHER, /* LIBSKRIFT_NONE */
	LIBSKRIFT_RGB,
	LIBSKRIFT_BGR,
	LIBSKRIFT_VRGB,
	LIBSKRIFT_VBGR
};

enum libskrift_smoothing {
	LIBSKRIFT_MONOCHROME, /* LIBSKRIFT_NONE */
	LIBSKRIFT_GREYSCALE,
	LIBSKRIFT_SUBPIXEL
};

enum libskrift_hinting {
	LIBSKRIFT_UNHINTED = 0, /* LIBSKRIFT_NONE */
	LIBSKRIFT_SLIGHT   = 25,
	LIBSKRIFT_MEDIUM   = 50,
	LIBSKRIFT_FULL     = 100
};

#define LIBSKRIFT_CORRECT_GAMMA       0x00000001U
#define LIBSKRIFT_REMOVE_GAMMA        0x00000002U
#define LIBSKRIFT_Y_INCREASES_UPWARDS 0x00000004U /* SFT_DOWNWARD_Y otherwise */
#define LIBSKRIFT_FLIP_TEXT           0x00000008U
#define LIBSKRIFT_MIRROR_TEXT         0x00000010U
#define LIBSKRIFT_MIRROR_CHARS        0x00000020U
#define LIBSKRIFT_TRANSPOSE_TEXT      0x00000040U
#define LIBSKRIFT_TRANSPOSE_CHARS     0x00000080U
#define LIBSKRIFT_NO_LIGATURES        0x00000100U
#define LIBSKRIFT_ADVANCE_TO_GRID     0x00000200U
#define LIBSKRIFT_REGRESS_TO_GRID     0x00000400U /* Combine with LIBSKRIFT_ADVANCE_TO_GRID for closest alternative */
#define LIBSKRIFT_USE_SUBPIXEL_GRID   0x00000800U
#define LIBSKRIFT_VERTICAL_TEXT       0x00001000U
#define LIBSKRIFT_AUTOHINTING         0x00002000U /* Use autohinter even if hint information exists */
#define LIBSKRIFT_NO_AUTOHINTING      0x00004000U /* Use autohinter if no hint information exist */
#define LIBSKRIFT_AUTOKERNING         0x00008000U /* Use autokerner even if kerning information exists */
#define LIBSKRIFT_NO_AUTOKERNING      0x00010000U /* Use autokerner if no kerning information exist */

struct libskrift_rendering {
	int struct_version;
	enum libskrift_subpixel_order subpixel_order;
	enum libskrift_smoothing      smoothing;
	enum libskrift_hinting        hinting;
	uint32_t flags;
	int      grid_fineness;
	double   horizontal_dpi;
	double   vertical_dpi;
	double   kerning;
	double   interletter_spacing;
	double   prestroke_transformation_rotation[4];
	double   left_transformation[6];
	double   right_transformation[6];
	double   top_transformation[6];
	double   bottom_transformation[6];
	double   poststroke_transformation_rotation[4];
	double   char_transformation[6];
	double   text_transformation[6];
};

struct libskrift_glyph {
	double   advance;
	int16_t  x;
	int16_t  y;
	uint16_t width;
	uint16_t height;
	size_t   size;
	uint8_t  image[];
};

struct libskrift_saved_grapheme {
	libskrift_codepoint_t cp;
	size_t len;
};


#define LIBSKRIFT_DEFAULT_RENDERING {\
	.struct_version                     = LIBSKRIFT_RENDERING_STRUCT_VERSION,\
	.subpixel_order                     = LIBSKRIFT_NONE,\
	.smoothing                          = LIBSKRIFT_GREYSCALE,\
	.hinting                            = LIBSKRIFT_FULL,\
	.flags                              = 0,\
	.grid_fineness                      = 1,\
	.horizontal_dpi                     = (double)1920 * 254 / 5180,\
	.vertical_dpi                       = (double)1200 * 254 / 3240,\
	.kerning                            = 1,\
	.interletter_spacing                = 0,\
	.prestroke_transformation_rotation  = {1, 0,      0, 1},\
	.left_transformation                = {1, 0, 0,   0, 1, 0},\
	.right_transformation               = {1, 0, 0,   0, 1, 0},\
	.top_transformation                 = {1, 0, 0,   0, 1, 0},\
	.bottom_transformation              = {1, 0, 0,   0, 1, 0},\
	.poststroke_transformation_rotation = {1, 0,      0, 1},\
	.char_transformation                = {1, 0, 0,   0, 1, 0},\
	.text_transformation                = {1, 0, 0,   0, 1, 0},\
}

#define LIBSKRIFT_NO_SAVED_GRAPHEME {0, 0}


_LIBSKRIFT_GCC_ONLY(__attribute__((__const__, __warn_unused_result__)))
inline double
libskrift_calculate_dpi(double pixels, double millimeters)
{
	return pixels * 254 / 10 / millimeters;
}

_LIBSKRIFT_GCC_ONLY(__attribute__((__const__, __warn_unused_result__)))
inline double
libskrift_inches_to_pixels(double inches, const struct libskrift_rendering *rendering)
{
	return inches * rendering->vertical_dpi;
}

_LIBSKRIFT_GCC_ONLY(__attribute__((__const__, __warn_unused_result__)))
inline double
libskrift_millimeters_to_pixels(double millimeters, const struct libskrift_rendering *rendering)
{
	return millimeters * 10 / 254 * rendering->vertical_dpi;
}

_LIBSKRIFT_GCC_ONLY(__attribute__((__const__, __warn_unused_result__)))
inline double
libskrift_points_to_pixels(double points, const struct libskrift_rendering *rendering)
{
	return points / 72 * rendering->vertical_dpi;
}


_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__)))
int libskrift_open_font_file(LIBSKRIFT_FONT **, const char *);

_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__)))
int libskrift_open_font_mem(LIBSKRIFT_FONT **, const void *, size_t);

_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__)))
int libskrift_open_font_fd(LIBSKRIFT_FONT **, int);

_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__)))
int libskrift_open_font(LIBSKRIFT_FONT **, FILE *);

void libskrift_close_font(LIBSKRIFT_FONT *);


_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__(1, 2))))
int libskrift_create_context(LIBSKRIFT_CONTEXT **, LIBSKRIFT_FONT **, size_t, double, const struct libskrift_rendering *, void *);

void libskrift_free_context(LIBSKRIFT_CONTEXT *);

_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__, __returns_nonnull__, __warn_unused_result__, __const__)))
const struct libskrift_rendering *libskrift_get_rendering_settings(LIBSKRIFT_CONTEXT *);


_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__)))
int libskrift_get_grapheme_glyph(LIBSKRIFT_CONTEXT *, libskrift_codepoint_t, double, double, struct libskrift_glyph **);

_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__(1, 2, 6))))
ssize_t libskrift_get_cluster_glyph(LIBSKRIFT_CONTEXT *, const char *, struct libskrift_saved_grapheme *,
                                    double, double, struct libskrift_glyph **);

_LIBSKRIFT_GCC_ONLY(__attribute__((__nonnull__)))
int libskrift_merge_glyphs(LIBSKRIFT_CONTEXT *, struct libskrift_glyph *, struct libskrift_glyph *, struct libskrift_glyph **);

#endif
