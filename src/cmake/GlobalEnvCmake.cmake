set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(BUILD_WITH_TM2 OFF)
set(BUILD_GRAPHICAL_EXAMPLES ON)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/librealsense-2.16.3/include)
include_directories(${PROJECT_SOURCE_DIR}/src/headers)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/osdk-core/hal/inc)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/osdk-core/protocol/inc)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/osdk-core/utility/inc)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/sample/linux/flight-control)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/sample/linux/common)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/osdk-core/platform/linux/inc)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Onboard-SDK/osdk-core/api/inc)
set(LIBGOOGLETEST gtest gmock)
set(LIBREALSENSE realsense)
