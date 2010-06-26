#ifndef DRAWINGOPERATION_HPP
#define DRAWINGOPERATION_HPP

#include <vector>
#include <gtkmm.h>

#include "sudokumodel.hpp"

class DrawingOperation {
public:
    DrawingOperation() {
    }

    virtual void draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const = 0;
protected:
    /**
     * Draws a line.
     *
     * The coordinates are given in device independent coordinates (0..1).
     * See init_matrix for details ...
     *
     * @param cr The Cairo context to draw on.
     * @param x1 the first x-coordinate
     * @param y1 the first y-coordinate
     * @param x2 the second x-coordinate
     * @param y2 the second y-coordinate
     */
    void draw_line(Cairo::RefPtr<Cairo::Context> &cr, double x1, double y1, double x2, double y2) const;

    /**
     * Draws a text centered at a given coordinate.
     *
     * @param cr The Cairo context to draw on.
     * @param x the x coordinate
     * @param y the y coordinate
     * @params text the string to be drawn
     */
    void draw_centered_text(Cairo::RefPtr<Cairo::Context> &cr, double x, double y, const std::string &text) const;
private:
    DrawingOperation(const DrawingOperation &other) {
    }

    DrawingOperation &operator =(const DrawingOperation &other) {
        return *this;
    }
};

class DrawGridOperation: public DrawingOperation {
public:
    virtual void draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const;
private:
    /**
     * Draws the grids border.
     *
     * @param cr The Cairo context to draw on.
     */
    void draw_border(Cairo::RefPtr<Cairo::Context> &cr) const;

    /**
     * Draws the grids cells. Draws the bold lines which
     * separate the houses "3x3 boxes" of the grid.
     *
     * @param cr The Cairo context to draw on.
     */
    void draw_bold_grid(Cairo::RefPtr<Cairo::Context> &cr) const;

    /**
     * Draws the grids cells.
     *
     * @param cr The Cairo context to draw on.
     */
    void draw_normal_grid(Cairo::RefPtr<Cairo::Context> &cr) const;
};

class DrawHighlightedCellsOperation: public DrawingOperation {
public:
    virtual void draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const;
private:
    /**
     * Draws one highlighted cell.
     *
     * Highlighted cells are the ones having a specific choice.
     * This makes it easier for the user to find singletons.
     *
     * @param cr The Cairo context to draw on.
     * @idx the index of the cell in the range (0..80)
     */
    void draw_highlighted_cell(Cairo::RefPtr<Cairo::Context> &cr, int idx) const;
};

class DrawFieldTextsOperation: public DrawingOperation {
public:
    virtual void draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const;
private:
    /**
     * Draws a text at the center of a given field.
     *
     * @param cr The Cairo context to draw on.
     * @param x The column of the field in the range (0..8)
     * @param y The row of the field in the range (0..8)
     */
    void draw_field_text(Cairo::RefPtr<Cairo::Context> &cr, int x, int y, const std::string &text) const;
};

class DrawChoicesOperation: public DrawingOperation {
public:
    virtual void draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const;
private:
    /**
     * Draws a choice for a specific field.
     *
     * @param cr The Cairo context to draw on.
     * @param x The column of the field in the range (0..8)
     * @param y The row of the field in the range (0..8)
     * @param choice The choice to be drawn in the range (1..9)
     */
    void draw_field_choice(Cairo::RefPtr<Cairo::Context> &cr, int x, int y, int choice) const;
};

class DrawSelectedCellOperation : public DrawingOperation {
public:
    virtual void draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const;
};

class CompositeDrawingOperation: public DrawingOperation {
private:
    std::vector<DrawingOperation *> operations;
public:
    CompositeDrawingOperation() {
    }

    virtual ~CompositeDrawingOperation();
    virtual void draw(Cairo::RefPtr<Cairo::Context> &cr, const Glib::RefPtr<SudokuModel> &model) const;
    void add(DrawingOperation *operation);
private:
    CompositeDrawingOperation(const CompositeDrawingOperation &other) {

    }

    CompositeDrawingOperation &operator =(const CompositeDrawingOperation &other) {
        return *this;
    }
};
#endif
