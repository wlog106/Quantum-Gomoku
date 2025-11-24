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
    pipe(ui_end_pipe);
    pipe(std_handler_end_pipe);
    pipe(socket_reader_end_pipe);
    pipe(socket_writer_end_pipe);
    signal(SIGINT, [](int){
        //use four write to terminate client
        write(std_handler_end_pipe[1], "x", 1);
        write(ui_end_pipe[1], "x", 1); //order does matter

        write(socket_reader_end_pipe[1], "x", 1);
        write(socket_writer_end_pipe[1], "x", 1); //order does matter
    });

    set_terminal();
    client_state = S_login_option;

    sockfd = start_connection(argv[1]);
    
    pthread_t tid_ui, tid_std, tid_socket_writer, tid_socket_reader;

    Pthread_create(&tid_ui  , NULL,             &ui, NULL);
    Pthread_create(&tid_std , NULL,  &stdin_handler, NULL);
    Pthread_create(&tid_socket_writer, NULL, &socket_writer, NULL);
    Pthread_create(&tid_socket_reader, NULL, &socket_reader, NULL);

    Pthread_join(tid_ui  , NULL);
    Pthread_join(tid_std , NULL);
    Pthread_join(tid_socket_writer, NULL);
    Pthread_join(tid_socket_reader, NULL);

    restore_terminal();
    exit(0);
}