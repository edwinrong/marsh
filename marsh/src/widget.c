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

#include <stdlib.h>

#include "helper/log.h"
#include "helper/checks.h"

#include "framebuffer.h"
#include "signalslot.h"
#include "widget_private.h"
#include "widget.h"
#include "widget_tree.h"
#include "widget_event.h"

widget_t * widget_new(widget_t * parent, void * report_instance, void (*report_draw)(void *), void (*report_delete)(void *))
{
	widget_t * obj = (widget_t *)calloc(1, sizeof(struct s_widget));
	MEMORY_ALLOC_CHECK_RETURN(obj, NULL);

	area_clear(&obj->configured_dim);
	area_clear(&obj->absolute_dim);

	obj->creator_instance = report_instance;
	obj->creator_draw = report_draw;
	obj->creator_delete = report_delete;

	obj->click_signal = signal_new();
	obj->press_signal = signal_new();
	obj->release_signal = signal_new();

	widget_tree_register(obj, parent);

	widget_event_init(&obj->event_handler_list);
	widget_event_install_handler(obj, event_code_interaction_click, default_interaction_event_handler);
	widget_event_install_handler(obj, event_code_interaction_release, default_interaction_event_handler);
	widget_event_install_handler(obj, event_code_interaction_press, default_interaction_event_handler);
	widget_event_install_handler(obj, event_code_draw, default_draw_event_handler);
	widget_event_install_handler(obj, event_code_delete, default_delete_event_handler);
	widget_event_install_handler(obj, event_code_refresh_dim, default_refresh_dim_event_handler);

	obj->pressed = false;

	return obj;
}

void widget_delete_instance_only(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	widget_event_deinit(&obj->event_handler_list);
	widget_tree_unregister(obj);

	signal_delete(obj->click_signal);
	signal_delete(obj->press_signal);
	signal_delete(obj->release_signal);

	free(obj);
}

void widget_delete(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	if (obj->creator_delete && obj->creator_instance)
	{
		obj->creator_delete(obj->creator_instance);
	}
	else
	{
		widget_delete_instance_only(obj);
	}
}

/*const*/ area_t * widget_area(widget_t *obj)
{
	PTR_CHECK_RETURN (obj, "widget", NULL);
	return &obj->configured_dim;
}

void widget_refresh_dim(widget_t * obj)
{
	obj->absolute_dim = obj->configured_dim;

	if (widget_parent(obj))
	{
		obj->absolute_dim.x += widget_parent(obj)->absolute_dim.x;
		obj->absolute_dim.y += widget_parent(obj)->absolute_dim.y;

		area_set_intersection(&obj->canvas_dim, &widget_parent(obj)->absolute_dim, &obj->configured_dim);
	}
	else
	{
		area_set_intersection(&obj->canvas_dim, framebuffer_area(), &obj->configured_dim);
	}
}

void widget_set_dim(widget_t *obj, dim_t width, dim_t height)
{
	PTR_CHECK(obj, "widget");

	obj->configured_dim.width = width;
	obj->configured_dim.height = height;
	widget_refresh_dim(obj);
	widget_tree_refresh_dimension(obj);
}

void widget_set_pos(widget_t *obj, dim_t x, dim_t y)
{
	PTR_CHECK(obj, "widget");

	obj->configured_dim.x = x;
	obj->configured_dim.y = y;
	widget_refresh_dim(obj);
	widget_tree_refresh_dimension(obj);
}

void widget_set_area(widget_t *obj, dim_t x, dim_t y, dim_t width, dim_t height)
{
	PTR_CHECK(obj, "widget");

	obj->configured_dim.x = x;
	obj->configured_dim.y = y;
	obj->configured_dim.width = width;
	obj->configured_dim.height = height;
	widget_refresh_dim(obj);
	widget_tree_refresh_dimension(obj);
}

void widget_click(widget_t * obj)
{
	PTR_CHECK(obj, "widget");
	signal_emit(obj->click_signal);
}

void widget_release(widget_t * obj)
{
	bool shall_click = false;
	PTR_CHECK(obj, "widget");

	if (obj->pressed)
	{
		shall_click = true;
		obj->pressed = false;
	}

	signal_emit(obj->release_signal);

	/* TODO: Create advanced click algorithm, do deal with false releases. */
	if (shall_click)
		signal_emit(obj->click_signal);
}

void widget_press(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	obj->pressed = true;

	signal_emit(obj->press_signal);
}

void widget_draw(widget_t * obj)
{
	PTR_CHECK(obj, "widget");

	if (obj->creator_draw && obj->creator_instance)
	{
		obj->creator_draw(obj->creator_instance);
	}
}

signal_t * widget_click_signal(widget_t * obj)
{
	PTR_CHECK_RETURN(obj, "widget", NULL);

	return obj->click_signal;
}

signal_t * widget_release_signal(widget_t * obj)
{
	PTR_CHECK_RETURN(obj, "widget", NULL);

	return obj->release_signal;
}

signal_t * widget_press_signal(widget_t * obj)
{
	PTR_CHECK_RETURN(obj, "widget", NULL);

	return obj->press_signal;
}
