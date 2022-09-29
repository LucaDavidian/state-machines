#include "state_table.h"

void state_table_ctor(state_table *st, state_function *state_table, uint8_t num_states, uint8_t num_events, state_function initial)
{
    st->state_table = state_table;
    st->num_states = num_states;
    st->num_events = num_events;
    st->initial = initial;
    st->current_state = num_states;  // init state out of range
}

void state_table_init(state_table *st)
{
    st->initial(st, NULL);   // no event on initial transition 
    //(*st->initial)(st, NULL);
    
    CHECK_VALID_STATE;
}

void state_table_dispatch(state_table *st, event *ev)
{
    CHECK_VALID_SIGNAL;

    st->state_table[st->current_state * st->num_events + ev->signal](st, ev);
    
    CHECK_VALID_STATE;
}

void state_table_empty_state_function(state_table *st, event *ev)
{
    (void)st;    // suppress compiler warning
    (void)ev;    // ditto
}
