/*
Servidor

Autores:        Rodrigo Toshiaki Horie  26620
                Tiago Coli Resende      2016000103
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main( int argc, char* argv[] ) {
    int server_socket = 0;
    int client_socket = 0;
    int quit          = 0;
    int c             = 0;
    int read_size     = 0;
    int port          = 0;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char client_message[40000];
    char ip_address[15];

    // cria arquivo para registro das mensagens
    FILE* log = fopen( "build/log.txt", "w" );
    if ( !log ) {
        perror( "[ERROR] Não foi possível abrir o arquivo" );
        return -1;
    }

    printf( "Iniciando servidor ...\n" );

    // Configura porta do servidor
    int port_sucess = 0;
    while ( port_sucess == 0 ) {
        printf( "Port: " );
        scanf( "%d", &port );
        if ( port <= 1023 ) {
            printf( "[ERROR] Porta reservada" );
            printf( "Escolha outra porta...\n" );
            printf( "\n" );
        } else {
            printf( "[SUCCESS] Porta configurada com sucesso!\n" );
            port_sucess++;
        }
    }

    // Cria socket
    server_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( server_socket == -1 ) {
        printf( "[ERROR] Não foi possível criar o socket\n" );
        return 1;
    }
    printf( "Socket criado...\n" );

    // Pega o ip do usuario
    printf( "IP: ");
    scanf( "%s", ip_address );

    // Configura sockaddr_in
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = inet_addr( (const char*) ip_address );
    server.sin_port        = htons( port );

    char buffer[100];
    inet_ntop(AF_INET, &server.sin_addr, buffer, 100);
    printf( "[SUCCESS] IP: %s \n", buffer );

    // Cria o  bind
    if ( bind( server_socket, (struct sockaddr*)&server, sizeof( server ) ) < 0 ) {
        // print the error message
        perror( "[ERROR] Falha no bind" );
        return 2;
    }
    printf( "[SUCCESS] Bind realizado com sucesso...\n" );

    // Cria listen
    listen( server_socket, 3 );

    while ( quit == 0 ) {
        // Aguarda conexão
        printf( "Aguardando conexões...\n" );
        c = sizeof( struct sockaddr_in );

        // Aceita conexão de um client
        client_socket = accept( server_socket, (struct sockaddr*)&client, (socklen_t*)&c );
        if ( client_socket < 0 ) {
            perror( "[ERROR] Falha no accept" );
            return 3;
        }

        printf( "[SUCCESS] Conexão aceita..." );

        // Recebe a mensagem do client
        while ( ( read_size = recv( client_socket, client_message, 40000, 0 ) ) > 0 ) {
            printf( "Mensagem do cliente: %s\n", client_message );
            fputs( client_message, log );
            if ( ferror( log ) ) {
                perror( "[ERROR] Erro ao escrever no arquivo" );
                fclose( log );
                return 4;
            }
            // Reennvia mensagem para o client
            write( client_socket, client_message, strlen( client_message ) );
        }
        fclose( log );

        if ( read_size == 0 ) {
            printf( "Cliente desconectado...\n" );
        } else if ( read_size == -1 ) {
            perror( "[ERROR] Falha no recebimento" );
        }
    }

    return 0;
}
