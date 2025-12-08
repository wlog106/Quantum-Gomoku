LIBMARIADB_MKDIR := $(dir $(lastword $(MAKEFILE_LIST)))

LIBMARIADB_FLAGS := $(shell mariadb_config --cflags) \
					-I$(LIBMARIADB_MKDIR)/include

LIBMARIADB_SRCS := $(LIBMARIADB_MKDIR)/src/*.c
LIBMARIADB_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(LIBMARIADB_SRCS))
LIBMARIADB_DEPS := $(LIBMARIADB_OBJS:.o=.d)
LIBMARIADB_LIBS := $(shell mariadb_config --libs)