#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int pipe_fd[2]; // pipe_fd[0] = lectura, pipe_fd[1] = escritura
    pid_t pid;
    char buffer[BUFFER_SIZE];
    char mensaje[] = "Hola desde el padre";

    // Crear el pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Crear proceso hijo
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Proceso hijo
        close(pipe_fd[1]); // Cierro extremo de escritura
        
        // Leer del pipe
        read(pipe_fd[0], buffer, BUFFER_SIZE);
        printf("Hijo recibió: %s\n", buffer);
        
        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    } 
    else { // Proceso padre
        close(pipe_fd[0]); // Cierro extremo de lectura
        
        // Escribir en el pipe
        printf("Padre enviando mensaje...\n");
        write(pipe_fd[1], mensaje, strlen(mensaje)+1);
        
        wait(NULL); // Esperar al hijo
        close(pipe_fd[1]);
    }

    return 0;
}