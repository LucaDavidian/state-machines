#include "fsm.h"
#include <stdio.h>

void bomb_fsm_ctor(bomb_fsm *bomb, uint8_t defuse_code, uint8_t timeout)
{
    static state_function state_functions[MAX_STATES][MAX_EVENTS] = 
    {
        setting_up, setting_down, setting_arm, state_table_empty_state_function,
        ticking_up, ticking_down, ticking_arm, ticking_tick
    };

    state_table_ctor(&bomb->super, &state_functions[0][0], MAX_STATES, MAX_EVENTS, initial);

    bomb->defuse_code = defuse_code;
    bomb->timeout = timeout;
}

void initial(state_table *st, event *ev)
{
    (void)ev;
    bomb_fsm *bomb = ((bomb_fsm*)st);

    TRANSITION(SETTING_STATE);
    printf("set timeout\n");
    printf("timeout: %d\n", bomb->timeout);
}

void setting_up(state_table *st, event *ev)
{
    (void)ev;
    bomb_fsm *bomb = ((bomb_fsm*)st);
    
    if (bomb->timeout < 60)
        bomb->timeout++;

    printf("timeout: %d\n", bomb->timeout);
}

void setting_down(state_table *st, event *ev)
{
    (void)ev;
    bomb_fsm *bomb = ((bomb_fsm*)st);
    
    if (bomb->timeout > 1)
        bomb->timeout--;

    printf("timeout: %d\n", bomb->timeout);
}

void setting_arm(state_table *st, event *ev)
{
    (void)ev;
    bomb_fsm *bomb = ((bomb_fsm*)st);
    TRANSITION(TICKING_STATE);
    printf("bomb armed!\n");
    printf("ticking.... timeout: %d\n", bomb->timeout);
}

void ticking_up(state_table *st, event *ev)
{
    (void)ev;

    bomb_fsm *bomb = ((bomb_fsm*)st);
    bomb->code <<= 1;
    bomb->code |= 1;
    printf("code is: %d\n", bomb->code);
}

void ticking_down(state_table *st, event *ev)
{
    (void)ev;

    bomb_fsm *bomb = ((bomb_fsm*)st);
    bomb->code <<= 1;
    printf("code is: %d\n", bomb->code);
}

void ticking_arm(state_table *st, event *ev)
{
    (void)ev;
    bomb_fsm *bomb = ((bomb_fsm*)st);

    printf("ticking.... timeout: %d\n", bomb->timeout);

    if (bomb->code == bomb->defuse_code)
    {
        TRANSITION(SETTING_STATE);
        printf("bomb defused! pfeeewwwwww...\n");
    }
}

void ticking_tick(state_table *st, event *ev)
{
    ticking_event *event = (ticking_event*)ev;
    bomb_fsm *bomb = ((bomb_fsm*)st);

    --event->fine_time;

    if (event->fine_time == 0)
    {
        bomb->timeout--;
        event->fine_time = 5;
        if (bomb->timeout == 0)
            printf("bomb exploded! BOOOOOOM!!!");
    }

    printf("ticking.... timeout: %d.%d\n", bomb->timeout, event->fine_time);
}

event up_event = { PRESS_UP_EVENT };
event down_event = { PRESS_DOWN_EVENT };
event arm_event = { ARM_EVENT };
ticking_event tick_event = { { TICK_EVENT }, 3 };