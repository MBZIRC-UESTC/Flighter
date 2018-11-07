find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBS})
message(STATUS Boost_Include_Dir:${Boost_INCLUDE_DIRS})
