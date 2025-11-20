#include "client.h"

#define CLEAR_SCREEN   "\x1b[2J"
#define CLEAR_LINE     "\x1b[2K"
#define CURSOR_HOME    "\x1b[H"
#define CURSOR_HIDE    "\x1b[?25l"
#define CURSOR_SHOW    "\x1b[?25h"

#define MOVE(row, col)  CSI << row << ";" << col << "H"

void *ui(void *vptr){
    (void)vptr;

    int client_state = get_state();
    while(true){
        /*
        use switch case to determine what info to show
        */
        switch (client_state)
        {
        case S_login_option:
            cout << CLEAR_SCREEN << CURSOR_HOME;
            cout << "test:haha" << flush;
            break;
        
        default:
            break;
        }

        Pthread_mutex_lock(&ui_mutex);
        Pthread_cond_wait(&ui_cond, &ui_mutex);
        client_state = get_state();
        /*
        use switch case to determine what info to copy
        */
        

        Pthread_mutex_unlock(&ui_mutex);
        
    }
}