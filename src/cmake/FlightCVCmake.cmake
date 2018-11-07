set(FLIGHTCV_SOURCES
        ${PROJECT_SOURCE_DIR}/src/sources/flightcv/flightcv.cpp
        ${PROJECT_SOURCE_DIR}/src/headers/flightcv/flightcv.h
        ${PROJECT_SOURCE_DIR}/src/headers/serial/flight_serial.h
        ${PROJECT_SOURCE_DIR}/src/sources/serial/flight_serial.cpp
        ${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/sample/linux/flight-control/flight_control_sample.cpp
        ${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/sample/linux/common/dji_linux_environment.cpp
        ${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/sample/linux/common/dji_linux_helpers.cpp
        )
add_library(flightcv SHARED ${FLIGHTCV_SOURCES})
target_link_libraries(flightcv
        ${OpenCV_LIBS}
        stdc++
        m
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARY}
        pthread
        realsense2
        boost_system
        boost_thread
        )
