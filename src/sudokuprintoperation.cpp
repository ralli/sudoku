#include "sudokuprintoperation.hpp"
#include "drawingoperation.hpp"

Glib::RefPtr<SudokuPrintOperation> SudokuPrintOperation::create(const Glib::RefPtr<SudokuModel> &model) {
    return Glib::RefPtr<SudokuPrintOperation>(new SudokuPrintOperation(model));
}

SudokuPrintOperation::SudokuPrintOperation(const Glib::RefPtr<SudokuModel> &model) :
    model(model) {

}

void SudokuPrintOperation::on_begin_print(const Glib::RefPtr<Gtk::PrintContext>& context) {
    set_n_pages(1);
}

void SudokuPrintOperation::on_draw_page(const Glib::RefPtr<Gtk::PrintContext>& context, int page_nr) {
    gdouble width  = context->get_width();
    gdouble height = context->get_height();

    Cairo::Matrix m;
    init_matrix(m, width, height);

    //Get a Cairo Context, which is used as a drawing board:
    Cairo::RefPtr<Cairo::Context> cairo_ctx = context->get_cairo_context();
    cairo_ctx->transform(m);

    CompositeDrawingOperation op;
    op.add(new DrawGridOperation());
    op.add(new DrawFieldTextsOperation());

    op.draw(cairo_ctx, model);
}

void SudokuPrintOperation::init_matrix(Cairo::Matrix &m, gdouble width, gdouble height) const {
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
