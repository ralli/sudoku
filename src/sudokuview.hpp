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

/**
 * The view to display the sudoku
 *
 * Depending upon the settings of the SudokuModel, the view
 * displays the sudoku puzzle itself, the remaining options
 * for each field, singleton cells etc.
 */
class SudokuView: public Gtk::DrawingArea {
private:
	Glib::RefPtr<SudokuModel> model;
public:
	/**
	 * constructor
	 *
	 * @param model the model to display
	 */
	SudokuView(const Glib::RefPtr<SudokuModel> &model);

	/**
	 * The on_realize event handler.
	 *
	 * Will be executed, when the view is displayed the first time.
	 */
	void on_realize();

	/**
	 * The expose event.
	 *
	 * Will be executed to redraw the view.
	 *
	 * @param event Holds information about the area to redraw. May be null.
	 */
	virtual bool on_expose_event(GdkEventExpose *event);

	/**
	 * The mouse up event.
	 *
	 * Will be executed, if the mouse button has been released.
	 *
	 * @param event Holds information about the mouse location.
	 */
	virtual bool on_button_release_event(GdkEventButton* event);

	/**
	 * The key press event.
	 *
	 * @param event Holds information about the key pressed.
	 */
	virtual bool on_key_release_event(GdkEventKey* event);

	/*!
	 * \brief Will be executed to handle changes of the model
	 * state.
	 */
	void on_model_changed_event();
private:
	/*!
	 * \brief Draws the grids border.
	 *
	 * \param cr The Cairo context to draw on.
	 */
	void draw_border(Cairo::RefPtr<Cairo::Context> &cr) const;

	/*!
	 * \brief Draws the grids cells. Draws the bold lines which
	 * separate the houses "3x3 boxes" of the grid.
	 *
	 * \param cr The Cairo context to draw on.
	 */
	void draw_bold_grid(Cairo::RefPtr<Cairo::Context> &cr) const;

	/*!
	 * \brief Draws the grids cells.
	 *
	 * \param cr The Cairo context to draw on.
	 */
	void draw_normal_grid(Cairo::RefPtr<Cairo::Context> &cr) const;

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
	void draw_line(Cairo::RefPtr<Cairo::Context> &cr, double x1, double y1,
			double x2, double y2) const;

	/**
	 * Draws a text centered at a given coordinate.
	 *
	 * @param cr The Cairo context to draw on.
	 * @param x the x coordinate
	 * @param y the y coordinate
	 * @params text the string to be drawn
	 */
	void draw_centered_text(Cairo::RefPtr<Cairo::Context> &cr, double x,
			double y, const std::string &text) const;

	/**
	 * Draws a text at the center of a given field.
	 *
	 * @param cr The Cairo context to draw on.
	 * @param x The column of the field in the range (0..8)
	 * @param y The row of the field in the range (0..8)
	 */
	void draw_field_text(Cairo::RefPtr<Cairo::Context> &cr, int x, int y,
			const std::string &text) const;

	/**
	 * Draws the texts of all fields.
	 *
	 * @param cr The Cairo context to draw on.
	 */
	void draw_field_texts(Cairo::RefPtr<Cairo::Context> &cr) const;

	/**
	 * Draws a choice for a specific field.
	 *
	 * @param cr The Cairo context to draw on.
	 * @param x The column of the field in the range (0..8)
	 * @param y The row of the field in the range (0..8)
	 * @param choice The choice to be drawn in the range (1..9)
	 */
	void draw_field_choice(Cairo::RefPtr<Cairo::Context> &cr, int x, int y,
			int choice) const;

	/**
	 * Draws all remaining choices of all fields.
     *
	 * @param cr The Cairo context to draw on.
	 */
	void draw_field_choices(Cairo::RefPtr<Cairo::Context> &cr) const;

	/**
	 * Draws the selected (currently focused) cell.
	 *
     * @param cr The Cairo context to draw on.
	 */
	void draw_selected_cell(Cairo::RefPtr<Cairo::Context> &cr) const;

	/**
	 * Draws all highlighted cells.
	 *
	 * Highlighted cells are the ones having a specific choice.
	 * This makes it easier for the user to find singletons.
     *
     * @param cr The Cairo context to draw on.
	 */
	void draw_highlighted_cells(Cairo::RefPtr<Cairo::Context> &cr) const;

	/**
	 * Draws one highlighted cell.
	 *
	 * @see draw_highlighted_cells for details.
	 *
     * @param cr The Cairo context to draw on.
     * @idx the index of the cell in the range (0..80)
	 */
	void draw_highlighted_cell(Cairo::RefPtr<Cairo::Context> &cr, int idx) const;

	/**
	 * Initializes the transformation matrix used to display the view.
	 *
	 * The matrix transforms from logical coordinates (0..1) to device coordinates,
	 * with the (0,0)-coordinate in the upper left of the view.
	 *
	 * The view itself is normalized so that the (0,0)-(1,1) square is centered
	 * within the view.
	 *
	 * The matrix may be inversed to map from device coordinates to logical
	 * coordinates. So the cell selected by a mouse click may be found.
	 *
	 * The method uses the cairo transformation matrix methods. So font sizes
	 * and line widths are scaled as well.
	 */
	void init_matrix(Cairo::Matrix &m, int width, int height) const;
};

#endif
