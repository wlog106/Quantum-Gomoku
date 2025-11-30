#include "../ui.h"
#include <ostream>

void select_option_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << flush;
    cout << ((opselect_option == 0) ? ">" : " ") << "\x1b[1mCreate a new room\x1b[0m\n" << flush;
    cout << ((opselect_option == 1) ? ">" : " ") << "\x1b[1mJoin by room ID\x1b[0m\n" << flush;
    cout << ((opselect_option == 2) ? ">" : " ") << "\x1b[1mPair a random opponent\x1b[0m\n" << flush;
    cout << ((opselect_option == 3) ? ">" : " ") << "\x1b[1mObserve a random room\x1b[0m\n" << flush;
    cout << "----------------------------------------\n";
    cout << "Press Esc to log out\nPress Enter to confirm\nPress Up/Down arrow to select\n" << flush;
    cout << "----------------------------------------\n";
}

void select_enter_room_id_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << flush;
    cout << "\x1b[0mRoom id: " << room_id_input_box << endl;
    cout << "----------------------------------------\n";
    cout << "Press Esc to cancel\nPress Enter to confirm\n" << flush;
    cout << "----------------------------------------\n";
}