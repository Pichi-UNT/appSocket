#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define cantIp 254
char enteroACaracter(int numero);
void conexion(char *arg);
void *prueba(char *arg);
int main()
{
    char direccion[15];
    pthread_t threads[cantIp];
    printf("Ingrese su direccion de red: ");
    scanf("%s", direccion);
    system("clear");

    int flag = 0;
    int posicionHostID;

    for (int i = 0; i < sizeof(direccion); i++)
    {

        if (flag > 2)
        {
            posicionHostID = i;
            direccion[posicionHostID] = '\0';
            break;
        }

        if (direccion[i] == '.')
        {
            flag++;
        };
    }

    direccion[posicionHostID] = '0';
    direccion[posicionHostID + 1] = '\0';

    printf("Dirección de Red %s\n", direccion);
    printf("Buscando servidores...\n");

    for (int i = 0; i < cantIp; ++i)
    {

        if ((i + 1) < 10)
        {
            char c = enteroACaracter(i + 1);
            direccion[posicionHostID] = c;
        }
        else if ((i + 1) < 100)
        {
            char c1 = enteroACaracter((i + 1) / 10);
            char c2 = enteroACaracter((i + 1) % 10);
            direccion[posicionHostID] = c1;
            direccion[posicionHostID + 1] = c2;
            direccion[posicionHostID + 2] = '\0';
        }
        else
        {
            char c1 = enteroACaracter((i + 1) / 100);
            char c2 = enteroACaracter((i + 1) / 10 % 10);
            char c3 = enteroACaracter((i + 1) % 10);
            direccion[posicionHostID] = c1;
            direccion[posicionHostID + 1] = c2;
            direccion[posicionHostID + 2] = c3;
            direccion[posicionHostID + 3] = '\0';
        }

        conexion(direccion);
    }

}

char enteroACaracter(int numero)
{
    return numero + '0';
}

void conexion(char *dir)
{
    int Descriptor;
    struct sockaddr_in servidor;
    if ((Descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error en Socket()\n");
        exit(-1);
    }
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(80);
    servidor.sin_addr.s_addr = inet_addr(dir);
    bzero(&(servidor.sin_zero), 8);

    if (connect(Descriptor, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        printf("No se encontro Servidor en IP: %s\n", dir);
    }
    else
    {
        printf("Se encontro Servidor en IP: %s\n", dir);
        close(Descriptor);
    }
}
