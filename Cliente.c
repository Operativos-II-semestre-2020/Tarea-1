#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

int i = 0;
int send_image(int socket, char *imagen)
{
   /**
    * Esta función se encarga de enviar toda la información de la imagen ingresada por 
    * el usuario por medio del socket.
    * 
    * Parámetros de entrada:
    *       socket: Es el socket por el cual se va a enviar la imagen.
    *       imagen: Es la ruta de la imagen que se va a enviar.
    * 
    * Valores de salida:
    *       Retorna un 0 cuando finaliza el programa.
    * */

   FILE *picture;
   int size, read_size, stat, packet_index;
   char send_buffer[10240], read_buffer[256];
   packet_index = 1;

   picture = fopen(imagen, "r");

   fseek(picture, 0, SEEK_END);
   size = ftell(picture);
   fseek(picture, 0, SEEK_SET);

   write(socket, (void *)&size, sizeof(int));

   do
   {
      stat = read(socket, &read_buffer, 255);
   } while (stat < 0);

   printf("Datos enviados\n");

   while (!feof(picture))
   {

      read_size = fread(send_buffer, 1, sizeof(send_buffer) - 1, picture);

      do
      {
         stat = write(socket, send_buffer, read_size);
      } while (stat < 0);

      packet_index++;

      bzero(send_buffer, sizeof(send_buffer));
   }
   char server_response[256];
   recv(socket, &server_response, sizeof(server_response), 0);
   if (strcmp(server_response, "") != 0)
   {
      printf("Respuesta del servidor: %s\n", server_response);
   }
   else
   {
      printf("Respuesta del servidor: Conexión rechazada -> IP no admitida\n");
   }

   return 0;
}

int main(int argc, char *argv[])
{
   /**
    * Está es la función principal del cliente. Se encarga de crear el socket del cliente,
    * solicitar la ruta de la imagen y hacer de la función para enviar la imagen por el socket.
    * 
    * Parámetros de entrada:
    *       argv[1]: Recibe la IP del sevidor al cual conectarse.
    * 
    * Valores de salida:
    *       Retorna un 1 si ocurre algún error a la hora de conectarse al servidor.
    *       Retorna un 0 cuando finaliza el programa.
    * 
    * */

   int socket_desc;
   struct sockaddr_in server;
   char *parray;

   FILE *picture;
   if (argv[1] == NULL)
   {
      printf("IP del servidor no ingresada como parámetro\nFinalizando programa...\n");
      exit(0);
   }

   while (i == 0)
   {

      char imagen[100];
      printf("\nIngrese la ruta y nombre de la imagen: ");
      scanf("%s", imagen);
      if (strcmp(imagen, "fin") == 0)
      {
         break;
      }

      picture = fopen(imagen, "r");

      if (picture == NULL)
      {
         printf("error: No se encontró la imagen ingresada\n\n");
      }
      else
      {

         socket_desc = socket(AF_INET, SOCK_STREAM, 0);

         if (socket_desc == -1)
         {
            printf("No se pudo crear el socket");
         }

         memset(&server, 0, sizeof(server));
         server.sin_addr.s_addr = inet_addr(argv[1]);
         server.sin_family = AF_INET;
         server.sin_port = htons(8585);

         if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
         {

            close(socket_desc);
            puts("Error conectando con el servidor\n");
            return 1;
         }

         puts("Conectado con el servidor\n");

         send_image(socket_desc, imagen);

         close(socket_desc);
      }
   }
   return 0;
}