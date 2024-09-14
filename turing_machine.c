#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 1000 // Tamaño de la cinta

typedef struct {
    char *tape;   // Cinta de la máquina
    int head;     // Posición de la cabeza
    char state[10]; // Estado actual
    char trf[100][5][5]; // Tabla de estados internos
} TuringMachine;

// Función para inicializar la máquina de Turing
void initTuringMachine(TuringMachine *tm, const char *algo, const char *input, int state) {
    tm->tape = (char *)malloc(N * sizeof(char));
    memset(tm->tape, '_', N); // Inicializar la cinta con guiones bajos
    tm->head = N / 2; // Colocar la cabeza en el centro de la cinta
    strncpy(tm->tape + tm->head, input, strlen(input)); // Insertar el input en el centro de la cinta
    snprintf(tm->state, sizeof(tm->state), "%d", state); // Establecer el estado inicial

    printf("\nInternal states table\n---------------------\n");

    // Leer y asignar los valores de la tabla de estados internos
    int line_num = 0;
    char line[256];
    const char *delim = ",";
    char *token;
    char algo_copy[strlen(algo) + 1];
    strcpy(algo_copy, algo);

    char *line_ptr = strtok(algo_copy, "\n");
    while (line_ptr != NULL) {
        strcpy(line, line_ptr);
        token = strtok(line, delim);
        strcpy(tm->trf[line_num][0], token); // Estado actual

        for (int i = 1; i < 5; ++i) {
            token = strtok(NULL, delim);
            strcpy(tm->trf[line_num][i], token); // Leer, escribir, dirección, próximo estado
        }

        printf("%s\n", line_ptr);
        line_ptr = strtok(NULL, "\n");
        line_num++;
    }
    printf("---------------------\n");
}

// Función para realizar un solo paso en la cinta
void shiftOneStep(TuringMachine *tm, int i) {
    if (strcmp(tm->state, "H") != 0) { // Si no está en estado de Halt (H)
        char a = tm->tape[tm->head]; // Leer valor en la posición actual de la cabeza
        for (int j = 0; j < 100; j++) { // Buscar la transición correspondiente
            if (strcmp(tm->trf[j][0], tm->state) == 0 && tm->trf[j][1][0] == a) {
                char r = tm->trf[j][2][0]; // Valor a escribir
                char d = tm->trf[j][3][0]; // Dirección (r, l, o *)
                strcpy(tm->state, tm->trf[j][4]); // Próximo estado

                tm->tape[tm->head] = r; // Escribir en la cinta

                if (d == 'r') {
                    tm->head++;
                } else if (d == 'l') {
                    tm->head--;
                }
                
                printf("%d %s %s\n", i + 1, tm->tape, tm->state);
                printf("---------\n");
                break;
            }
        }
    }
}

// Función para ejecutar la máquina de Turing
void execute(TuringMachine *tm, const char *first_bin_num, const char *second_bin_num, int max_iter) {
    printf("\ni in   state\n------------\n");
    int i = 0;
    while (strcmp(tm->state, "H") != 0 && i < max_iter) {
        shiftOneStep(tm, i);
        i++;
    }

    // Mostrar el resultado
    char res[1000];
    strncpy(res, tm->tape, strlen(tm->tape));
    for (int j = 0; j < strlen(res); j++) {
        if (res[j] == '_') {
            res[j] = '\0'; // Limpiar guiones bajos de la cinta
        }
    }
    printf("\nRESULT\n");
    printf("%s + %s = %s (binary)\n", first_bin_num, second_bin_num, res);
    printf("%d + %d = %d (decimal)\n", (int)strtol(first_bin_num, NULL, 2), (int)strtol(second_bin_num, NULL, 2), (int)strtol(res, NULL, 2));
}

int main() {
    // Solicitar input al usuario
    printf("Adding two binary numbers x + y\n");
    char first_bin_num[100], second_bin_num[100];
    printf("x = ");
    scanf("%s", first_bin_num);
    printf("y = ");
    scanf("%s", second_bin_num);
    
    char input[200];
    snprintf(input, sizeof(input), "%s_%s", first_bin_num, second_bin_num);

    // Leer la tabla de estados internos desde el archivo
    FILE *file = fopen("add_2_binary_numbers_2.txt", "r");
    if (!file) {
        perror("Error al abrir el archivo.");
        return 1;
    }
    
    char algo[1000];
    fread(algo, sizeof(char), 1000, file);
    fclose(file);

    // Inicializar y ejecutar la máquina de Turing
    TuringMachine tm;
    initTuringMachine(&tm, algo, input, 0);
    execute(&tm, first_bin_num, second_bin_num, 10000);

    // Liberar memoria
    free(tm.tape);

    return 0;
}
