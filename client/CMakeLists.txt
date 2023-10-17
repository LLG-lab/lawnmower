cmake_minimum_required(VERSION 3.1.1)
project(lawnmowerc)

#
# The version number.
#

execute_process(COMMAND ${CMAKE_CURRENT_LIST_DIR}/../etc/version.sh "--major" OUTPUT_VARIABLE lawnmowerc_VERSION_MAJOR)
execute_process(COMMAND ${CMAKE_CURRENT_LIST_DIR}/../etc/version.sh "--minor" OUTPUT_VARIABLE lawnmowerc_VERSION_MINOR)

#
# Set ‘make install’ destination dir.
#

# XXX Ciekawe info na temat robienia instalacji
# https://cmake.org/cmake/help/latest/command/install.html
set(CMAKE_INSTALL_PREFIX /tmp)

#
# Configure a header file to pass some of
# the CMake settings to the source code.
#

configure_file(
  "${PROJECT_SOURCE_DIR}/lawnmowerc-config.h.in"
  "${PROJECT_SOURCE_DIR}/lawnmowerc-config.h"
)

#
# Header .h and .hpp.
#

list(APPEND HEADERS
     ${PROJECT_SOURCE_DIR}/lawnmowerc-config.h
     ${PROJECT_SOURCE_DIR}/include/lawnmowerc_settings.hpp
     ${PROJECT_SOURCE_DIR}/include/ssl_connection.hpp
     ${PROJECT_SOURCE_DIR}/include/pixel.hpp
     ${PROJECT_SOURCE_DIR}/include/fonts.hpp
     ${PROJECT_SOURCE_DIR}/include/default_screen.hpp
     ${PROJECT_SOURCE_DIR}/include/screen_buffer.hpp
     ${PROJECT_SOURCE_DIR}/include/camera.hpp
     ${PROJECT_SOURCE_DIR}/include/output_terminal.hpp
     ${PROJECT_SOURCE_DIR}/include/indications.hpp
     ${PROJECT_SOURCE_DIR}/include/client.hpp
     ${PROJECT_SOURCE_DIR}/../common/include/aux_functions.hpp
)

#
# Sources .c and .cpp.
#

list(APPEND SOURCES
     ${PROJECT_SOURCE_DIR}/main.cpp
     ${PROJECT_SOURCE_DIR}/lawnmowerc_settings.cpp
     ${PROJECT_SOURCE_DIR}/ssl_connection.cpp
     ${PROJECT_SOURCE_DIR}/fonts.cpp
     ${PROJECT_SOURCE_DIR}/default_screen.cpp
     ${PROJECT_SOURCE_DIR}/screen_buffer.cpp
     ${PROJECT_SOURCE_DIR}/camera.cpp
     ${PROJECT_SOURCE_DIR}/output_terminal.cpp
     ${PROJECT_SOURCE_DIR}/indications.cpp
     ${PROJECT_SOURCE_DIR}/client.cpp
     ${PROJECT_SOURCE_DIR}/../common/aux_functions.cpp
     ${PROJECT_SOURCE_DIR}/../3rd-party/easylogging++/easylogging++.cc
)

#
# Setup nonstandard packages cmake script directory.
#

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


find_package(Protobuf REQUIRED)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
    ../common/lawnmower.proto
)
add_executable(lawnmowerc ${SOURCES} ${PROTO_SRCS} ${HEADERS} ${PROTO_HDRS})
install(TARGETS lawnmowerc DESTINATION ${CMAKE_INSTALL_PREFIX})
target_compile_features(lawnmowerc PRIVATE cxx_range_for)

get_target_property(TEMP lawnmowerc COMPILE_FLAGS)
if(TEMP STREQUAL "TEMP-NOTFOUND")
    SET(TEMP "") # set to empty string
else()
    SET(TEMP "${TEMP} ")
endif()

SET(TEMP "${TEMP} -DBOOST_SYSTEM_NO_DEPRECATED" )
SET(TEMP "${TEMP} -DBOOST_ERROR_CODE_HEADER_ONLY" )
SET(TEMP "${TEMP} -DBOOST_BIND_GLOBAL_PLACEHOLDERS" )

SET(TEMP "${TEMP} -DELPP_THREAD_SAFE" )
SET(TEMP "${TEMP} -DELPP_STL_LOGGING" )
SET(TEMP "${TEMP} -DELPP_BOOST_LOGGING" )
SET(TEMP "${TEMP} -DELPP_FEATURE_CRASH_LOG" )
SET(TEMP "${TEMP} -DELPP_FEATURE_ALL" )
SET(TEMP "${TEMP} -DELPP_NO_DEFAULT_LOG_FILE" )

set_target_properties(lawnmowerc PROPERTIES COMPILE_FLAGS ${TEMP} )


#
# Setup nonstandard packages cmake script directory.
#

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#
# Check for dependences: find cURL library (Required).
#

#find_package(CURL REQUIRED)
#include_directories(${CURL_INCLUDE_DIRS})
#target_link_libraries(lawnmowerc ${CURL_LIBRARIES})

#
# Check for dependences: pthread (Required).
#

include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(lawnmowerc ${Protobuf_LIBRARIES})
find_package (Threads)
target_link_libraries (lawnmowerc ${CMAKE_THREAD_LIBS_INIT})

SET(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(lawnmowerc ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(lawnmowerc ${GLEW_LIBRARIES})

find_package(GLFW3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})
target_link_libraries(lawnmowerc ${GLFW3_LIBRARIES})
#target_link_libraries (lawnmowerc glfw)


find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(lawnmowerc ${OPENSSL_LIBRARIES})

find_package(JPEGTURBO REQUIRED)
include_directories(${JPEGTURBO_INCLUDE_DIR})
#target_link_libraries(lawnmowerc ${JPEGTURBO_LIBRARIES})
target_link_libraries(lawnmowerc turbojpeg)

target_link_libraries (lawnmowerc ${CMAKE_DL_LIBS})

#
# Static linking boost libraries.
# Linking order is important!
#

set(BOOST_LIBS_PATH "${PROJECT_SOURCE_DIR}/../3rd-party/boost_current_release/stage/lib/")
target_link_libraries(lawnmowerc "${BOOST_LIBS_PATH}/libboost_atomic.a")
target_link_libraries(lawnmowerc "${BOOST_LIBS_PATH}/libboost_chrono.a")
target_link_libraries(lawnmowerc "${BOOST_LIBS_PATH}/libboost_date_time.a")
target_link_libraries(lawnmowerc "${BOOST_LIBS_PATH}/libboost_system.a")
target_link_libraries(lawnmowerc "${BOOST_LIBS_PATH}/libboost_thread.a")
target_link_libraries(lawnmowerc "${BOOST_LIBS_PATH}/libboost_filesystem.a")
target_link_libraries(lawnmowerc "${BOOST_LIBS_PATH}/libboost_program_options.a")

#
# Additional include directory.
#

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/../common/include")
include_directories("${PROJECT_SOURCE_DIR}/../3rd-party")
include_directories("${PROJECT_SOURCE_DIR}/../3rd-party/boost")
include_directories("${PROJECT_SOURCE_DIR}/../3rd-party/glad")
include_directories("${PROJECT_SOURCE_DIR}/../3rd-party/easylogging++")
