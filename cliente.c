#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
int main(void)
{
    int fd1; //Descriptores de archivo
    int tama_sin;
    struct sockaddr_in servidor; // Info de la direccion del cliente
    char direccion [100];
    printf("Ingrese su direccion de red");
    scanf("%s",direccion);
    system("clear");
    printf("\t********BIENVENIDO AL CLIENTE!********\n\n");
    if ((fd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error en Socket()\n");
        exit(-1);
    }
    for (size_t i = 1; i <= 254; i++)
    {
        servidor.sin_family = AF_INET;
        servidor.sin_port = htons(80); // Convierte PORT al numero de red
        servidor.sin_addr.s_addr = inet_addr(strcat(direccion,strcat('.',char(i)));
        bzero(&(servidor.sin_zero), 8); //Coloca ceros en resto estructura

        if (connect(fd1, (struct sockaddr *)&servidor, sizeof(servidor)) != 0)
        {
            printf("connection with the server failed...\n");
            exit(0);
        }
        else
            printf("connected to the server..\n");
    }
}