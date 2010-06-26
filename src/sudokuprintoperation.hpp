#ifndef SUDOKUPRINTOPERATION_HPP
#define SUDOKUPRINTOPERATION_HPP

#include <gtkmm.h>
#include "sudokumodel.hpp"

class SudokuPrintOperation: public Gtk::PrintOperation {
private:
    const Glib::RefPtr<SudokuModel> model;
public:
    static Glib::RefPtr<SudokuPrintOperation> create(const Glib::RefPtr<SudokuModel> &model);
protected:
    SudokuPrintOperation(const Glib::RefPtr<SudokuModel> &model);

    virtual void on_begin_print(const Glib::RefPtr<Gtk::PrintContext>& context);
    virtual void on_draw_page(const Glib::RefPtr<Gtk::PrintContext>& context, int page_nr);
private:
    void init_matrix(Cairo::Matrix &m, gdouble width, gdouble height) const;
private:
    SudokuPrintOperation(const SudokuPrintOperation &other) {
    }

    SudokuPrintOperation &operator =(const SudokuPrintOperation &other) {
        return *this;
    }
};

#endif
