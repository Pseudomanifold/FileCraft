CMAKE_MINIMUM_REQUIRED( VERSION 2.8.11 )
PROJECT( FileCraft )

SET( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic" )

SET( CMAKE_INCLUDE_CURRENT_DIR ON )
SET( CMAKE_AUTOMOC ON )

FIND_PACKAGE( Qt5Core )
FIND_PACKAGE( Qt5Gui  )
FIND_PACKAGE( Qt5OpenGL )

SET( SOURCES
  Chunk.cc
  FileCraft.cc
  MainWindow.cc
  RenderWidget.cc
)

ADD_EXECUTABLE( filecraft ${SOURCES} )

TARGET_LINK_LIBRARIES( filecraft
  Qt5::Core
  Qt5::Gui
  Qt5::OpenGL
)
