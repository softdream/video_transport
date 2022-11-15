CMAKE_MINIMUM_REQUIRED(VERSION 3.16.3)

PROJECT(video_transport)

ADD_DEFINITIONS( -std=c++14 )

# find opencv4
FIND_PACKAGE( OpenCV REQUIRED )
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LINK_DIRECTORIES(${OpenCV_LIBRARY_DIRS})

# source file headers
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

# source file
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src test_src)

# generate execuate file
ADD_EXECUTABLE( video_transport_test ${test_src} )

TARGET_LINK_LIBRARIES(video_transport_test ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(video_transport_test x264)



