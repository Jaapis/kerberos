#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main( int argc, char* argv[] ) {
    int client_socket = -1;
    int server_port   = 0;
    int quit          = 0;
    struct sockaddr_in server;
    char message[40000];
    char server_reply[40000];
    char ip_address[16];
    char terminate[] = "tchau\n";

    // Configura IP e porta do serviço
    printf( "Insira IP do server: " );
    scanf( "%s", ip_address );
    printf( "Insira a porta do server: " );
    scanf( "%d", &server_port );

    // Cria socket
    client_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( client_socket == -1 ) { printf( "[ERROR] Não foi possível criar o socket" ); }
    puts( "Socket criado" );

    server.sin_addr.s_addr = inet_addr( ip_address );
    server.sin_family      = AF_INET;
    server.sin_port        = htons( server_port );

    // Conecta ao servidor
    if ( connect( client_socket, (struct sockaddr*)&server, sizeof( server ) ) <
         0 ) {
        perror( "[ERROR] Falha na conexão" );
        return 1;
    }

    printf( "[SUCCESS] Conectado!\n" );

    // Envia mensagem para o servidor
    bzero( message, 40000 );
    bzero( server_reply, 40000 );
    fgets( message, 40000, stdin );


    // Loop da conexão do cliente
    while ( quit == 0 ) {
        // Envia mensagem para o servidor
        printf( "\nClient: " );

        bzero( message, 40000 );
        bzero( server_reply, 40000 );
        fgets( message, 40000, stdin );

        // Recebe mensagem de resposta do servidor
        write( client_socket, message, strlen( message ) + 1 );
        read( client_socket, server_reply, 40000 );
        printf( "Server: %s", server_reply );

        // Compara com palavra de finalização (char terminate) para encerrar a
        // conexão
        if ( strcmp( terminate, message ) == 0 ) {
            quit++;
            printf( "Saindo do programa..." );
        }
    }

    close( client_socket );
    return 0;
}
