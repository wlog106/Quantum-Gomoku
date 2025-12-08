SHELL := /bin/bash
PROJECT_ROOT := $(CURDIR)

CC := gcc
CXX := g++

CFLAGS := -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -MMD -MP -c -O0 -g
CXXFLAGS := -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -MMD -MP -c -O0 -g

# for lib/execute to specift their flags
# e.g. $(BUILDIR)/server/libs/utils/src/%.o: CPP_EXTRA_FLAGS += ...
C_EXTRA_FLAGS := 
CPP_EXTRA_FLAGS := 

ALL_DEPS := 

BUILD_DIR := $(PROJECT_ROOT)/build
BIN_DIR := $(PROJECT_ROOT)/bin