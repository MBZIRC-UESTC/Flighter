
#add_executable(fl_flight_control_test
#        ${PROJECT_SOURCE_DIR}/src/sources/unit_test/fl_flight_control_test.cpp
#        )
#target_link_libraries(fl_flight_control_test flightcv djiosdk-core )
add_executable(fl_cv_test
        ${PROJECT_SOURCE_DIR}/src/sources/unit_test/fl_cv_test.cpp
        )
target_link_libraries(fl_cv_test flightcv djiosdk-core )

#add_executable(fl_radar_test
#        ${PROJECT_SOURCE_DIR}/src/sources/unit_test/fl_radar_test.cpp
#        )
#target_link_libraries(fl_radar_test flightcv djiosdk-core)
#
#add_executable(fl_rs_camera_test
#        ${PROJECT_SOURCE_DIR}/src/sources/unit_test/fl_rs_camera_test.cpp
#        )
#target_link_libraries(fl_rs_camera_test flightcv djiosdk-core)
#
#add_executable(fl_darknet_test
#        ${PROJECT_SOURCE_DIR}/src/sources/unit_test/fl_darknet_test.cpp
#        )
#target_link_libraries(fl_darknet_test flightcv djiosdk-core)