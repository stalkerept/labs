#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum {
    ERR_OK = 0,
    ERR_MEMORY_PUSH = 1,
    ERR_INVALID_INPUT = 2,
    ERR_FGETS = 3,
    ERR_MEMORY_PATIENTS = 4,
    ERR_TOKEN_COUNT_MISMATCH = 5,
    ERR_MEMORY_HANDLE_FINISH = 6,
    ERR_MEMORY_HANDLE_ARRIVAL = 7,
    ERR_MEMORY_ENSURE_CAPACITY = 8
} ErrorNumber;

typedef struct {  
    char j_a;
    char id[100];
    int ta;
    int ts;
} Info;

struct Person {
    Info man;
    struct Person* next;
};

typedef struct {
    int impl; // 0 - список, 1 - массив
    union {
        struct {
            struct Person* top;
            struct Person* bottom;
        } list;
        struct {
            Info* data;
            int capacity;
            int front;
            int back;
            int size;
        } array;
    } u;
} Queue;

typedef struct {
    char error_char;
    ErrorNumber number;
} Error;

typedef struct {
    Info info;
    int order;
} PatientData;

typedef struct {
    PatientData* patients;   // массив всех пациентов (отсортированный)
    int count;               // количество пациентов
    int next_idx;            // индекс следующего ещё не прибывшего пациента
    int current_time;        // текущее время симуляции
    Info* current;           // текущий обслуживаемый пациент (NULL если никого)
    int finish_time;         // время окончания обслуживания текущего
    Queue waiting;           // очередь ожидающих
} SimulationState;

#endif