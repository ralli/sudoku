bin_PROGRAMS=gsudoku
gsudoku_SOURCES=gsudoku.cpp mainwindow.cpp mainwindow.hpp sudokuview.cpp sudokuview.hpp
AM_CXXFLAGS=-Wall @GTKMM_CFLAGS@
gsudoku_LDADD=@LIBS@ @GTKMM_LIBS@ @LIBINTL@
DEFS=-DLOCALEDIR=\"$(datadir)/locale\" @DEFS@
