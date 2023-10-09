#include <stdio.h>  //Entrada e saída
#include <stdlib.h> //Alocação de memória
#include <string.h> //Manipulação de string
#include <unistd.h> //Chamadas de sistema
#include <sys/wait.h> //Espera de processos

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {-
        printf("MeuShell> ");
        fgets(input, MAX_INPUT_SIZE, stdin);

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf("Saindo do shell...\n");
            break;
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("Erro ao criar um processo filho");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Processo filho
            
            char* args[64];
            char* token = strtok(input, " ");
            int arg_count = 0;

            while (token != NULL) {
                args[arg_count] = token;
                token = strtok(NULL, " ");
                arg_count++;
            }

            args[arg_count] = NULL;

            // Executar o comando
            execvp(args[0], args);

            perror("Erro ao executar o comando");
            exit(EXIT_FAILURE);
        } else { // Processo pai
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                printf("O comando terminou com código de saída %d\n", WEXITSTATUS(status));
            }
        }
    }

    return 0;
}
