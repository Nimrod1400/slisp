CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(slisp)

ADD_EXECUTABLE(slisp
	src/slisp/main.cpp
)
ADD_EXECUTABLE(tests
	src/tests/main.cpp
)
