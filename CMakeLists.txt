CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2 FATAL_ERROR)

project (sudoku)

include(FindGettext)

set(VERSION \"0.0\")
set(PACKAGE \"sudoku\")

if(GETTEXT_FOUND)
set(ENABLE_NLS 1)
else(GETTEXT_FOUND)
set(ENABLE_NLS 0)
endif(GETTEXT_FOUND)

set(LOCALEDIR \"${CMAKE_INSTALL_PREFIX}/share/locale\")

configure_file (
  "${PROJECT_SOURCE_DIR}/include/config.h.cmake"
  "${PROJECT_SOURCE_DIR}/include/config.h"
  )

add_subdirectory(src)
add_subdirectory(po)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

if(CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS -Wall)
endif(CMAKE_COMPILER_IS_GNUCXX)

IF( NOT CMAKE_BUILD_TYPE )
  SET(CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
