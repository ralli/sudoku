project (sudoku CXX)
cmake_minimum_required(VERSION 2.6)

if(MSVC)
include(FindGTK2)
find_package(GTK2 COMPONENTS gtk gtkmm)
set(GTKMM_FOUND ${GTK2_FOUND})
set(GTKMM_INCLUDE_DIRS ${GTK2_INCLUDE_DIRS})
set(GTKMM_LIBRARIES ${GTK2_LIBRARIES})
else(MSVC)
include(FindPkgConfig)
pkg_check_modules(GTKMM gtkmm-2.4)
endif(MSVC)

set(COMMON_SOURCES
boxlinereduction.cpp 
forcingchain.cpp 
grid.cpp 
hiddendouble.cpp 
hiddentriple.cpp 
hintconsumer.cpp 
hint.cpp 
pointing.cpp 
range.cpp 
singlehint.cpp 
xwing.cpp 
nakeddouble.cpp 
nakedsingle.cpp 
claiming.cpp 
swordfish.cpp 
xywing.cpp 
indirecthint.cpp)

set(SUDOKU_SOURCES 
sudoku.cpp 
${COMMON_SOURCES})

set(DANCING_LINKS_SOURCES
dlxmain.cpp dlxsolver.cpp dancinglinks.cpp grid.cpp range.cpp)

set(GSUDOKU_SOURCES 
${COMMON_SOURCES}
commands.cpp 
dancinglinks.cpp 
dlxsolver.cpp 
gridchecker.cpp 
gsudoku.cpp 
hintview.cpp 
mainwindow.cpp 
statusview.cpp 
sudokugenerator.cpp 
sudokumodel.cpp 
sudokusolver.cpp 
sudokuview.cpp
drawingoperation.cpp
sudokuprintoperation.cpp)

add_executable(sudoku ${SUDOKU_SOURCES})
add_executable(dancinglinks ${DANCING_LINKS_SOURCES})

IF (GTKMM_FOUND) 
  include_directories(${GTKMM_INCLUDE_DIRS})
  add_executable(gsudoku ${GSUDOKU_SOURCES})
  target_link_libraries(gsudoku ${GTKMM_LIBRARIES})
IF (MSVC)
   set_target_properties(gsudoku PROPERTIES 
   LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\""
   COMPILE_FLAGS "/wd4250")
ENDIF(MSVC)
ENDIF(GTKMM_FOUND)
