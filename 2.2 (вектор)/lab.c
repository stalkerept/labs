#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "structures.h"
#include "func.h"
#include "simulation.h"

int main(void) {
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
    init_state(&state, patients, count);
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