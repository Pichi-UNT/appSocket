#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#define NTHREADS 254 //LA cantida de hilos que se crea corresponde  a la cantidad de ip

int hostID = 1;      //Variable global protegida con los mutex
pthread_mutex_t lock;

char enteroACaracter(int numero);
void *conexion(void *data);
int IpCorrecta(char *ip);
int CantidadPuntosCorrecta(char ip[]);
int CantidadDigPorOctetoCorrecta(char ip[]);
int CantidadOctetoCorrecta(char ip[]);
void corregirIP(char *ip);
int rangoOctetoCorrecto(char ip[]);


void *conexion(void *data)
{
    //Mutex para proteger la variable global contador y que no se creen 2 ip iguales en un hilo
    pthread_mutex_lock(&lock);
    int flag = 0;
    char s_host_id_invert[100];
    char s_host_id[100];
    char base[15];
    strcpy(base, (char *)data);
    //Calcular IP
    int aux=hostID;
    int cont=0;
    while(aux!=0){
        s_host_id_invert[cont]=enteroACaracter(aux%10);
        aux=aux/10;
        cont++;
    }
    s_host_id_invert[cont]='\0';
    int tam=strlen(s_host_id_invert);
    int i=0;
    while(tam>0)
    {
       s_host_id[i]=s_host_id_invert[tam-1];
       tam--;
       i++;
    }
    s_host_id[i]='\0';
    strcat(base,s_host_id);
    //FIn calculo
    hostID++;
    pthread_mutex_unlock(&lock);

    int Descriptor = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servidor;
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(80);
    servidor.sin_addr.s_addr = inet_addr(base);
    bzero(&(servidor.sin_zero), 8);

    if (connect(Descriptor, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        printf("\x1b[0mNo se encontro un servidor web en la IP: %s\n", base);
        close(Descriptor);
    }
    else
    {
        printf("\x1b[33mSe encontro un servidor web en la IP: %s\n", base);
        close(Descriptor);
    }
}

int main()
{
    char direccion[100];
    //Inicializo el mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n ERROR EN MUTEX\n");
        return 1;
    }
    //
    printf("\n\x1b[32m#####Bienvenido a PCFE_ServerDetector#####\n");
    printf("\n\x1b[32m#####INICIANDO EJECUCION#####\n");
    do
    {
        printf("Si la direccion ingresada es incorrecta se pedira que lo vuelva a intentar\n");
        printf("Ingrese su direccion de red(solo admite direccion con mascara /24): ");
        scanf("%s", direccion);
        system("clear");
    } while (!IpCorrecta(direccion));
    corregirIP(direccion);
    //Correccion IP

    printf("\x1b[32mDirección de Red %s0\n", direccion);
    printf("\x1b[32mBuscando servidores...\n");

    /* ==================== DEMO 3 ==================== */

    pthread_t threads[NTHREADS];

    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_create(&threads[i], NULL, conexion, (void *)direccion);
    }

    /* Esperar threads*/
    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    printf("\n\x1b[32m#####FIN DE EJECUCION#####\n");
    return 0;
}

char enteroACaracter(int numero)
{
    return numero + '0';
}

int IpCorrecta(char *ip)
{
    int flag = 1;
    if (!CantidadPuntosCorrecta(ip) || !CantidadDigPorOctetoCorrecta(ip) || !CantidadOctetoCorrecta(ip) || !rangoOctetoCorrecto(ip))
    {
        flag = 0;
    }

    return flag;
}

int CantidadPuntosCorrecta(char ip[])
{
    int flag = 1;
    int cantPuntos = 0;
    for (int i = 0; i < strlen(ip); i++)
    {
        if (ip[i] == '.')
        {
            cantPuntos++;
        }
    }
    if (cantPuntos > 3 || cantPuntos < 2)
    {
        flag = 0;
    }
    return flag;
}

int CantidadDigPorOctetoCorrecta(char ip[])
{
    int flag = 1;
    int cantDig = 0;
    for (int i = 0; i < strlen(ip); i++)
    {
        if (isdigit(ip[i]) == 1)
        {
            cantDig++;
        }
        if (cantDig > 3)
        {
            flag = 0;
            break;
        }
        if (ip[i] == '.')
        {
            cantDig = 0;
            continue;
        };
    }

    return flag;
}

int CantidadOctetoCorrecta(char ip[])
{
    int flag = 1;
    int posicionUltimoOcteto;
    int cantPuntos = 0;
    for (int i = 0; i < strlen(ip); i++)
    {
        if (ip[i] == '.')
        {
            cantPuntos++;
        }
        if (cantPuntos == 2)
        {
            posicionUltimoOcteto = i;
            break;
        }
    }
    if (!isdigit(ip[posicionUltimoOcteto + 1]))
    {
        flag = 0;
    }

    return flag;
}

void corregirIP(char *ip)
{
    int cantPuntos = 0;
    int posicionTpunto;
    int tam = strlen(ip);
    for (int i = 0; i < tam; i++)
    {
        if (ip[i] == '.')
        {
            cantPuntos++;
        }
        if (cantPuntos == 3)
        {
            posicionTpunto = i;
            break;
        }
    }
    if (cantPuntos == 2)
    {
        if (isdigit(ip[tam - 1]))
        {
            ip[tam] = '.';
            ip[tam + 1] = '\0';
        }
    }
    if (cantPuntos == 3)
    {
        ip[posicionTpunto] = '.';
        ip[posicionTpunto + 1] = '\0';
    }
}
int rangoOctetoCorrecto(char ip[])
{
    int flag = 1;
    int posicionUltimoOcteto;
    int cont = 0;
    char aux[100];
    strcpy(aux, ip);

    // Extract the first token
    char *token = strtok(aux, ".");
    // loop through the string to extract all other tokens
    while (token != NULL && cont<3)
    {
       if((int)strtol(token, NULL, 10)>254){
            flag=0;
            break;
        }
        cont++;
        token = strtok(NULL, ".");
    }

    return flag;
}



