#ifndef STATE_TABLE_H
#define STATE_TABLE_H

/********************* REUSABLE GENERAL EVENT PROCESSOR ********************************/

#include <stdint.h>
#include <assert.h>

#define CHECK_VALID_STATE  assert(st->current_state < st->num_states)  // macro checks for valid transition to initial state
#define CHECK_VALID_SIGNAL assert(ev->signal < st->num_events)         // macro checks if signal is in range
#define TRANSITION(state) (st->current_state = (uint8_t)state)        // state transistion macro

typedef struct event_tag
{
    uint8_t signal;
} event;

typedef struct state_table_tag state_table;

typedef void (*state_function)(state_table *st, event *e);

struct state_table_tag
{
    state_function *state_table;
    uint8_t num_states;
    uint8_t num_events;
    state_function initial;
    uint8_t current_state;
};

void state_table_ctor(state_table *st, state_function *state_table, uint8_t num_states, uint8_t num_events, state_function initial);
void state_table_init(state_table *st);
void state_table_dispatch(state_table *st, event *ev);
void state_table_empty_state_function(state_table *st, event *ev);

#endif  // STATE_TABLE_H