#ifndef FSM_H
#define FSM_H

#include "state_table.h"

/********************* APPLICATION SPECIFIC CODE ********************************/

enum state
{
    SETTING_STATE, TICKING_STATE, MAX_STATES    
};

enum event 
{
    PRESS_UP_EVENT, PRESS_DOWN_EVENT, ARM_EVENT, TICK_EVENT, MAX_EVENTS
};

typedef struct ticking_event_tag
{
    event super;
    uint8_t fine_time;
} ticking_event;

typedef struct bomb_fsm_tag
{
    state_table super;  // base subobject
    uint8_t defuse_code;
    uint8_t code;
    uint8_t timeout;
} bomb_fsm;

void bomb_fsm_ctor(bomb_fsm *bomb, uint8_t defuse_code, uint8_t timeout);

/*** application specific state functions ***/

void initial(state_table *st, event *ev);
void setting_up(state_table *st, event *ev);
void setting_down(state_table *st, event *ev);
void setting_arm(state_table *st, event *ev);
void ticking_up(state_table *st, event *ev);
void ticking_down(state_table *st, event *ev);
void ticking_arm(state_table *st, event *ev);
void ticking_tick(state_table *st, event *ev);

extern event up_event;
extern event down_event;
extern event arm_event;
extern ticking_event tick_event;

#endif  // FSM_H