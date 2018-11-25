/*
Servidor

Autores:        Rodrigo Toshiaki Horie  26620
                Tiago Coli Resende      2016000103
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc , char *argv[])
{
        
        int socket_desc , client_sock , c , read_size, port, port_val = 0;
        struct sockaddr_in server , client;
        char client_message[40000];

        //cria arquivo para registro das mensagens
        ofstream log;
        log.open("log.txt");

        printf("Iniciando servidor ...\n");

        //Configura porta do servidor
        while (port_val == 0) {
                printf("Port:");
                scanf("%d", &port);
                        if (port <= 1023) {
                                printf("Porta reservada\nEscolha outra porta...\n\n");
                        }else {
                                printf("Porta configurada com sucesso!\n");
                                port_val++;
                        }
        }

        //Cria socket
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc == -1)
        {
                printf("Não foi possível criar o socket...");
        }
        puts("Socket criado!");

        //Configura sockaddr_in
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( port );

        char buffer[100];
        const char* p = inet_ntop(AF_INET, &server.sin_addr, buffer, 100);
        printf("IP: %s \n" , buffer);

        //Cria o  bind
        if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        {
                //print the error message
                perror("Falha no bind.");
                return 1;
        }
        puts("Bind realizado com sucesso!");

        //Cria listen
        listen(socket_desc , 3);

        while (true) {
                //Aguarda conexão
                puts("Aguardando conexões...");
                c = sizeof(struct sockaddr_in);

                //Aceita conexão de um client
                client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
                if (client_sock < 0)
                {
                        perror("Falha no accept...");
                        return 1;
                }

                puts("Conexão aceita!");

                //Recebe a mensagem do client
                while( (read_size = recv(client_sock , client_message , 40000 , 0)) > 0 )
                {
                        printf("Mensagem do client: %s", client_message);
                        log << client_message;

                        //Reennvia mensagem para o client
                        write(client_sock , client_message , strlen(client_message));


                }
                log.close();

                if(read_size == 0)
                {
                        puts("Cliente desconectado...");
                        fflush(stdout);
                }
                else if(read_size == -1)
                {
                        perror("Falha no recv...");
                }
        }

        return 0;
}
