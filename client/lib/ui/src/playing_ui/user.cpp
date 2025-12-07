#include "playing_ui.h"

void show_all_user(){
    cout << MOVE(User_info_pos_x, User_info_pos_y);
    cout << "\x1b[0m\x1b[38;5;255;48;5;0m1.Player Black:\x1b[0m ";
    cout << (playing_user_existance[0] ? playing_username[0] : "(empty)") << flush;
    cout << MOVE(User_info_pos_x+1, User_info_pos_y);
    cout << "\x1b[0m\x1b[38;5;0;48;5;255m2.Player White:\x1b[0m ";
    cout << (playing_user_existance[1] ? playing_username[1] : "(empty)") << flush;
    cout << MOVE(User_info_pos_x+2, User_info_pos_y);
    cout << "\x1b[0m\x1b[38;5;141;48;5;228m3.Observer 1  :\x1b[0m ";
    cout << (playing_user_existance[2] ? playing_username[2] : "(empty)") << flush;
    cout << MOVE(User_info_pos_x+3, User_info_pos_y);
    cout << "\x1b[0m\x1b[38;5;141;48;5;228m4.Observer 2  :\x1b[0m ";
    cout << (playing_user_existance[3] ? playing_username[3] : "(empty)") << flush;
    cout << MOVE(User_info_pos_x+4, User_info_pos_y);
    cout << "\x1b[0m\x1b[38;5;141;48;5;228m5.Observer 3  :\x1b[0m ";
    cout << (playing_user_existance[4] ? playing_username[4] : "(empty)") << flush;
}

void user_change(int pos){
    pos = 1;
    if(pos){}
    //TODO
}