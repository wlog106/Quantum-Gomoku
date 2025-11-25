#pragma once
#include "../client.h"

#define CSI "\x1b["

#define CLEAR_SCREEN   "\x1b[2J"
#define CLEAR_LINE     "\x1b[2K"
#define CURSOR_HOME    "\x1b[H"
#define CURSOR_HIDE    "\x1b[?25l"
#define CURSOR_SHOW    "\x1b[?25h"

#define MOVE(row, col)  CSI << row << ";" << col << "H"

//login page
void login_option_ui();
void login_with_account_ui();
void creating_account_ui();

//select page
void select_option_ui();