#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define LEER 0
#define ESCRIBIR 1

const char *frase = "Envia esto a traves de un tubo o pipe";
extern int errno;

int main() {
    int fd[2], bytesLeidos;
    char mensaje[100];
    int control;

    // Se crea la tubería
    control = pipe(fd);
    if (control != 0) {
        perror("pipe:");
        exit(errno);
    }

    // Se crea el proceso hijo
    control = fork();
    switch(control) {
        case -1:
            perror("fork:");
            exit(errno);
        case 0: // Proceso hijo
            close(fd[LEER]);
            write(fd[ESCRIBIR], frase, strlen(frase) + 1);
            close(fd[ESCRIBIR]);
            exit(0);
        default: // Proceso padre
            close(fd[ESCRIBIR]);
            bytesLeidos = read(fd[LEER], mensaje, 100);
            printf("Leidos %d bytes : %s\n", bytesLeidos, mensaje);
            close(fd[LEER]);
    }
    exit(0);
}
