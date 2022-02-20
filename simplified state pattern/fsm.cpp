/* finite state machine - state design pattern implementation */
/* fsm reads characters from C source file and counts characters in comment lines */

class FSMContext;  // class declaration - incomplete type 

enum Event 
{
	CHAR_EV, SLASH_EV, STAR_EV,
};

class AbstractState
{
public:
	/* virtual functions represent events/signals */
	virtual void dispatch(FSMContext *context, Event event) = 0;
};

/* derived concrete classes represent states */
class CodeState : public AbstractState
{
	void dispatch(FSMContext *context, Event event) override;
};

class SlashState : public AbstractState
{
	void dispatch(FSMContext *context, Event event) override;
};

class CommentState : public AbstractState
{
	void dispatch(FSMContext *context, Event event) override;
};

class StarState : public AbstractState
{
	void dispatch(FSMContext *context, Event event) override;
};

/* context class handles events through delegation */
class FSMContext
{
friend class CodeState;
friend class SlashState;
friend class CommentState;
friend class StarState;
public:
	FSMContext() : commentCounter(0) {}
	void init();
	unsigned int getCommentCount() const { return commentCounter; }
	/* context class delegates event handling to AbstactState class */
	void dispatch(Event event) { state->dispatch(this, event); }
private:
	AbstractState *state;
	unsigned int commentCounter;
	static CodeState codeState;
	static SlashState slashState;
	static CommentState commentState;
	static StarState starState;
};	

/* static members initialization */
CodeState FSMContext::codeState;
SlashState FSMContext::slashState;
CommentState FSMContext::commentState;
StarState FSMContext::starState;

/* FSMContext class function definitions */
void FSMContext::init()
{
	state = &codeState;
}

/* function definitions for concrete state classes */
void CodeState::dispatch(FSMContext *context, Event event)
{
	switch(event)
	{
		case CHAR_EV:
			break;
		case SLASH_EV:
			context->state = &FSMContext::slashState;
			break;
		case STAR_EV:
			break;
	}
}

void SlashState::dispatch(FSMContext *context, Event event)
{
	switch(event)
	{
		case CHAR_EV:
			context->state = &FSMContext::codeState;
			break;
		case SLASH_EV:
			context->state = &FSMContext::codeState;
			break;
		case STAR_EV:
			context->commentCounter++;
			context->commentCounter++;
			context->state = &FSMContext::commentState;
			break;
	}
}

void CommentState::dispatch(FSMContext *context, Event event)
{
	switch(event)
	{
		case CHAR_EV:
			context->commentCounter++;
			break;
		case SLASH_EV:
			context->commentCounter++;
			break;
		case STAR_EV:
			context->commentCounter++;
			context->state = &FSMContext::starState;
			break;
	}
}

void StarState::dispatch(FSMContext *context, Event event)
{
	switch(event)
	{
		case CHAR_EV:
			context->commentCounter++;
			break;
		case SLASH_EV:
			context->commentCounter++;
			context->state = &FSMContext::codeState;
			break;
		case STAR_EV:
			context->commentCounter++;
			break;
	}
}

/* main application */
#include <cstdio>

int main(int argc, char **argv)
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
	
	FSMContext fsm;
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