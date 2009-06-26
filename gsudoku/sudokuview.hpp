#ifndef SUDOKUVIEW_HPP
#define SUDOKUVIEW_HPP

#include "gtkmm.h"

class SudokuView: public Gtk::DrawingArea {
public:
    SudokuView();
    void on_realize();
    virtual bool on_expose_event(GdkEventExpose *event);
    virtual bool on_button_release_event(GdkEventButton* event);
    virtual bool on_key_release_event(GdkEventKey* event);
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
    void init_matrix(Cairo::Matrix &m, int width, int height) const;
};

#endif
