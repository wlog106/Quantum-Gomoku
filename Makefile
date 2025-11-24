SHELL := /bin/bash
CC := gcc
CXX := g++

CFLAGS := -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -O2 -g
CFLAGS += $(shell mariadb_config --cflags)
CXXFLAGS := -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -O2 -g
CXXFLAGS += 

LIBS :=
LIBS += $(shell mariadb_config --libs)

TARGET := qg_server
SRCS_DIR := server lib
BUILD_DIR := build

SRCS := $(shell find ${SRCS_DIR} -type f \( -name '*.c' -o -name '*.cpp' \))
OBJS := $(SRCS:%.c=${BUILD_DIR}/%.o)
OBJS := $(OBJS:%.cpp=${BUILD_DIR}/%.o)
DIRS := $(sort $(dir ${OBJS}))

${BUILD_DIR}/%.o:	%.c
	${CC} ${CFLAGS} -c $< -o $@

${BUILD_DIR}/%.o:	%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

${TARGET}:	${OBJS}
	${CXX} ${OBJS} -o ${TARGET} ${LIBS}

all: clean create_dir ${TARGET}

create_dir: 
	mkdir -p ${DIRS}

clean: 
	@echo "clean all the object files in build directory"
	@rm -rf ${BUILD_DIR}/*

.PHONY:	all clean create_dir