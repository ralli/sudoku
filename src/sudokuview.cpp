/*
 * Copyright (c) 2009, Ralph Juhnke
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following conditions
 * are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *    * Neither the name of "Ralph Juhnke" nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sudokuview.hpp"
#include "sudokumodel.hpp"
#include "drawingoperation.hpp"
#include <sstream>
#include <gdk/gdkkeysyms.h>
#include <cmath>

SudokuView::SudokuView(const Glib::RefPtr<SudokuModel> &model) :
    model(model) {
    model->signal_changed().connect(sigc::mem_fun(*this, &SudokuView::on_model_changed_event));
    signal_button_press_event().connect(sigc::mem_fun(*this, &SudokuView::on_button_release_event));
    signal_key_press_event().connect(sigc::mem_fun(*this, &SudokuView::on_key_release_event));
    set_flags(Gtk::CAN_FOCUS);
    set_events(Gdk::EXPOSURE_MASK | Gdk::LEAVE_NOTIFY_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK
            | Gdk::POINTER_MOTION_HINT_MASK | Gdk::KEY_PRESS_MASK);
}

void SudokuView::on_realize() {
    Gtk::DrawingArea::on_realize();

    Glib::RefPtr<Gdk::Window> window = get_window();
    window->set_background(get_style()->get_white());
    grab_focus();
}

bool SudokuView::on_expose_event(GdkEventExpose *event) {
    Glib::RefPtr<Gdk::Window> window = get_window();

    if (!window)
        return true;

    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

    if (event) {
        // clip to the area indicated by the expose event so that we only
        // redraw the portion of the window that needs to be redrawn
        cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
        cr->clip();
    }

    Cairo::Matrix m;
    init_matrix(m, width, height);
    cr->transform(m);

    CompositeDrawingOperation operation;
    operation.add(new DrawHighlightedCellsOperation());
    operation.add(new DrawSelectedCellOperation());
    operation.add(new DrawGridOperation());
    operation.add(new DrawFieldTextsOperation());


    if (model->get_show_choices()) {
        operation.add(new DrawChoicesOperation());
    }

    operation.draw(cr, model);

    return true;
}

bool SudokuView::on_button_release_event(GdkEventButton* event) {
    Glib::RefPtr<Gdk::Window> window = get_window();
    if (!window)
        return true;
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

    Cairo::Matrix m;
    init_matrix(m, width, height);
    cr->transform(m);
    gdouble x = event->x;
    gdouble y = event->y;
    cr->device_to_user(x, y);
    int row = static_cast<int> (9 * y);
    int col = static_cast<int> (9 * x);
    int idx = row * 9 + col;
    if (idx >= 0 && idx < 81)
        model->set_selected_cell(idx);
    grab_focus();
    return true;
}

void SudokuView::init_matrix(Cairo::Matrix &m, int width, int height) const {
    int scale = width < height ? width : height;
    Cairo::Matrix helper;
    cairo_matrix_init_identity(&m);
    cairo_matrix_init_translate(&helper, -0.5, -0.5);
    cairo_matrix_multiply(&m, &m, &helper);
    cairo_matrix_init_scale(&helper, scale * 0.9, scale * 0.9);
    cairo_matrix_multiply(&m, &m, &helper);
    cairo_matrix_init_translate(&helper, width * 0.5, height * 0.5);
    cairo_matrix_multiply(&m, &m, &helper);
}

bool SudokuView::on_key_release_event(GdkEventKey* event) {
    if (event->type == static_cast<guint> (Gdk::KEY_RELEASE))
        return false;
    bool handled = true;
    switch (event->keyval) {
    case GDK_Left:
    case GDK_KP_Left:
        model->move_selection_left();
        break;
    case GDK_Right:
    case GDK_KP_Right:
        model->move_selection_right();
        break;
    case GDK_Up:
    case GDK_KP_Up:
        model->move_selection_up();
        break;
    case GDK_Down:
    case GDK_KP_Down:
        model->move_selection_down();
        break;
    case GDK_1:
    case GDK_2:
    case GDK_3:
    case GDK_4:
    case GDK_5:
    case GDK_6:
    case GDK_7:
    case GDK_8:
    case GDK_9:
        if (event->state & GDK_CONTROL_MASK) {
            model->toggle_current_cell_choice(event->keyval - GDK_1 + 1);
        } else {
            model->set_current_cell_value(event->keyval - GDK_1 + 1);
        }
        break;
    case GDK_Delete:
        model->clear_current_cell_value();
        break;
    default:
        handled = false;
    }
    return handled;
}

void SudokuView::on_model_changed_event() {
    get_window()->invalidate(true);
}
