
#compile tool
CROSS := 
MYGCC := ${CROSS}g++

#root dir
ROOTDIR := $(shell pwd)

#project dir
BIN_DIR     := ${ROOTDIR}/bin
INCLUDE_DIR := ${ROOTDIR}/include
SRC_DIR     := ${ROOTDIR}/src

#target name
MYTARGET:= ${BIN_DIR}/client

#source file,*.c and *.cpp
CPP_SRC := $(shell find ${SRC_DIR} -name "*.cpp" -type f)
C_SRC   := $(shell find ${SRC_DIR} -name "*.c" -type f)

#debug
#$(warning CPP_SRC : ${CPP_SRC})

CPPFLAG := -std=c++11


.PHONY: all clean

all: 
	${MYGCC} ${CPPFLAG} -o ${MYTARGET} ${CPP_SRC}


clean:
	rm -rf ${MYTARGET}