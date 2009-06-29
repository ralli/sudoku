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

#ifndef SUDOKUVIEW_HPP
#define SUDOKUVIEW_HPP

#include "gtkmm.h"
#include "sudokumodel.hpp"

class SudokuView: public Gtk::DrawingArea {
private:
    Glib::RefPtr<SudokuModel> model;
public:
    SudokuView(const Glib::RefPtr<SudokuModel> &model);
    void on_realize();
    virtual bool on_expose_event(GdkEventExpose *event);
    virtual bool on_button_release_event(GdkEventButton* event);
    virtual bool on_key_release_event(GdkEventKey* event);
    void on_model_changed_event();
private:
    void draw_border(Cairo::RefPtr<Cairo::Context> &cr) const;

    void draw_bold_grid(Cairo::RefPtr<Cairo::Context> &cr) const;
    void draw_normal_grid(Cairo::RefPtr<Cairo::Context> &cr) const;
    void draw_line(Cairo::RefPtr<Cairo::Context> &cr, double x1, double y1,
            double x2, double y2) const;
    void draw_centered_text(Cairo::RefPtr<Cairo::Context> &cr,
            double x, double y, const std::string &text) const;
    void draw_field_text(Cairo::RefPtr<Cairo::Context> &cr, int x,
            int y, const std::string &text) const;
    void draw_field_texts(Cairo::RefPtr<Cairo::Context> &cr) const;
    void draw_field_choice(Cairo::RefPtr<Cairo::Context> &cr, int x, int y,
            int choice) const;
    void draw_field_choices(Cairo::RefPtr<Cairo::Context> &cr) const;
    void draw_selected_cell(Cairo::RefPtr<Cairo::Context> &cr) const;
    void init_matrix(Cairo::Matrix &m, int width, int height) const;
};

#endif
