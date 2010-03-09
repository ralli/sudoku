project (sudoku CXX)
cmake_minimum_required(VERSION 2.6)

include(FindGettext)
FIND_PACKAGE( Gettext )
GETTEXT_CREATE_TRANSLATIONS ( sudoku.pot ALL de.po )
