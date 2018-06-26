CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(IRIMAGEROOPEXAMPLE)

OPTION(USE_LIBUSB "use libusb instead of hidraw interface")

# QT precompiler
set(CMAKE_AUTOMOC ON)

if (APPLE)
    include_directories(/Developer/ThermalSDK)
    #    set(CMAKE_PREFIX_PATH "/Developer/Qt/5.6/clang_64/lib/cmake")
endif (APPLE)

set(CMAKE_PREFIX_PATH "/home/pascalknierim/Qt/5.7/gcc_64/lib/cmake")
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_C_FLAGS "-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0")
SET(CMAKE_CXX_FLAGS "-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -std=c++11")

IF (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    SET(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0")
ENDIF (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

INCLUDE_DIRECTORIES(/usr/include/libirimager ../opengl)
LINK_DIRECTORIES(/usr/lib)

add_library(mainwindow mainwindow.cpp)
target_link_libraries (mainwindow Qt5::Widgets)
target_link_libraries (mainwindow Qt5::Quick)


find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    list(APPEND ThirdParty_LIBS ${OpenCV_LIBS})
    include_directories(${OpenCV_INCLUDE_DIRS})
endif (OpenCV_FOUND)

LINK_LIBRARIES(irdirectsdk udev GL GLU glut pthread)
#####################################################################################
# If your applications do not rely on hidraw, use usb-1.0 as additional dependency. #
# This needs for a library linked against usb-1.0. Take care to install the correct #
# Debian package.                                                                   #
#####################################################################################
IF (USE_LIBUSB)
    LINK_LIBRARIES(usb-1.0)
ENDIF (USE_LIBUSB)

ADD_EXECUTABLE(ThermalExperimentServer main.cpp Obvious2D.cpp IRImagerGUI.cpp IRImagerHandler.cpp
        mainwindow.cpp CVImageWidget.cpp ThermalWorker.cpp ThermalDataServer.cpp HandleThermalDataConnection.cpp StreamWorker.cpp)

target_link_libraries(ThermalExperimentServer mainwindow ${OpenCV_LIBS})
