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
    ERR_MEMORY_HANDLE_ARRIVAL = 7
} ErrorNumber;

typedef struct 
{  
    char j_a;
    char id[100];
    int ta;
    int ts;
}Info;


struct Person{
    Info man;
    struct Person* next;
};

typedef struct
{
    struct Person* top;
    struct Person* bottom;
}Queue;

typedef struct
{
    char error_char;
    ErrorNumber number;
}Error;

typedef struct {
    Info info;
    int order;
} PatientData;


typedef struct {
    PatientData* patients;   // array of all patients (sorted)
    int count;               // number of patients
    int next_idx;            // index of the next patient who hasn't arrived yet
    int current_time;        // current simulation time
    Info* current;           // current patient being served (NULL if none)
    int finish_time;         // time when current patient will finish
    Queue waiting;           // queue of waiting patients
} SimulationState;

#endif