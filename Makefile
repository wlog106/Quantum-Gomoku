SHELL := /bin/bash
CC := gcc
CXX := g++

CFLAGS := -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -O2 -g -MMD -MP
CFLAGS += $(shell mariadb_config --cflags)
CXXFLAGS := -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -O2 -g -MMD -MP
CXXFLAGS += 

LIBS :=
LIBS += $(shell mariadb_config --libs)

SERV_TARGET := qg_server
CLI_TARGET := qg_client

SERV_SRCS_DIR := server lib
CLI_SRCS_DIR := client lib 
BUILD_DIR := build

SERV_SRCS := $(shell find ${SERV_SRCS_DIR} -type f \( -name '*.c' -o -name '*.cpp' \))
SERV_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SERV_SRCS)))
SERV_DEP = $(SERV_OBJS:.o=.d)


CLI_SRCS := $(shell find ${CLI_SRCS_DIR} -type f \( -name '*.c' -o -name '*.cpp' \))
CLI_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CLI_SRCS)))
CLI_DEP = $(CLI_OBJS:.o=.d)

SERV_DIRS := $(sort $(dir ${SERV_OBJS}))
CLI_DIRS := $(sort $(dir ${CLI_OBJS}))
DIRS := $(sort $(dir $(SERV_OBJS) $(CLI_OBJS)))

${BUILD_DIR}/%.o:	%.c
	${CC} ${CFLAGS} -c $< -o $@

${BUILD_DIR}/%.o:	%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

${SERV_TARGET} : ${SERV_OBJS}
	${CXX} ${SERV_OBJS} -o ${SERV_TARGET} ${LIBS}

${CLI_TARGET} : ${CLI_OBJS}
	${CXX} ${CLI_OBJS} -o ${CLI_TARGET} ${LIBS}

-include $(SERV_DEP)
-include $(CLI_DEP)

all: create_dir_all ${SERV_TARGET} ${CLI_TARGET}

client: create_dir_cli ${CLI_TARGET}

server: create_dir_serv ${SERV_TARGET}

clean: clean

create_dir_all: 
	mkdir -p ${DIRS}

create_dir_cli: 
	mkdir -p ${CLI_DIRS}

create_dir_serv: 
	mkdir -p ${SERV_DIRS}

clean: 
	@echo "clean all the object files in build directory"
	@rm -rf ${BUILD_DIR}/*

.PHONY:	all clean create_dir