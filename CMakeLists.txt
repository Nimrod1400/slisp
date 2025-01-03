CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(slisp)

SET(CMAKE_COLOR_DIAGNOSTICS OFF)
SET(CMAKE_COLOR_MAKEFILE OFF)

ADD_EXECUTABLE(slisp
	src/slisp/main.cpp
	src/slisp/lexer.cpp
	src/slisp/types.cpp
	src/slisp/parser.cpp
)
ADD_EXECUTABLE(tests
	src/slisp/lexer.cpp
	src/slisp/types.cpp
	src/slisp/parser.cpp

	src/tests/lexer_values_tests.cpp
)

TARGET_COMPILE_FEATURES(slisp PRIVATE cxx_std_20)
TARGET_COMPILE_FEATURES(tests PRIVATE cxx_std_20)

IF(MSVC)
	TARGET_COMPILE_OPTIONS(slisp PRIVATE /W4 /WX)
	TARGET_COMPILE_OPTIONS(tests PRIVATE /W4 /WX)
ELSE()
	TARGET_COMPILE_OPTIONS(slisp PRIVATE -Wall -Wextra -Wpedantic)
	TARGET_COMPILE_OPTIONS(tests PRIVATE -Wall -Wextra -Wpedantic)
ENDIF()

#--------------------

SET(CATCH_CONFIG_NO_COLOUR_WIN32 ON)
INCLUDE(FetchContent)

FETCHCONTENT_DECLARE(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)

FETCHCONTENT_MAKEAVAILABLE(Catch2)

TARGET_LINK_LIBRARIES(tests PRIVATE Catch2::Catch2WithMain)
