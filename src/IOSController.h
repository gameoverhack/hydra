#ifndef _H_IOSCONTROLLER
#define _H_IOSCONTROLLER

#include "AppModel.h"
#include "BaseState.h"

enum {
	kIOSCONTROLLER_INIT,
	kIOSCONTROLLER_READY
};

class IOSController : public BaseState
{
public:
    IOSController();
    ~IOSController();

    void registerStates();

    void update();

protected:

private:


};

#endif // _H_IOSController
