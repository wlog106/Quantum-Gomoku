LIBMARIADB_MKDIR := $(dir $(lastword $(MAKEFILE_LIST)))

LIBMARIADB_FLAGS := $(shell mariadb_config --cflags) \
					-I$(LIBMARIADB_MKDIR)include

LIBMARIADB_SRCS := $(wildcard $(LIBMARIADB_MKDIR)src/*.c)
LIBMARIADB_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(LIBMARIADB_SRCS))
LIBMARIADB_LIBS := $(shell mariadb_config --libs)

ALL_DEPS += $(LIBMARIADB_OBJS:.o=.d)