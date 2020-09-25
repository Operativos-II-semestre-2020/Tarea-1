FROM ubuntu:latest

RUN apt-get update

RUN apt-get install -y gcc

RUN mkdir -p /home/Tarea1/TMP

COPY Servidor.c /home/Tarea1/Servidor.c

COPY ColorPromedio.h /home/Tarea1/ColorPromedio.h

COPY stb_image.h /home/Tarea1/stb_image.h

COPY stb_image_write.h /home/Tarea1/stb_image_write.h

WORKDIR /home/Tarea1/

RUN gcc -o servidor Servidor.c -lm

CMD ./servidor

