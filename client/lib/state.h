#pragma once

typedef enum{
    S_end,
    S_login_option,
    S_login_with_account,
    S_creating_account,
    S_select_option,
    S_select_enter_room_id,
    S_waiting_room,
    S_playing,
    S_debug
}State_t;