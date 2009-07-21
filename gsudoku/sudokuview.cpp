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
#include <sstream>
#include <gdk/gdkkeysyms.h>
#include <cmath>

SudokuView::SudokuView(const Glib::RefPtr<SudokuModel> &model) :
    model(model) {
    model->signal_changed().connect(sigc::mem_fun(*this,
            &SudokuView::on_model_changed_event));
    signal_button_press_event().connect(sigc::mem_fun(*this,
            &SudokuView::on_button_release_event));
    signal_key_press_event().connect(sigc::mem_fun(*this,
            &SudokuView::on_key_release_event));
    set_flags(Gtk::CAN_FOCUS);
    set_events(Gdk::EXPOSURE_MASK | Gdk::LEAVE_NOTIFY_MASK
            | Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK
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
        cr->rectangle(event->area.x, event->area.y, event->area.width,
                event->area.height);
        cr->clip();
    }

    Cairo::Matrix m;
    init_matrix(m, width, height);
    cr->transform(m);

    cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL,
            Cairo::FONT_WEIGHT_BOLD);

    cr->set_font_size(1.0 / 9.0 * 0.7);

    draw_highlighted_cells(cr);
    draw_selected_cell(cr);

    draw_border(cr);
    draw_bold_grid(cr);
    draw_normal_grid(cr);
    draw_field_texts(cr);

    if (model->get_show_choices()) {
        cr->save();
        cr->set_source_rgb(0.5, 0.5, 0.5);
        cr->set_font_size(1.0 / 9.0 * 0.8 * 0.3);
        draw_field_choices(cr);
        cr->restore();
    }

    return true;
}

void SudokuView::draw_border(Cairo::RefPtr<Cairo::Context> &cr) const {
    cr->save();
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->set_line_width(0.007);
    cr->set_antialias(Cairo::ANTIALIAS_NONE);
    cr->move_to(0.0, 0.0);
    cr->line_to(1.0, 0.0);
    cr->line_to(1.0, 1.0);
    cr->line_to(0.0, 1.0);
    cr->close_path();
    cr->stroke();
    cr->restore();
}

void SudokuView::draw_bold_grid(Cairo::RefPtr<Cairo::Context> &cr) const {
    double delta = 1.0 / 3.0;

    cr->save();
    cr->set_antialias(Cairo::ANTIALIAS_NONE);
    cr->set_line_width(0.007);
    draw_line(cr, 1 * delta, 0.0, 1 * delta, 1.0);
    draw_line(cr, 2 * delta, 0.0, 2 * delta, 1.0);
    draw_line(cr, 0.0, 1 * delta, 1.0, 1 * delta);
    draw_line(cr, 0.0, 2 * delta, 1.0, 2 * delta);
    cr->restore();
}

void SudokuView::draw_normal_grid(Cairo::RefPtr<Cairo::Context> &cr) const {
    double delta = 1.0 / 9.0;

    cr->save();

    cr->set_line_width(0.003);

    draw_line(cr, 1 * delta, 0.0, 1 * delta, 1.0);
    draw_line(cr, 2 * delta, 0.0, 2 * delta, 1.0);
    draw_line(cr, 4 * delta, 0.0, 4 * delta, 1.0);
    draw_line(cr, 5 * delta, 0.0, 5 * delta, 1.0);
    draw_line(cr, 7 * delta, 0.0, 7 * delta, 1.0);
    draw_line(cr, 8 * delta, 0.0, 8 * delta, 1.0);

    draw_line(cr, 0.0, 1 * delta, 1.0, 1 * delta);
    draw_line(cr, 0.0, 2 * delta, 1.0, 2 * delta);
    draw_line(cr, 0.0, 4 * delta, 1.0, 4 * delta);
    draw_line(cr, 0.0, 5 * delta, 1.0, 5 * delta);
    draw_line(cr, 0.0, 7 * delta, 1.0, 7 * delta);
    draw_line(cr, 0.0, 8 * delta, 1.0, 8 * delta);

    cr->restore();
}

inline double round(double x) {
    return floor(x + 0.5);
}

void SudokuView::draw_line(Cairo::RefPtr<Cairo::Context> &cr, double x1,
        double y1, double x2, double y2) const {
    cr->user_to_device(x1, y1);
    x1 = round(x1);
    y1 = round(y1);
    cr->device_to_user(x1, y1);
    cr->move_to(x1, y1);
    cr->user_to_device(x2, y2);
    x2 = round(x2);
    y2 = round(y2);
    cr->device_to_user(x2, y2);
    cr->line_to(x2, y2);
    cr->stroke();
}

void SudokuView::draw_centered_text(Cairo::RefPtr<Cairo::Context> &cr,
        double x, double y, const std::string &text) const {
    Cairo::TextExtents ex;
    cr->get_text_extents(text, ex);
    double px = x - 0.5 * ex.width;
    double py = y + 0.5 * ex.height;
    cr->move_to(px, py);
    cr->show_text(text);
}

void SudokuView::draw_field_text(Cairo::RefPtr<Cairo::Context> &cr, int x,
        int y, const std::string &text) const {
    double delta = 1.0 / 9.0;
    double px = x * delta + 0.5 * delta;
    double py = y * delta + 0.5 * delta;
    draw_centered_text(cr, px, py, text);
}

void SudokuView::draw_field_texts(Cairo::RefPtr<Cairo::Context> &cr) const {
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            int idx = 9 * y + x;
            if (model->has_value(idx)) {
                std::ostringstream os;
                os << model->get_value(idx);
                draw_field_text(cr, x, y, os.str());
            }
        }
    }
}

void SudokuView::draw_field_choice(Cairo::RefPtr<Cairo::Context> &cr, int x,
        int y, int choice) const {
    double delta = 1.0 / 9.0;
    double choice_delta = delta / 3.0;

    double px = x * delta + ((choice - 1) % 3) * choice_delta + 0.5
            * choice_delta;
    double py = y * delta + ((choice - 1) / 3) * choice_delta + 0.5
            * choice_delta;
    std::ostringstream os;
    os << choice;
    draw_centered_text(cr, px, py, os.str());
}

void SudokuView::draw_field_choices(Cairo::RefPtr<Cairo::Context> &cr) const {
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            int idx = 9 * y + x;
            if (!model->has_value(idx)) {
                for (int choice = 1; choice <= 9; ++choice) {
                    if (model->has_choice(idx, choice))
                        draw_field_choice(cr, x, y, choice);
                }
            }
        }
    }
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
    //    int subrow = static_cast<int> (3 * 9 * y) % 3;
    //    int subcol = static_cast<int> (3 * 9 * x) % 3;
    //    int value = (subrow * 3 + subcol) + 1;
    int idx = row * 9 + col;
    if (idx >= 0 && idx < 81)
        model->set_selected_cell(idx);
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

void SudokuView::draw_selected_cell(Cairo::RefPtr<Cairo::Context> &cr) const {
    int selected_cell = model->get_selected_cell();
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    const gdouble delta = 1.0 / 9.0;
    cr->save();
    cr->move_to(col * delta, row * delta);
    cr->line_to((col + 1) * delta, row * delta);
    cr->line_to((col + 1) * delta, (row + 1) * delta);
    cr->line_to(col * delta, (row + 1) * delta);
    cr->close_path();
    cr->set_source_rgb(0.85, 0.85, 1.0);
    cr->fill();
    cr->restore();
}

void SudokuView::draw_highlighted_cells(Cairo::RefPtr<Cairo::Context> &cr) const {
    if (model->get_highlighted_choice() == 0)
        return;

    for (int i = 0; i < 81; ++i) {
        if (!model->has_choice(i, model->get_highlighted_choice())) {
            draw_highlighted_cell(cr, i);
        }
    }
}

void SudokuView::draw_highlighted_cell(Cairo::RefPtr<Cairo::Context> &cr,
        int idx) const {
    int selected_cell = idx;
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    const gdouble delta = 1.0 / 9.0;
    cr->save();
    cr->move_to(col * delta, row * delta);
    cr->line_to((col + 1) * delta, row * delta);
    cr->line_to((col + 1) * delta, (row + 1) * delta);
    cr->line_to(col * delta, (row + 1) * delta);
    cr->close_path();
    cr->set_source_rgb(1.0, 0.7, 0.7);
    cr->fill();
    cr->restore();
}

void SudokuView::on_model_changed_event() {
    get_window()->invalidate(true);
}
