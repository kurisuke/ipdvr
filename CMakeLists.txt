CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ipdvr)

SET(SOURCES
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

ADD_EXECUTABLE(ipdvr ${SOURCES})
TARGET_COMPILE_FEATURES(ipdvr PRIVATE cxx_auto_type)
TARGET_COMPILE_FEATURES(ipdvr PRIVATE cxx_lambdas)


IF(CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_COMPILE_DEFINITIONS(ipdvr PRIVATE -DDEBUG)
ENDIF()

# Multi-Threading

find_package(Threads)
target_link_libraries(ipdvr ${CMAKE_THREAD_LIBS_INIT})

# CURL

FIND_PACKAGE(CURL)
IF(CURL_FOUND)
    INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(ipdvr ${CURL_LIBRARIES})
ELSE()
    MESSAGE(FATAL_ERROR "Could not find libcurl.")
ENDIF()

# SQLite3
FIND_PATH(SQLITE3_INCLUDE_DIR NAMES sqlite3.h)
FIND_LIBRARY(SQLITE3_LIBRARY NAMES sqlite3)
IF(SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY)
    INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(ipdvr ${SQLITE3_LIBRARY})
ELSE()
    MESSAGE(FATAL_ERROR "Could not find libsqlite3.")
ENDIF()
