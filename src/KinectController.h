#ifndef _H_KINECTCONTROLLER
#define _H_KINECTCONTROLLER

#include "AppModel.h"
#include "BaseState.h"

enum {
	kKINECTCONTROLLER_INIT,
	kKINECTCONTROLLER_READY
};

class KinectController : public BaseState{

public:

    KinectController();
    ~KinectController();

    void registerStates();

    void update();

protected:

    ofxOscReceiver oscReceiver;

private:

};

#endif
