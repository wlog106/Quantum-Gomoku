include config.mk

# common modules
include share/board/board.mk
include share/cmd/share_cmd.mk
include share/wrap/wrap.mk

# server modules
include server/lib/mariadb/mariadb.mk
include server/lib/objects/objects.mk
include server/lib/server_cmd/server_cmd.mk
include server/lib/utils/utils.mk

include server/server_main/server_main.mk
include server/db_worker/db_worker.mk
include server/playing_room/playing_room.mk

# client modules
include client/client.mk

# share extra flags
$(SHAREBOARD_OBJS):  CPP_EXTRA_FLAGS := $(SHAREBOARD_FLAGS)
$(SHAREWRAP_OBJS):   CPP_EXTRA_FLAGS := $(SHAREWRAP_FLAGS)

# server extra flags
$(LIBMARIADB_OBJS):    C_EXTRA_FLAGS := $(LIBMARIADB_FLAGS)
$(LIBOBJECTS_OBJS):  CPP_EXTRA_FLAGS := $(LIBOBJECTS_FLAGS)
$(LIBUTILS_OBJS): 	 CPP_EXTRA_FLAGS := $(LIBUTILS_FLAGS)
$(SERVERMAIN_OBJS):  CPP_EXTRA_FLAGS := $(SERVERMAIN_FLAGS)
$(DBWORKER_OBJS):	 CPP_EXTRA_FLAGS := $(DBWORKER_FLAGS)
$(PLAYINGROOM_OBJS): CPP_EXTRA_FLAGS := $(PLAYINGROOM_FLAGS)

# client extra flags
$(CLIENT_OBJS):		 CPP_EXTRA_FLAGS := $(CLIENT_FLAGS)

# server targets
$(SERVERMAIN_TARGET): $(SERVERMAIN_OBJS)
	@mkdir -p $(dir $@)
	@echo "- Linking $@ ..."
	@$(CXX) $^ -o $@ $(SERVERMAIN_LIBS)
	@echo "- Done"

$(DBWORKER_TARGET): $(DBWORKER_OBJS)
	@mkdir -p $(dir $@)
	@echo "- Linking $@ ..."
	@$(CXX) $^ -o $@ $(DBWORKER_LIBS)
	@echo "- Done"

$(PLAYINGROOM_TARGET): $(PLAYINGROOM_OBJS)
	@mkdir -p $(dir $@)
	@echo "- Linking $@ ..."
	@$(CXX) $^ -o $@ $(PLAYINGROOM_LIBS)
	@echo "- Done"

# client targets
$(CLIENT_TARGET): $(CLIENT_OBJS)
	@mkdir -p $(dir $@)
	@echo "- Linking $@ ..."
	@$(CXX) $^ -o $@ $(CLIENT_LIBS)
	@echo "- Done"

# generic rules
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "- Compiling $< ..."
	@$(CC) $(CFLAGS) $(C_EXTRA_FLAGS) $< -o $@
	@echo "- Done"

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "- Compiling $< ..."
	@$(CXX) $(CXXFLAGS) $(CPP_EXTRA_FLAGS) $< -o $@
	@echo "- Done"

-include $(sort $(ALL_DEPS))

all: $(CLIENT_TARGET) $(SERVERMAIN_TARGET)\
	 $(DBWORKER_TARGET) $(PLAYINGROOM_TARGET)\
	 send_complete

server: $(SERVERMAIN_TARGET) $(DBWORKER_TARGET) $(PLAYINGROOM_TARGET)\
		send_complete

client: $(CLIENT_TARGET) send_complete

clean:
	@echo "- Clean build directory..."
	@rm -rf $(BUILD_DIR)/*
	@echo "- Done"

debug:
	@echo $(LIBOBJECTS_OBJS)

send_complete:
	@echo "Complete!! binarys can be found under ${PROJECT_ROOT}/bin/"

.PHONY: clean debug send_complete