#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

size_t NUM_PIXELS_TO_PRINT;

int colorProm(int code, int imageNumber)
{
    /**
     * Esta función se encarga de calcular el color promedio de la imagen recibida
     * y de guardar la imagen en la carpeta correspodiente.
     * 
     * Parámetros de entrada:
     *      code: Es el código que identifica si una imagen debe ser procesada o no.
     *            Si tiene un valor igual a 2 indica que si debe ser procesada la imagen
     *            y guardarse en alguna de las carpetas R, G o B.  
     *            Si tiene un valor igual a 3 indica que la imagen no debe ser procesada 
     *            y solo se guarda en carpeta Not Trusted.  
     *      imageNumber: Es el número de imagen recibida para contatenarlo al nombre de las
     *            imágenes guardadas y no sobreescribirlas.
     * 
     * Valor de salida: Se retorna un 0 cuando la función finaliza.
     * */
    int width, height, comp, r, g, b, rprom, gprom, bprom;
    r = 0;
    g = 0;
    b = 0;
    unsigned char *data = stbi_load("/home/Tarea1/TMP/temp.jpeg", &width, &height, &comp, 3);
    NUM_PIXELS_TO_PRINT = width * height;

    char name[100] = "resultado";
    char num[100];
    if (imageNumber != 0)
    {
        snprintf(num, sizeof(num), "%d", imageNumber);
        strcat(name, num);
    }
    strcat(name, ".png");

    if (data && code == 2)
    {
        printf("width = %d, height = %d, comp = %d (channels)\n", width, height, comp);

        for (size_t i = 0; i < NUM_PIXELS_TO_PRINT; i++)
        {
            r = r + data[i * comp];
            g = g + data[i * comp + 1];
            b = b + data[i * comp + 2];
        }
        rprom = r / NUM_PIXELS_TO_PRINT;
        gprom = g / NUM_PIXELS_TO_PRINT;
        bprom = b / NUM_PIXELS_TO_PRINT;
        printf("Rojo promedio: %d, Verde promedio: %d, Azul promedio: %d\n", rprom, gprom, bprom);

        if (rprom > gprom && rprom > bprom)
        {
            char path1[100] = "/home/Tarea1/Resultados/R/";
            strcat(path1, name);
            stbi_write_png(path1, width, height, comp, data, width * comp);
            printf("Imagen guardada en: %s\n\n", path1);
        }
        else if (gprom > rprom && gprom > bprom)
        {
            char path2[100] = "/home/Tarea1/Resultados/G/";
            strcat(path2, name);
            stbi_write_png(path2, width, height, comp, data, width * comp);
            printf("Imagen guardada en: %s\n\n", path2);
        }
        else
        {
            char path3[100] = "/home/Tarea1/Resultados/B/";
            strcat(path3, name);
            stbi_write_png(path3, width, height, comp, data, width * comp);
            printf("Imagen guardada en: %s\n\n", path3);
        }
    }
    else if (data && code == 3)
    {
        char path4[100] = "/home/Tarea1/Resultados/Not trusted/";
        strcat(path4, name);
        stbi_write_png(path4, width, height, comp, data, width * comp);
        printf("Imagen guardada en: %s\n\n", path4);
    }
    stbi_image_free(data);
    return 0;
}