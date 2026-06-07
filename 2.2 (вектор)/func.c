#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include <ctype.h>
#include <string.h>
#include <limits.h>

int is_empty_queue(Queue* queue) {
    return queue->size == 0;
}

static Error ensure_capacity(Queue* queue, int needed) {
    Error err = {0, ERR_OK};
    if (queue->capacity >= needed) {
        return err;
    }
    int new_capacity = (queue->capacity == 0) ? 4 : queue->capacity * 2;
    while (new_capacity < needed) {
        new_capacity *= 2;
    }
    Info* new_data = (Info*)malloc(new_capacity * sizeof(Info));
    if (!new_data) {
        err.error_char = 1;
        err.number = ERR_MEMORY_ENSURE_CAPACITY;
        return err;
    }
    for (int i = 0; i < queue->size; i++) {
        new_data[i] = queue->data[(queue->front + i) % queue->capacity];
    }
    free(queue->data);
    queue->data = new_data;
    queue->capacity = new_capacity;
    queue->front = 0;
    queue->back = queue->size;
    return err;
}

Error push_back(Queue* queue, Info value) {
    Error err = ensure_capacity(queue, queue->size + 1);
    if (err.error_char) return err;
    queue->data[queue->back] = value;
    queue->back = (queue->back + 1) % queue->capacity;
    queue->size++;
    return err;
}

Error push_front(Queue* queue, Info value) {
    Error err = ensure_capacity(queue, queue->size + 1);
    if (err.error_char) return err;
    queue->front = (queue->front - 1 + queue->capacity) % queue->capacity;
    queue->data[queue->front] = value;
    queue->size++;
    return err;
}

Info pop_front(Queue* queue) {
    // Предполагается, что очередь не пуста
    Info value = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    if (queue->size == 0) {
        queue->front = 0;
        queue->back = 0;
    }
    return value;
}

void clear_queue(Queue* queue) {
    if (queue->data) {
        free(queue->data);
        queue->data = NULL;
    }
    queue->capacity = 0;
    queue->front = 0;
    queue->back = 0;
    queue->size = 0;
}

void print_queue(Queue* queue) {
    printf("Queue (size=%d):\n", queue->size);
    for (int i = 0; i < queue->size; i++) {
        Info* p = &queue->data[(queue->front + i) % queue->capacity];
        printf("  %s (ta=%d, ts=%d, j_a=%d)\n", p->id, p->ta, p->ts, p->j_a);
    }
}

char check_error(Error error, Queue* queue) {
    if (error.error_char != 0) {
        if (queue != NULL) {
            clear_queue(queue);
        }
        printf("Error: %hhd/%d\n", error.error_char, error.number);
        switch (error.number) {
            case ERR_MEMORY_PUSH:
            case ERR_MEMORY_ENSURE_CAPACITY:
                printf("Memory error in queue operation\n");
                break;
            case ERR_INVALID_INPUT:
                printf("Invalid input. Use format: [*]id/ta/ts\n");
                break;
            case ERR_FGETS:
                printf("Error reading input line\n");
                break;
            case ERR_MEMORY_PATIENTS:
                printf("Memory allocation failed for patients array\n");
                break;
            case ERR_TOKEN_COUNT_MISMATCH:
                printf("Token count mismatch during parsing\n");
                break;
            case ERR_MEMORY_HANDLE_FINISH:
                printf("Memory allocation failed in handle_finish\n");
                break;
            case ERR_MEMORY_HANDLE_ARRIVAL:
                printf("Memory allocation failed in handle_arrivals\n");
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
    while (part) {
        token_count++;
        switch (token_count) {
            case 1: id_token = part; break;
            case 2: ta_token = part; break;
            case 3: ts_token = part; break;
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
    if (*endptr || val < 0 || val > INT_MAX) {
        error.error_char = 1;
        error.number = ERR_INVALID_INPUT;
        return error;
    }
    patient->ta = (int)val;
    val = strtol(ts_token, &endptr, 10);
    if (*endptr || val < 0 || val > INT_MAX) {
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
        for (int i = 0; i < waiting->size; i++) {
            Info* p = &waiting->data[(waiting->front + i) % waiting->capacity];
            printf(" %s", p->id);
        }
    } else {
        if (waiting->size > 0) {
            Info* p = &waiting->data[waiting->front];
            printf("%s", p->id);
            for (int i = 1; i < waiting->size; i++) {
                p = &waiting->data[(waiting->front + i) % waiting->capacity];
                printf(" %s", p->id);
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