/* finite state machine - state table implementation */
/* fsm reads characters from C source file and counts characters in comment lines */

enum State
{
	CODE, SLASH, COMMENT, STAR, MAX_STATES,
};

enum Event
{
	CHAR_EV, SLASH_EV, STAR_EV, MAX_EVS, 
};

class StateTable
{
public:
	typedef void (StateTable::*Action)(void);
	struct Transition
	{
		Action action;
		State nextState;
	};
	StateTable(const Transition *table_, int numStates_, int numEvents_) : table(table_), numStates(numStates_), numEvents(numEvents_) {}
	virtual ~StateTable() {}
	void init();
	void dispatch(Event event);
private:
	State state;
	const Transition *table;
	int numStates;
	int numEvents;
};

class FSM : public StateTable
{
public:
	FSM() : StateTable(&table[0][0], MAX_STATES, MAX_EVS), commentCtr(0) {}
	unsigned int getCommentCount() const { return commentCtr; }
private:
	static const Transition table[MAX_STATES][MAX_EVS];  // StateTable::Transition 
	unsigned int commentCtr;
	void doNothing() {}
	void action1();
	void action2();
};

void StateTable::dispatch(Event event)
{
	const Transition *transition = table + event + state * numEvents;
	(this->*transition->action)();
	state = transition->nextState;
}

const StateTable::Transition FSM::table[MAX_STATES][MAX_EVS] = 
{
	              /*    CHAR_EV                                              SLASH_EV                                                  STAR_EV                                                 */
	/* CODE */    {{static_cast<StateTable::Action>(&FSM::doNothing), CODE},{static_cast<StateTable::Action>(&FSM::doNothing), SLASH},{static_cast<StateTable::Action>(&FSM::doNothing), CODE}},
	/* SLASH */   {{static_cast<StateTable::Action>(&FSM::doNothing), CODE},{static_cast<StateTable::Action>(&FSM::doNothing), CODE},{static_cast<StateTable::Action>(&FSM::action2), COMMENT}},
	/* COMMENT */ {{static_cast<StateTable::Action>(&FSM::action1), COMMENT},{static_cast<StateTable::Action>(&FSM::action1), COMMENT},{static_cast<StateTable::Action>(&FSM::action1), STAR}},	
	/* STAR */    {{static_cast<StateTable::Action>(&FSM::action1), COMMENT},{static_cast<StateTable::Action>(&FSM::action1), CODE},{static_cast<StateTable::Action>(&FSM::action1), STAR}},
};

void StateTable::init()
{
	state = CODE;
}

void FSM::action1()
{
	++commentCtr;
}

void FSM::action2()
{
	commentCtr += 2;
}

#include <cstdio>

int main(int argc, char *argv[])
{	

	if (argc < 2)
	{
		printf("usage: fsm filename");
		return 1;
	}
	
	FILE *file = fopen(argv[1], "r");
	if (file == 0)
	{
		printf("error: can't open file %s", argv[1]);
		return 2;
	}
	
	FSM fsm;
	fsm.init();

	int i = 0;
	while ((i = getc(file) ) != EOF)
	{
		switch(i)
		{
			case '/':
				fsm.dispatch(SLASH_EV);
				break;
			case '*':
				fsm.dispatch(STAR_EV);
				break;
			default:
				fsm.dispatch(CHAR_EV);
				break;
		}	
	}

	printf("comment chars count: %u", fsm.getCommentCount());
	
	if (fclose(file))
	{
		printf("error: can't close file %s", argv[1]);
		return 3;
	}	
	
	return 0;
}