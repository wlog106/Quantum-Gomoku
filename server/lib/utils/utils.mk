LIBUTILS_MKDIR := $(dir $(lastword $(MAKEFILE_LIST)))

LIBUTILS_FLAGS := -I$(LIBUTILS_MKDIR)include

LIBUTILS_SRCS := $(wildcard $(LIBUTILS_MKDIR)src/*.cpp)
LIBUTILS_OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(LIBUTILS_SRCS))
LIBUTILS_LIBS := 

ALL_DEPS += $(LIBUTILS_OBJS:.o=.d)