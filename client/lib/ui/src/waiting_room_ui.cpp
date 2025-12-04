#include "../ui.h"

void waiting_room_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE;
    int ready_pos = max(16, max((waiting_user_existance[0] ? int(waiting_username[0].size()) : 7), (waiting_user_existance[1] ? int(waiting_username[1].size()) : 7)));
    cout << "\x1b[1mWaiting Room" << endl;
    cout << "Room id: " << waiting_room_id << endl;
    cout << "════════════════════════════════════════" << endl;
    cout << "\x1b[0m\x1b[40;37m1.Player Black:\x1b[0m ";
    cout << (waiting_user_existance[0] ? waiting_username[0] : "(empty)");
    if(waiting_user_existance[0]) cout << MOVE(4, 16 + ready_pos) << (waiting_is_ready[0] ? "  (ready)" : "(unready)");
    cout << endl;
    cout << "\x1b[0m\x1b[30;47m2.Player White:\x1b[0m ";
    cout << (waiting_user_existance[1] ? waiting_username[1] : "(empty)");
    if(waiting_user_existance[1]) cout << MOVE(5, 16 + ready_pos) << (waiting_is_ready[1] ? "  (ready)" : "(unready)");
    cout << endl;
    cout << "\x1b[0m\x1b[40;37m3.Observer 1  :\x1b[0m ";
    cout << (waiting_user_existance[2] ? waiting_username[2] : "(empty)") << endl;
    cout << "\x1b[0m\x1b[40;37m4.Observer 2  :\x1b[0m ";
    cout << (waiting_user_existance[3] ? waiting_username[3] : "(empty)") << endl;
    cout << "\x1b[0m\x1b[40;37m5.Observer 3  :\x1b[0m ";
    cout << (waiting_user_existance[4] ? waiting_username[4] : "(empty)") << endl;
    cout << "════════════════════════════════════════" << endl;
    cout << "Press Esc to leave room\nPress R to ready/unready\n" << flush;
    cout << "Press Number 1~5 to change position\n";
}