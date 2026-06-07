#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "structures.h"
#include "func.h"

char check_error(Error error, Queue* queue) {
    if (error.error_char != 0) {
        if (queue != NULL) {
            clear_queue(queue);
        }
        printf("Error: %hhd/%d\n", error.error_char, error.number);
        switch (error.number) {
            case ERR_MEMORY_PUSH:
                printf("Memory error in push\n");
                break;
            case ERR_INVALID_INPUT:
                printf("Invalid input. Use format: [*]id/ta/ts\n");
                break;
            case ERR_FGETS:
                printf("Error with fgets in read_patients\n");
                break;
            case ERR_MEMORY_PATIENTS:
                printf("Memory error with patients in read_patients\n");
                break;
            case ERR_TOKEN_COUNT_MISMATCH:
                printf("Error: idx != token_count\n");
                break;
            case ERR_MEMORY_HANDLE_FINISH:
                printf("Memory allocation error in handle_finish\n");
                break;
            case ERR_MEMORY_HANDLE_ARRIVAL:
                printf("Memory allocation error in handle_arrivals\n");
                break;
            case ERR_MEMORY_ENSURE_CAPACITY:
                printf("Memory allocation error in ensure_capacity\n");
                break;
            default:
                printf("Unknown error code\n");
                break;
        }
        return 1;
    }
    return 0;
}

Error parse_patient(const char* token, Info* patient) {
    Error error = {0, ERR_OK};
    memset(patient, 0, sizeof(Info));
    char buffer[256];
    strcpy(buffer, token);
    char* token_start = buffer;
    if (buffer[0] == '*') {
        patient->j_a = 1;
        token_start++;
    } else {
        patient->j_a = 0;
    }
    char* id_token = NULL;
    char* ta_token = NULL;
    char* ts_token = NULL;
    int token_count = 0;
    char* part = strtok(token_start, "/");
    while (part != NULL) {
        token_count++;
        switch (token_count) {
            case 1: id_token = part; break;
            case 2: ta_token = part; break;
            case 3: ts_token = part; break;
            default: break;
        }
        part = strtok(NULL, "/");
    }
    if (token_count != 3) {
        error.error_char = 1;
        error.number = ERR_INVALID_INPUT;
        return error;
    }
    size_t id_len = strlen(id_token);
    if (id_len == 0 || id_len > 99) {
        error.error_char = 1;
        error.number = ERR_INVALID_INPUT;
        return error;
    }
    strcpy(patient->id, id_token);
    char* endptr;
    long val = strtol(ta_token, &endptr, 10);
    if (*endptr != '\0' || val < 0 || val > INT_MAX) {
        error.error_char = 1;
        error.number = ERR_INVALID_INPUT;
        return error;
    }
    patient->ta = (int)val;
    val = strtol(ts_token, &endptr, 10);
    if (*endptr != '\0' || val < 0 || val > INT_MAX) {
        error.error_char = 1;
        error.number = ERR_INVALID_INPUT;
        return error;
    }
    patient->ts = (int)val;
    return error;
}

int cmp_patient(const void* a, const void* b) {
    const PatientData* pa = (const PatientData*)a;
    const PatientData* pb = (const PatientData*)b;
    if (pa->info.ta != pb->info.ta)
        return pa->info.ta - pb->info.ta;
    return pa->order - pb->order;
}

void print_state(int time, Info* current, Queue* waiting) {
    printf("%d\n", time);
    if (current != NULL) {
        printf("%s", current->id);
        if (waiting->impl == 0) { // список
            struct Person* p = waiting->u.list.top;
            while (p != NULL) {
                printf(" %s", p->man.id);
                p = p->next;
            }
        } else { // массив
            for (int i = 0; i < waiting->u.array.size; i++) {
                Info* p = &waiting->u.array.data[(waiting->u.array.front + i) % waiting->u.array.capacity];
                printf(" %s", p->id);
            }
        }
    } else {
        if (!is_empty_queue(waiting)) {
            if (waiting->impl == 0) {
                struct Person* p = waiting->u.list.top;
                printf("%s", p->man.id);
                p = p->next;
                while (p != NULL) {
                    printf(" %s", p->man.id);
                    p = p->next;
                }
            } else {
                for (int i = 0; i < waiting->u.array.size; i++) {
                    Info* p = &waiting->u.array.data[(waiting->u.array.front + i) % waiting->u.array.capacity];
                    if (i == 0)
                        printf("%s", p->id);
                    else
                        printf(" %s", p->id);
                }
            }
        }
    }
    printf("\n");
}

Error read_patients(PatientData** patients, int* count) {
    Error error = {0, ERR_OK};
    char line[1024];
    if (!fgets(line, sizeof(line), stdin)) {
        error.error_char = 1;
        error.number = ERR_FGETS;
        return error;
    }
    line[strcspn(line, "\n")] = '\0';
    char line_copy[1024];
    strcpy(line_copy, line);
    char *saveptr1;
    char *token = strtok_r(line_copy, " ", &saveptr1);
    int token_count = 0;
    while (token) {
        token_count++;
        token = strtok_r(NULL, " ", &saveptr1);
    }

    if (token_count == 0) {
        *patients = NULL;
        *count = 0;
        return error;
    }

    *patients = (PatientData*)malloc(token_count * sizeof(PatientData));
    if (!*patients) {
        error.error_char = 1;
        error.number = ERR_MEMORY_PATIENTS;
        return error;
    }
    strcpy(line_copy, line);
    char *saveptr2;
    token = strtok_r(line_copy, " ", &saveptr2);
    int idx = 0;
    while (token) {
        Error parse_err = parse_patient(token, &(*patients)[idx].info);
        if (parse_err.error_char) {
            free(*patients);
            *patients = NULL;
            return parse_err;
        }
        (*patients)[idx].order = idx;
        idx++;
        token = strtok_r(NULL, " ", &saveptr2);
    }

    if (idx != token_count) {
        free(*patients);
        *patients = NULL;
        error.error_char = 1;
        error.number = ERR_TOKEN_COUNT_MISMATCH;
        return error;
    }

    *count = token_count;
    return error;
}