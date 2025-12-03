#include "../ui.h"

void select_option_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << flush;
    cout << ((opselect_option == 0) ? ">" : " ") << "\x1b[1mCreate a new room\x1b[0m\n" << flush;
    cout << ((opselect_option == 1) ? ">" : " ") << "\x1b[1mJoin by room ID\x1b[0m\n" << flush;
    cout << ((opselect_option == 2) ? ">" : " ") << "\x1b[1mPair a random opponent\x1b[0m\n" << flush;
    cout << ((opselect_option == 3) ? ">" : " ") << "\x1b[1mObserve a random room\x1b[0m\n" << flush;
    cout << "────────────────────────────────────────\n";
    cout << "Press Esc to log out\nPress Enter to confirm\nPress Up/Down arrow to select\n" << flush;
    cout << "────────────────────────────────────────\n";
    switch (opselect_reply) {
        case OSR_waiting:
            cout << "\x1b[1mWaiting...\n";
            break;
        case OSR_too_much_room:
            cout << "\x1b[1;31mThere are too many rooms already exist. Please try again later.\n";
            break;
        case OSR_no_empty_waiting_room:
            cout << "\x1b[1;31mThere are currently no rooms with an available battle slot to join.\n";
            cout << "Please try again later\n";
            break;
        case OSR_no_current_playing_room:
            cout << "\x1b[1;31mNo rooms are currently playing. Please try again later.\n";
            break;
        default:
            break;
    }
    cout << "\x1b[0m" << flush;
}

void select_enter_room_id_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << flush;
    cout << "\x1b[0mRoom id: " << room_id_input_box << endl;
    cout << "────────────────────────────────────────\n";
    cout << "Press Esc to cancel\nPress Enter to confirm\n" << flush;
    cout << "────────────────────────────────────────\n";
    switch (opselect_reply) {
        case OSR_waiting:
            cout << "\x1b[1mWaiting...\n";
            break;
        case OSR_room_id_len_error:
            cout << "\x1b[1;31mRoom ID length error.\nThe room ID should be " << ROOM_ID_LEN << " charactors.\n";
            break;
        case OSR_room_id_dne:   
            cout << "\x1b[1;31mThis room ID does not exist. Please try another one.\n";
            break;
        case OSR_room_already_full:
            cout << "\x1b[1;31mThis room is already full. Please try again later.\n";
            break;
        default:
            break;
    }
    cout << "\x1b[0m" << flush;
}