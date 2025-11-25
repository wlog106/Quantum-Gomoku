#include "../ui.h"

void select_option_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE << flush;
    cout << ((opselect_option == 0) ? ">" : " ") << "\x1b[1m1.Create a new room\x1b[0m\n" << flush;
    cout << ((opselect_option == 1) ? ">" : " ") << "\x1b[1m2.Join by room ID\x1b[0m\n" << flush;
    cout << ((opselect_option == 2) ? ">" : " ") << "\x1b[1m3.Pair a random opponent\x1b[0m\n" << flush;
    cout << ((opselect_option == 3) ? ">" : " ") << "\x1b[1m4.Observe a random room\x1b[0m\n" << flush;
    cout << "---------------------------------------\n";
    cout << "Press Esc to log out\nPress eEnter to confirm\nPress Up/Down arrow to select\n" << flush;
    cout << "---------------------------------------\n";
}