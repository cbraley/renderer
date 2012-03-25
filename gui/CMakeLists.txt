CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

PROJECT(QT_TEST)
set(QT_USE_QTOPENGL TRUE)
FIND_PACKAGE(Qt4 REQUIRED)


set(SOURCES src/gui.cpp src/main_gui.cpp src/StatsPanel.cpp src/RenderViewWidget.cpp ${GUI_CORE_SOURCES})
set(HEADERS src/gui.h src/StatsPanel.h src/RenderViewWidget.h) #ONLY THE FILES THAT NEED TO BE PROCSSED BY QT MOC(Meta-object-compiler)
message(STATUS "Sources for gui app are:: ${SOURCES}")


#Invoke MOC
QT4_WRAP_CPP(MOC_HEADER_FILES ${HEADERS})
#All headers inside of the list ${HEADERS} are processed by QT's MOC
#and the output headers reside in HEADERS_MOC
message(${MOC_HEADER_FILES})

set(GUI_BINARY_NAME ../bin/qtgui)

#Now add the executable and link as usual
set ( CMAKE_CXX_FLAGS "-Wall -g")

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
ADD_EXECUTABLE(${GUI_BINARY_NAME} ${SOURCES} ${MOC_HEADER_FILES}) #Note that the MOCed headers go here
TARGET_LINK_LIBRARIES(${GUI_BINARY_NAME} ${QT_LIBRARIES})


