#include "lib/client.h"

int main(int argc, char **argv){
    signal(SIGINT, [](int){
        restore_terminal();
        exit(1);
    });

    set_terminal();
    //sockfd = start_connection(argv[1]);
    client_state = S_login_option;

    pthread_t tid_ui, tid_std, tid_sock;

    Pthread_create(&tid_ui  , NULL,             &ui, NULL);
    //Pthread_create(&tid_std , NULL,  &stdin_handler, NULL);
    //Pthread_create(&tid_sock, NULL, &socket_handler, NULL);

    Pthread_join(tid_ui  , NULL);
    //Pthread_join(tid_std , NULL);
    //Pthread_join(tid_sock, NULL);

    restore_terminal();
    exit(0);
}