#include "fsm.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    uint8_t timeout = 0;
    printf("insert bomb timeout: ");
    scanf( "%d", &timeout);

    uint8_t defuse_code = 0;
    printf("insert bomb defuse code: ");
    scanf("%d", &defuse_code);

    bomb_fsm bomb;
    bomb_fsm_ctor(&bomb, 3, defuse_code);  

    state_table_init(&bomb.super);

    uint8_t exit = 0;
    while (!exit)
    {
        int c = getc(stdin);
        switch (c)
        {
            case 'u':
                state_table_dispatch((state_table*)&bomb, &up_event);
                break;
            case 'd':
                state_table_dispatch((state_table*)&bomb, &down_event);
                break;
            case 'a':
                state_table_dispatch((state_table*)&bomb, &arm_event);
                break;
            case 't':
                state_table_dispatch((state_table*)&bomb, (event*)&tick_event);
                break;
        }
    }

    return EXIT_SUCCESS;
}