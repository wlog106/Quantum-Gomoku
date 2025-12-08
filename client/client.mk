CLIENT_MKDIR := $(dir $(lastword $(MAKEFILE_LIST)))

CLIENT_FLAGS := $(SHARECMD_FLAGS) $(SHAREWRAP_FLAGS) $(SHAREBOARD_FLAGS)

CLIENT_SRCS := $(shell find $(CLIENT_MKDIR) -type f \( -name "*.cpp" -o -name "*.c" \))
CLIENT_OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(patsubst %.c, $(BUILD_DIR)/%.o, $(CLIENT_SRCS)))\
               $(SHAREBOARD_OBJS) $(SHAREWRAP_OBJS)
CLIENT_LIBS := $(SHAREWRAP_LIBS) $(SHAREBOARD_LIBS)
ALL_DEPS += $(CLIENT_OBJS:.o=.d)

CLIENT_TARGET = $(BIN_DIR)/qg_client