include config.mk

# common modules
#

# server modules
include server/libs/mariadb/mariadb.mk
include server/libs/objects/objects.mk
include server/libs/server_cmd/server_cmd.mk
include server/libs/utils/utils.mk

include server/server_main/server_main.mk
include server/db_worker/db_worker.mk

# client modules
#

# server objects' flags
$(LIBMARIADB_OBJS): C_EXTRA_FLAGS 	:= $(LIBMARIADB_FLAGS)
$(LIBOBJECTS_OBJS): CPP_EXTRA_FLAGS := $(LIBOBJECTS_FLAGS)
$(LIBUTILS_OBJS): 	CPP_EXTRA_FLAGS := $(LIBUTILS_FLAGS)
$(SERVERMAIN_OBJS): CPP_EXTRA_FLAGS := $(SERVERMAIN_FLAGS)
$(DBWORKER_OBJS):	CPP_EXTRA_FLAGS := $(DBWORKER_FLAGS)

# server targets
$(SERVERMAIN_TARGET): $(SERVERMAIN_OBJS) $(SERVERMAIN_LIBS)
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@

$(DBWORKER_TARGET): $(DBWORKER_OBJS) $(DBWORKER_LIBS)
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@

# generic rules
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(C_EXTRA_FLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPP_EXTRA_FLAGS) $< -o $@


all: $(SERVERMAIN_TARGET) $(DBWORKER_TARGET)

clean:
	@echo "Clean build directory..."
	@rm -rf $(BUILD_DIR)/*
	@echo "done"

.PHONY: clean