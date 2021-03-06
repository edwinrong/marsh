/*
 *  Copyright (C) 2013 to 2014 by Felipe Lavratti
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the "Software"), to deal in the
 *  Software without restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 *  Software, and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 *  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef TYPES_H_
#define TYPES_H_

#if !defined SYSTEM_UNIT_TEST
#include <malloc.h>
#else
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#endif

extern int (*marshmallow_terminal_output)(const char * str);

#include <stdint.h>
#include <stdbool.h>

typedef int32_t dim_t;
typedef uint16_t pixel_t;

typedef struct s_signal signal_t;
typedef struct s_slot slot_t;
typedef struct s_signal2 signal2_t;
typedef struct s_slot2 slot2_t;
typedef void* slot_arg;
typedef void(*slot_func)(slot_arg);
typedef void(*slot2_func)(slot_arg, size_t, size_t);

typedef struct s_point {
	dim_t x, y;
} point_t;

typedef struct s_area {
	dim_t x, y, width, height;
} area_t;

typedef struct s_canvas canvas_t;
typedef struct s_virtual_canvas virtual_canvas_t;

typedef struct s_my_string my_string_t;

typedef uint32_t color_t;
typedef const struct s_font font_t;
typedef const struct s_bitmap bitmap_t;
typedef struct s_rectangle_instance rectangle_t;
typedef struct s_icon_instance icon_t;
typedef struct s_image_instance image_t;

enum e_text_justification
{
	TEXT_LEFT_JUST,
	TEXT_CENTER_JUST,
	TEXT_RIGHT_JUST
};
typedef struct s_text text_t;

typedef struct s_widget widget_t;
typedef struct s_button_engine button_engine_t;

enum e_event_default_codes
{
	event_code_interaction_press = 1,
	event_code_interaction_release,
	event_code_interaction_click,
	event_code_draw,
	event_code_delete,
};

struct s_interaction_data
{
	point_t interaction_point;
};
typedef struct s_interaction_data interaction_event_data_t;

typedef struct s_event event_t;
typedef int32_t event_code_t;

#endif /* TYPES_H_ */
