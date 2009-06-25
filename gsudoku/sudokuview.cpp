#include "sudokuview.hpp"
#include <sstream>

void SudokuView::on_realize() {
    Gtk::DrawingArea::on_realize();

    Glib::RefPtr<Gdk::Window> window = get_window();
    window->set_background(get_style()->get_white());
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

    int scale = width < height ? width : height;

    Cairo::Matrix m;
    Cairo::Matrix helper;
    cairo_matrix_init_identity(&m);
    cairo_matrix_init_translate(&helper, -0.5, -0.5);
    cairo_matrix_multiply(&m, &m, &helper);
    cairo_matrix_init_scale(&helper, scale * 0.9, scale * 0.9);
    cairo_matrix_multiply(&m, &m, &helper);
    cairo_matrix_init_translate(&helper, width * 0.5, height * 0.5);
    cairo_matrix_multiply(&m, &m, &helper);
    cr->transform(m);

        cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL,
                Cairo::FONT_WEIGHT_BOLD);

    cr->set_font_size(1.0 / 9.0 * 0.7);

    draw_border(cr);
    draw_bold_grid(cr);
    draw_normal_grid(cr);
    // draw_field_texts(cr);

    cr->set_source_rgb(0.5, 0.5, 0.5);
    cr->set_font_size(1.0 / 9.0 * 0.8 * 0.3);
    draw_field_choices(cr);

    return true;
}

void SudokuView::draw_border(Cairo::RefPtr<Cairo::Context> &cr) const {
    cr->save();
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->set_line_width(0.007);
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
    cr->set_line_width(0.004);
    draw_line(cr, 1 * delta, 0.0, 1 * delta, 1.0);
    draw_line(cr, 2 * delta, 0.0, 2 * delta, 1.0);
    draw_line(cr, 0.0, 1 * delta, 1.0, 1 * delta);
    draw_line(cr, 0.0, 2 * delta, 1.0, 2 * delta);
    cr->restore();
}

void SudokuView::draw_normal_grid(Cairo::RefPtr<Cairo::Context> &cr) const {
    double delta = 1.0 / 9.0;

    cr->save();
    cr->set_line_width(0.001);
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

void SudokuView::draw_line(Cairo::RefPtr<Cairo::Context> &cr, double x1,
        double y1, double x2, double y2) const {
    cr->move_to(x1, y1);
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
            std::ostringstream os;
            os << x + 1;
            draw_field_text(cr, x, y, os.str());
        }
    }
}

void SudokuView::draw_field_choice(Cairo::RefPtr<Cairo::Context> &cr, int x,
        int y, int choice) const {
    double delta = 1.0 / 9.0;
    double choice_delta = delta / 3.0;

    double px = x * delta + ((choice - 1) % 3) * choice_delta + 0.5 * choice_delta;
    double py = y * delta + ((choice - 1) / 3) * choice_delta + 0.5 * choice_delta;
    std::ostringstream os;
    os << choice;
    draw_centered_text(cr, px, py, os.str());
}

void SudokuView::draw_field_choices(Cairo::RefPtr<Cairo::Context> &cr) const {
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            for (int choice = 1; choice <= 9; ++choice) {
                draw_field_choice(cr, x, y, choice);
            }
        }
    }
}
