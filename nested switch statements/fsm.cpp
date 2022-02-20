/* finite state machine - nested switch statements implementation */
/* fsm reads characters from C source file and counts characters in comment lines */

#define _CRT_SECURE_NO_WARNINGS
enum State
{
	CODE, SLASH, STAR, COMMENT,
};

enum Event
{
	CHAR_EV, SLASH_EV, STAR_EV, EOF_EV,
};

class FSM
{
public:
	FSM() : commentCtr(0) {}
	void init();
	void dispatch(Event event);
	unsigned int getCommentCount() const { return commentCtr; }
private:
	State state;
	unsigned int commentCtr;
	void transition(State state);
};

#include <cstdio>

Event GetEvent(FILE *file)
{
	int i = 0;
	if ((i = getc(file)) != EOF)
	{
		switch(i)
		{
			case '/':
				return SLASH_EV;
			case '*':
				return STAR_EV;
			default:
				return CHAR_EV;
		}	
	}
	else
		return EOF_EV;
}

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

	Event event;
	do
	{
		event = GetEvent(file);
		fsm.dispatch(event);
	} while (event != EOF_EV);

	printf("comment chars count: %u", fsm.getCommentCount());
	
	if (fclose(file))
	{
		printf("error: can't close file %s", argv[1]);
		return 3;
	}	
	
	return 0;
}

void FSM::init()
{
	state = CODE;
}

void FSM::transition(State state)
{
	this->state = state;
}

// event dispatcher is a big nested switch statement
void FSM::dispatch(Event event)
{
	switch(state)
	{
		case CODE:
			switch(event)
			{
				case CHAR_EV:			
					break;
				case SLASH_EV:
					transition(SLASH);
					break;
				case STAR_EV:
					break;
			}
			break;
		case SLASH:
			switch(event)
			{
				case CHAR_EV:			
				case SLASH_EV:
					transition(CODE);
					break;
				case STAR_EV:
					commentCtr += 2;
					transition(COMMENT);
					break;
			}
			break;
		case COMMENT:
			switch(event)
			{
				case CHAR_EV:
				case SLASH_EV:
					++commentCtr;
					break;
				case STAR_EV:
					++commentCtr;
					transition(STAR);
					break;
			}	
			break;
		case STAR:
			switch(event)
			{
				case CHAR_EV:
					++commentCtr;
					transition(COMMENT);
					break;
				case SLASH_EV:
					++commentCtr;
					transition(CODE);
					break;
				case STAR_EV:
					++commentCtr;
					break;
			}
			break;		
	}
}
