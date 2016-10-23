cmake_minimum_required(VERSION 3.0)
project(ipdvr)

set(SOURCES
    src/ipdvr/channeldata.h
    src/ipdvr/config.cpp
    src/ipdvr/fetchjob_curl.cpp
    src/ipdvr/ilistingfetch.h
    src/ipdvr/listingdb.cpp
    src/ipdvr/listingfetch_xmltvse.cpp
    src/ipdvr/listingparse_jsontv.cpp
    src/ipdvr/listingupdater.cpp
    src/ipdvr/main.cpp
    src/ipdvr/programmedata.h
    )

add_executable(ipdvr ${SOURCES})
target_compile_features(ipdvr PRIVATE cxx_auto_type)
target_compile_features(ipdvr PRIVATE cxx_lambdas)
target_include_directories(ipdvr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(ipdvr PRIVATE -DDEBUG)
endif()

# Multi-Threading

find_package(Threads)
target_link_libraries(ipdvr ${CMAKE_THREAD_LIBS_INIT})

# CURL

find_package(CURL)
if(CURL_FOUND)
    target_include_directories(ipdvr PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(ipdvr ${CURL_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find libcurl.")
endif()

# SQLite3

find_path(SQLITE3_INCLUDE_DIR NAMES sqlite3.h)
find_library(SQLITE3_LIBRARY NAMES sqlite3)
if(SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY)
    target_include_directories(ipdvr PRIVATE ${SQLITE3_INCLUDE_DIR})
    target_link_libraries(ipdvr ${SQLITE3_LIBRARY})
else()
    message(FATAL_ERROR "Could not find libsqlite3.")
endif()
