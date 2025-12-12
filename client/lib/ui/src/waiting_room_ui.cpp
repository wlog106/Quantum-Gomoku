#include "../ui.h"

void waiting_room_ui(){
    cout << CLEAR_SCREEN << CURSOR_HOME << CURSOR_HIDE;
    int ready_pos = MAX_ACCOUNT_LEN;
    cout << "\x1b[1m              Waiting Room" << endl;
    cout << "            Room id : " << waiting_room_id << "\x1b[0m" << endl;
    cout << "════════════════════════════════════════" << endl;
    cout << "\x1b[0m\x1b[38;5;255;48;5;0m1.Player Black:\x1b[0m ";
    cout << (waiting_user_existance[0] ? waiting_username[0] : "(empty)");
    if(waiting_user_existance[0]) cout << MOVE(4, 18 + ready_pos) << (waiting_is_ready[0] ? "  (ready)" : "(unready)");
    cout << endl;
    cout << "\x1b[0m\x1b[38;5;0;48;5;255m2.Player White:\x1b[0m ";
    cout << (waiting_user_existance[1] ? waiting_username[1] : "(empty)");
    if(waiting_user_existance[1]) cout << MOVE(5, 18 + ready_pos) << (waiting_is_ready[1] ? "  (ready)" : "(unready)");
    cout << endl;
    cout << "\x1b[0m\x1b[38;5;141;48;5;228m3.Observer 1  :\x1b[0m ";
    cout << (waiting_user_existance[2] ? waiting_username[2] : "(empty)") << endl;
    cout << "\x1b[0m\x1b[38;5;141;48;5;228m4.Observer 2  :\x1b[0m ";
    cout << (waiting_user_existance[3] ? waiting_username[3] : "(empty)") << endl;
    cout << "\x1b[0m\x1b[38;5;141;48;5;228m5.Observer 3  :\x1b[0m ";
    cout << (waiting_user_existance[4] ? waiting_username[4] : "(empty)") << endl;
    cout << "═════════════════Chat═══════════════════" << endl;
    for(int i = 5; i > int(waiting_room_history_message.size()); i--) cout << '\n';
    for(auto it = waiting_room_history_message.begin(); it != waiting_room_history_message.end(); it++){
        cout << *it << endl;
    }
    cout << "════════════════════════════════════════" << endl;
    if(waiting_room_entering){
        cout << "Text: " << waiting_room_input_box << endl;
        cout << "Press Enter to send or leave" << endl;
    }
    else{
        cout << "Press Esc to leave room\nPress R to ready/unready\n" << flush;
        cout << "Press Number 1~5 to change position\n";
        cout << "Press Enter to enter message" << endl; 
    }
}