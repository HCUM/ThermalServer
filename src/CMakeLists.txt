#cmake_minimum_required(VERSION 3.5)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(SOURCE_FILES main.cpp)
#add_executable(ThermalVisualization ${SOURCE_FILES})





CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

project(ThermalVisualization)
SET(IMAGER_VERSION_MAJOR 0)
SET(IMAGER_VERSION_MINOR 1)


set(CMAKE_PREFIX_PATH "/home/pascalknierim/Qt/5.7/gcc_64/lib/cmake")

if (APPLE)
    include_directories(/Developer/Qt/5.6/clang_64)
    set(CMAKE_PREFIX_PATH "/Developer/Qt/5.6/clang_64/lib/cmake")
endif (APPLE)

find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
    list( APPEND ThirdParty_LIBS ${OpenCV_LIBS} )
    include_directories( ${OpenCV_INCLUDE_DIRS} )
endif( OpenCV_FOUND )



#new changes for qt
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

find_package(Qt5Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(mainwindow mainwindow.cpp)
target_link_libraries (mainwindow Qt5::Widgets)
target_link_libraries (mainwindow Qt5::Quick)


SET(CMAKE_CXX_FLAGS "-Wno-write-strings -fpermissive -malign-double -pthread")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../linux ${PROJECT_SOURCE_DIR}/../include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib)

find_package(Qt5Widgets)
LINK_LIBRARIES(PIImager ImageProcessing udev glut GLU GL pthread)
ADD_EXECUTABLE(ThermalVisualizer
        Obvious2D.cpp
        main.cpp
        CVImageWidget.cpp CVImageWidget.h Server.cpp Server.h ConnectionHandler.cpp ConnectionHandler.h worker.cpp worker.h ThermalDataServer.cpp ThermalDataServer.h ThermalExperiment.cpp ThermalExperiment.h HandleThermalDataConnection.cpp HandleThermalDataConnection.h)
target_link_libraries (ThermalVisualizer mainwindow ${OpenCV_LIBS})
#target_link_libraries(ThermalVisualizer ${OpenCV_LIBS} )


#ADD_EXECUTABLE(imager_serialize
#			mainMinimalistic.cpp
#)

#ADD_EXECUTABLE(read_snapshot
#			Obvious2D.cpp
#			readSnapshot.cpp
#)

