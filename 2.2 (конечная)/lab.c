#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "structures.h"
#include "func.h"
#include "simulation.h"

int main(int argc, char* argv[]) {
    int queue_impl = 0; // по умолчанию список
    if (argc > 1) {
        if (strcmp(argv[1], "array") == 0) {
            queue_impl = 1;
        } else if (strcmp(argv[1], "list") == 0) {
            queue_impl = 0;
        } else {
            printf("Usage: %s [list|array]\n", argv[0]);
            return 1;
        }
    } else {
        printf("Select queue implementation: 0 for list, 1 for array: ");
        scanf("%d", &queue_impl);
        while (getchar() != '\n'); // очистка буфера после scanf
    }

    printf("Enter patients (format: [*]id/ta/ts ...):\n");
    PatientData* patients = NULL;
    int count = 0;
    Error err = read_patients(&patients, &count);
    if (err.error_char) {
        check_error(err, NULL);
        return 1;
    }
    if (count == 0) {
        printf("No patients entered.\n");
        return 0;
    }
    qsort(patients, count, sizeof(PatientData), cmp_patient);
    SimulationState state;
    init_state(&state, patients, count, queue_impl);
    err = simulate(&state);
    if (err.error_char) {
        check_error(err, &state.waiting);
        free_state(&state);
        free(patients);
        return 1;
    }
    free_state(&state);
    free(patients);
    return 0;
}