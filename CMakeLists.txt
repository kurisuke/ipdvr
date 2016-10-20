CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ipdvr)

SET(SOURCES
    channeldata.h
    config.cpp
    fetchjob_curl.cpp
    listingfetch_xmltvse.cpp
    main.cpp
    programmedata.h
    )

ADD_EXECUTABLE(ipdvr ${SOURCES})
TARGET_COMPILE_FEATURES(ipdvr PRIVATE cxx_auto_type)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_COMPILE_DEFINITIONS(ipdvr PRIVATE -DDEBUG)
ENDIF()

# CURL

FIND_PACKAGE(CURL)
IF(CURL_FOUND)
    INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(ipdvr ${CURL_LIBRARIES})
ELSE()
    MESSAGE(FATAL_ERROR "Could not find libcurl.")
ENDIF()
