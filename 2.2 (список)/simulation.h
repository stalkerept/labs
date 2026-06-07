#ifndef SIMULATION_H
#define SIMULATION_H

#include "structures.h"
#include "func.h"

void init_state(SimulationState* state, PatientData* patients, int count);
void free_state(SimulationState* state);
int next_event_time(SimulationState* state);
Error handle_finish(SimulationState* state);
Error handle_arrivals(SimulationState* state);
Error simulate(SimulationState* state);

#endif