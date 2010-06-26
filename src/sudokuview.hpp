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
	/**
	 * Draws the selected (currently focused) cell.
	 *
     * @param cr The Cairo context to draw on.
	 */
	void draw_selected_cell(Cairo::RefPtr<Cairo::Context> &cr) const;

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
