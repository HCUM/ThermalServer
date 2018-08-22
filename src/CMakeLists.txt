cmake_minimum_required(VERSION 2.8)

PROJECT(ThermalExperimentServer)

OPTION(USE_LIBUSB "use libusb instead of hidraw interface")

# QT precompiler
set(CMAKE_AUTOMOC ON)

#if (APPLE)
#   include_directories(/Developer/ThermalSDK)
#    set(CMAKE_PREFIX_PATH "/Developer/Qt/5.6/clang_64/lib/cmake")
#endif (APPLE)


find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(OpenCV REQUIRED)





#if (OpenCV_FOUND)
#    list(APPEND ThirdParty_LIBS ${OpenCV_LIBS})
#    include_directories(${OpenCV_INCLUDE_DIRS})
#endif (OpenCV_FOUND)

#SET(CMAKE_BUILD_TYPE Debug)



SET(CMAKE_C_FLAGS "-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0")
SET(CMAKE_CXX_FLAGS "-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 ")

SET(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


SET (CMAKE_C_COMPILER             "/usr/bin/clang")
SET (CMAKE_C_FLAGS                "-std=c99")

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++-3.8")
SET (CMAKE_CXX_FLAGS                "-Wall")



INCLUDE_DIRECTORIES(/usr/include/libirimager)  # h files
LINK_DIRECTORIES(/usr/lib)   # .so file


#INCLUDE_DIRECTORIES(/usr/include/opencv2)  # h files
#LINK_DIRECTORIES(/usr/local/lib)   # .so file


INCLUDE_DIRECTORIES( ${OpenCV_LIBS} )
#LINK_DIRECTORIES(/usr/local/lib)
#INCLUDE_DIRECTORIES(/usr/local/lib)


add_library(window mainwindow.cpp)
target_link_libraries (window Qt5::Widgets)
target_link_libraries (window Qt5::Quick)

add_library(stream ThermalWorker.cpp)
target_link_libraries (stream ${OpenCV_LIBS})
target_link_libraries (stream Qt5::Widgets)

#MESSAGE("Load OpenCV Now! ")
#MESSAGE("${OpenCV_LIBS}")
#MESSAGE("OPEN CV IS HERE:")
#MESSAGE("${OpenCV_INCLUDE_DIRS}")

#-I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_calib3d -lopencv_contrib
#-lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc
#-lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo
#ä-lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lrt -lpthread -lm -ldl
LINK_LIBRARIES(irdirectsdk udev GL GLU glut pthread opencv_highgui opencv_legacy opencv_core ${OpenCV_LIBS})
#####################################################################################
# If your applications do not rely on hidraw, use usb-1.0 as additional dependency. #
# This needs for a library linked against usb-1.0. Take care to install the correct #
# Debian package.                                                                   #
#####################################################################################
IF(USE_LIBUSB)
    LINK_LIBRARIES(usb-1.0)
ENDIF(USE_LIBUSB)


ADD_EXECUTABLE(ThermalExperimentServer main.cpp Obvious2D.cpp IRImagerGUI.cpp IRImagerHandler.cpp
         CVImageWidget.cpp ThermalDataServer.cpp HandleThermalDataConnection.cpp StreamWorker.cpp ConnectionHandler.cpp Server.cpp)


target_link_libraries(ThermalExperimentServer stream opencv_core opencv_imgproc opencv_highgui window )
target_link_libraries(ThermalExperimentServer window)


