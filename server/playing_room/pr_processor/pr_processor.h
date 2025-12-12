#include <queue>

struct Game;
struct conn;

void pr_processor(
    Game *g,
    conn *u,
    std::queue<std::pair<int, char *>> &q
);