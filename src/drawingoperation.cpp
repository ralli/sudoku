#include "drawingoperation.hpp"
#include "util.hpp"
#include <algorithm>

inline double round(double x) {
    return floor(x + 0.5);
}

void DrawingOperation::draw_line(Cairo::RefPtr<Cairo::Context> &cr, double x1, double y1, double x2, double y2) const {
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

void DrawingOperation::draw_centered_text(Cairo::RefPtr<Cairo::Context> &cr, double x, double y, const std::string &text) const {
    Cairo::TextExtents ex;
    cr->get_text_extents(text, ex);
    double px = x - 0.5 * ex.width;
    double py = y + 0.5 * ex.height;
    cr->move_to(px, py);
    cr->show_text(text);
}

void DrawGridOperation::draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const {
    draw_border(cr);
    draw_bold_grid(cr);
    draw_normal_grid(cr);
}

void DrawGridOperation::draw_border(Cairo::RefPtr<Cairo::Context> &cr) const {
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

void DrawGridOperation::draw_bold_grid(Cairo::RefPtr<Cairo::Context> &cr) const {
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

void DrawGridOperation::draw_normal_grid(Cairo::RefPtr<Cairo::Context> &cr) const {
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

void DrawHighlightedCellsOperation::draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const {
    if (model->get_highlighted_choice() == 0)
        return;

    for (int i = 0; i < 81; ++i) {
        if (!model->has_choice(i, model->get_highlighted_choice())) {
            draw_highlighted_cell(cr, i);
        }
    }
}

void DrawHighlightedCellsOperation::draw_highlighted_cell(Cairo::RefPtr<Cairo::Context> &cr, int idx) const {
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

void DrawFieldTextsOperation::draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const {
    cr->save();
    cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
    cr->set_font_size(1.0 / 9.0 * 0.7);

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

    cr->restore();
}

void DrawFieldTextsOperation::draw_field_text(Cairo::RefPtr<Cairo::Context> &cr, int x, int y, const std::string &text) const {
    double delta = 1.0 / 9.0;
    double px = x * delta + 0.5 * delta;
    double py = y * delta + 0.5 * delta;
    draw_centered_text(cr, px, py, text);
}

void DrawChoicesOperation::draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const {
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

void DrawChoicesOperation::draw_field_choice(Cairo::RefPtr<Cairo::Context> &cr, int x, int y, int choice) const {
    cr->save();
    cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
    cr->set_source_rgb(0.5, 0.5, 0.5);
    cr->set_font_size(1.0 / 9.0 * 0.8 * 0.3);

    double delta = 1.0 / 9.0;
    double choice_delta = delta / 3.0;

    double px = x * delta + ((choice - 1) % 3) * choice_delta + 0.5 * choice_delta;
    double py = y * delta + ((choice - 1) / 3) * choice_delta + 0.5 * choice_delta;
    std::ostringstream os;
    os << choice;
    draw_centered_text(cr, px, py, os.str());

    cr->restore();
}

void DrawSelectedCellOperation::draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const {
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

CompositeDrawingOperation::~CompositeDrawingOperation() {
    std::for_each(operations.begin(), operations.end(), destroy<DrawingOperation *> ());
}

void CompositeDrawingOperation::draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const {
    for (std::vector<DrawingOperation *>::const_iterator i = operations.begin(); i != operations.end(); ++i)
        (*i)->draw(cr, model);
}

void CompositeDrawingOperation::add(DrawingOperation *operation) {
    operations.push_back(operation);
}
