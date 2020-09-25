#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "ColorPromedio.h"
#include <netdb.h>

#define MAXCHAR 1000

int receive_image(int socket)
{
  /**
   * Esta función se encarga de recibir todos los datos pertenecientes a la imagen 
   * proveniente del cliente.
   * 
   * Parámetros de entrada:
   *      socket: Es el socket en el cual se estableción la conexión de envío de la imagen.
   * 
   * Valores de salida:
   *      Retorna un -1 si no es posible guardar la imagen temporal.
   *      Retorna un 1 al finalizar el programa.
   * */

  int buffersize = 0, recv_size = 0, size = 0, read_size, write_size, packet_index = 1, stat;

  char imagearray[10241], verify = '1';
  FILE *image;

  do
  {
    stat = read(socket, &size, sizeof(int));
  } while (stat < 0);

  char buffer[] = "Got it";

  do
  {
    stat = write(socket, &buffer, sizeof(int));
  } while (stat < 0);

  image = fopen("/home/Tarea1/TMP/temp.jpeg", "w");

  if (image == NULL)
  {
    printf("No se pudo abrir la imagen temporal\n");
    return -1;
  }

  int need_exit = 0;
  struct timeval timeout = {10, 0};

  fd_set fds;
  int buffer_fd, buffer_out;

  while (recv_size < size)
  {

    FD_ZERO(&fds);
    FD_SET(socket, &fds);

    buffer_fd = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);

    if (buffer_fd < 0)
      printf("error: File descriptor erróneo.\n");

    if (buffer_fd == 0)
      printf("error: Tiempo de espera del buffer finalizado.\n");

    if (buffer_fd > 0)
    {
      do
      {
        read_size = read(socket, imagearray, 10241);
      } while (read_size < 0);

      write_size = fwrite(imagearray, 1, read_size, image);

      if (read_size != write_size)
      {
        printf("Error guardando la imagen temporal\n");
      }

      recv_size += read_size;
      packet_index++;
    }
  }

  fclose(image);
  printf("Imagen recibida de forma exitosa\n");
  return 1;
}

int revisarIp(char *ip)
{
  /**
   * Está función se encarga de revisar si la IP del cliente se encuentra en la lista de IPs 
   * preaprobadas.
   * 
   * Parámetros de entrada:
   *      ip: Contiene la IP del cliente que va a ser revisada.
   * 
   * Valores de salida: 
   *      Retorna un 1 si no se encuentra el archivo de configuración.
   *      Retorna un 2 si la IP es aprobada.
   *      Retorna un 3 si la IPes no segura.
   * */
  int flag = 1;
  FILE *fp;
  char str[MAXCHAR];
  char *filename = "/home/Tarea1/Resultados/configuracion/configuracion.config";

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("No se pudo abrir archivo de configuración %s\n", filename);
    return 1;
  }
  while (fgets(str, MAXCHAR, fp) != NULL)
  {
    if (strcmp(str, "NOT TRUSTED\n") == 0)
      flag = 0;
    else if (strcmp(str, ip) == 0 && flag)
      return 2;
    else if (strcmp(str, ip) == 0 && !flag)
      return 3;
  }
  fclose(fp);
  return 0;
}

int main(int argc, char *argv[])
{
  /**
   * Está es la función principal del servidor se encarga de crear el socket y ponerlos a la
   * escucha de nuevas conexiones y hacer uso de las demás funciones para verificación de IPs
   * y maenjo y procesamiento de imágenes.
   * 
   * Parámetros de entrada: No se reciben parámetros de entrada.
   * 
   * Valores de salida: 
   *      Retorna un 1 si hay algún error de conexión.
   *      Retorna un 0 al finalizar el programa. 
   * */
  char server_message1[256] = "Imagen procesada y guardada";
  char server_message2[256] = "Imagen Guardada pero no procesada";
  char server_message3[256] = "Conexion no aceptada";
  int socket_desc, new_socket, c, read_size, buffer = 0;
  struct sockaddr_in server, client;
  char *readin;

  struct hostent *he;
  char hostbuffer[256];
  char *IPbuffer;
  struct hostent *host_entry;
  int hostname;

  hostname = gethostname(hostbuffer, sizeof(hostbuffer));
  host_entry = gethostbyname(hostbuffer);
  IPbuffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
  printf("IP Servidor: %s\n", IPbuffer);

  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
    printf("No se pudo crear el socket\n");
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8585);

  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    puts("Fallo enlace\n");
    return 1;
  }
  puts("\nEnlace Realizado\n");

  int aceptados = 0;
  int noProcesados = 0;
  while (1)
  {

    listen(socket_desc, 3);

    puts("Esperando conexiones entrantes...\n");
    c = sizeof(struct sockaddr_in);

    if ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
      puts("Conexión aceptada\n");
    }

    fflush(stdout);

    if (new_socket < 0)
    {
      perror("Conexión fallida\n");
      return 1;
    }

    int manejoIp = revisarIp(strcat(inet_ntoa(client.sin_addr), "\n"));

    if (manejoIp == 2)
    {
      printf("Se procesa la imagen\n");
      receive_image(new_socket);
      colorProm(2, aceptados);
      aceptados = aceptados + 1;
      send(new_socket, server_message1, sizeof(server_message1), 0);
    }
    else if (manejoIp == 3)
    {
      printf("No procesa la imagen\n");
      receive_image(new_socket);
      colorProm(3, noProcesados);
      noProcesados = noProcesados + 1;
      send(new_socket, server_message2, sizeof(server_message2), 0);
    }
    else if (manejoIp == 0)
    {
      printf("No se acepta la imagen\n");
      send(new_socket, server_message3, sizeof(server_message3), 0);
    }
  }
  close(socket_desc);
  fflush(stdout);

  return 0;
}