#ifndef _H_TEXTCONTROLLER
#define _H_TEXTCONTROLLER

#include "AppModel.h"
#include "BaseState.h"

enum {
	kTEXTCONTROLLER_INIT,
	kTEXTCONTROLLER_READY
};

class TextController : public BaseState
{
public:
    TextController();
    ~TextController();

    void registerStates();

    void update();

protected:

private:


};

#endif // _H_TEXTCONTROLLER
