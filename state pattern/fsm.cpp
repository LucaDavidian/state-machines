/* finite state machine - state design pattern implementation */
/* fsm reads characters from C source file and counts characters in comment lines */

class FSMContext;  // class declaration - incomplete type 

class AbstractState
{
public:
	/* virtual functions represent events/signals */
	virtual void onCHAR(FSMContext *context) = 0;
	virtual void onSLASH(FSMContext *context) = 0;
	virtual void onSTAR(FSMContext *context) = 0;
};

/* derived concrete classes represent states */
class CodeState : public AbstractState
{
	void onCHAR(FSMContext *context) override {}
	void onSLASH(FSMContext *context) override;
	void onSTAR(FSMContext *context) override {}
};

class SlashState : public AbstractState
{
	void onCHAR(FSMContext *context) override;
	void onSLASH(FSMContext *context) override;
	void onSTAR(FSMContext *context) override;
};

class CommentState : public AbstractState
{
	void onCHAR(FSMContext *context) override;
	void onSLASH(FSMContext *context) override;
	void onSTAR(FSMContext *context) override;
};

class StarState : public AbstractState
{
	void onCHAR(FSMContext *context) override;
	void onSLASH(FSMContext *context) override;
	void onSTAR(FSMContext *context) override;
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
	void onCHAR() { state->onCHAR(this); }
	void onSLASH() { state->onSLASH(this); }
	void onSTAR() { state->onSTAR(this); }
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
void CodeState::onSLASH(FSMContext *context)
{
	context->state = &FSMContext::slashState;
}

void SlashState::onCHAR(FSMContext *context)
{
	context->state = &FSMContext::codeState;
}

void SlashState::onSLASH(FSMContext *context)
{
	context->state = &FSMContext::codeState;
}

void SlashState::onSTAR(FSMContext *context)
{
	context->commentCounter++;
	context->commentCounter++;
	context->state = &FSMContext::commentState;
}

void CommentState::onCHAR(FSMContext *context)
{
	context->commentCounter++;
}

void CommentState::onSLASH(FSMContext *context)
{
	context->commentCounter++;
}

void CommentState::onSTAR(FSMContext *context)
{
	context->commentCounter++;
	context->state = &FSMContext::starState;
}

void StarState::onCHAR(FSMContext *context)
{
	context->commentCounter++;
	context->state = &FSMContext::commentState;
}

void StarState::onSLASH(FSMContext *context)
{
	context->commentCounter++;
	context->state = &FSMContext::codeState;
}

void StarState::onSTAR(FSMContext *context)
{
	context->commentCounter++;
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
				fsm.onSLASH();
				break;
			case '*':
				fsm.onSTAR();
				break;
			default:
				fsm.onCHAR();
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