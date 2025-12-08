#include "lib/client.h"

/*
client termination:
write pipe to std_handler and socket_reader(cause both of them block at select)
std_handler -> change ui_end and wake ui up using signal -> write pipe to ui
same as above: socket_reader -> socket_writer
*/

int main(int argc, char **argv){
    if (argc != 2){
        cout << "usage: main <IPaddress>" << endl;
        exit(0);
    }
    Pipe(client_end_pipe);
    pthread_t tid_end;
    Pthread_create(&tid_end, NULL, &terminator, NULL);

    signal(SIGINT, [](int){
        CLOSE_CLIENT('c');
    });

    client_state = S_login_option;
    start_connection(argv[1]);

    set_terminal();
    
    pthread_t tid_ui, tid_std, tid_socket_writer, tid_socket_reader;

    Pthread_create(&tid_ui  , NULL,             &ui, NULL);
    Pthread_create(&tid_std , NULL,  &stdin_handler, NULL);
    Pthread_create(&tid_socket_writer, NULL, &socket_writer, NULL);
    Pthread_create(&tid_socket_reader, NULL, &socket_reader, NULL);

    Pthread_join(tid_ui  , NULL);
    Pthread_join(tid_std , NULL);
    Pthread_join(tid_socket_writer, NULL);
    Pthread_join(tid_socket_reader, NULL);
    Pthread_join(tid_end, NULL);

    shutdown(server_sockfd, SHUT_RDWR);
    restore_terminal();
    if(client_end_code == 'c'){
        cout << "Ctrl-C keyboard interrupt" << endl;
    }
    else if(client_end_code == 's'){
        cout << "Server closed permanently" << endl;
    }
    else if(client_end_code == 'd'){
        cout << "Ctrl-D keyboard interrupt" << endl;
    }
    exit(0);
}