#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "structures.h"
#include "func.h"
#include "simulation.h"

void init_state(SimulationState* state, PatientData* patients, int count, int queue_impl) {
    state->patients = patients;
    state->count = count;
    state->next_idx = 0;
    state->current_time = (count > 0) ? patients[0].info.ta : 0;
    state->current = NULL;
    state->finish_time = 0;
    init_queue(&state->waiting, queue_impl);
}

void free_state(SimulationState* state) {
    if (state->current) {
        free(state->current);
        state->current = NULL;
    }
    clear_queue(&state->waiting);
}

int next_event_time(SimulationState* state) {
    int next_arrival = (state->next_idx < state->count) ? state->patients[state->next_idx].info.ta : INT_MAX;
    int next_finish = (state->current != NULL) ? state->finish_time : INT_MAX;
    return (next_arrival < next_finish) ? next_arrival : next_finish;
}

Error handle_finish(SimulationState* state) {
    Error err = {0, ERR_OK};
    if (state->current) {
        free(state->current);
        state->current = NULL;
    }
    print_state(state->current_time, NULL, &state->waiting);
    
    if (!is_empty_queue(&state->waiting)) {
        Info next_patient = pop_front_queue(&state->waiting);
        state->current = (Info*)malloc(sizeof(Info));
        if (!state->current) {
            err.error_char = 1;
            err.number = ERR_MEMORY_HANDLE_FINISH;
            return err;
        }
        *(state->current) = next_patient;
        state->finish_time = state->current_time + state->current->ts;
        print_state(state->current_time, state->current, &state->waiting);
    }
    return err;
}

Error handle_arrivals(SimulationState* state) {
    Error err = {0, ERR_OK};
    while (state->next_idx < state->count && 
           state->patients[state->next_idx].info.ta == state->current_time) {
        Info new_patient = state->patients[state->next_idx].info;
        if (state->current == NULL) {
            state->current = (Info*)malloc(sizeof(Info));
            if (!state->current) {
                err.error_char = 1;
                err.number = ERR_MEMORY_HANDLE_ARRIVAL;
                return err;
            }
            *(state->current) = new_patient;
            state->finish_time = state->current_time + new_patient.ts;
        } else {
            err = push_queue(&state->waiting, new_patient);
            if (err.error_char) {
                return err;
            }
        }
        print_state(state->current_time, state->current, &state->waiting);
        state->next_idx++;
    }
    return err;
}

Error simulate(SimulationState* state) {
    Error err = {0, ERR_OK};
    while (state->next_idx < state->count || state->current != NULL || !is_empty_queue(&state->waiting)) {
        int event_time = next_event_time(state);
        if (event_time == INT_MAX) break;
        state->current_time = event_time;
        
        if (state->current != NULL && state->finish_time == state->current_time) {
            err = handle_finish(state);
            if (err.error_char) return err;
        }
        
        err = handle_arrivals(state);
        if (err.error_char) return err;
    }
    return err;
}