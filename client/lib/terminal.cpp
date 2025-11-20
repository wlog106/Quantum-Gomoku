#include "client.h"

#define ALT_SCREEN_ON   "\x1b[?1049h"
#define ALT_SCREEN_OFF  "\x1b[?1049l"
#define CLEAR_SCREEN    "\x1b[2J"
#define CURSOR_SHOW     "\x1b[?25h"
#define CURSOR_HOME     "\x1b[H"

struct termios oldt, newt;

void set_terminal(){
    tcgetattr(STDIN_FILENO, &oldt); 
    memcpy(&newt, &oldt, sizeof(newt));
    newt.c_lflag&= ~( ECHO | ICANON | ECHOE | ECHOK |
    ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    cout << ALT_SCREEN_ON << flush;

}

void restore_terminal(){
    cout << ALT_SCREEN_OFF << CURSOR_SHOW << flush;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}