/*
 *  OscModel.h
 *  Bunyip
 *
 *  Created by gameover on 1/09/11.
 *  Copyright 2011 trace media. All rights reserved.
 *
 */

#ifndef _H_OSCMODEL
#define _H_OSCMODEL

#include "AppModel.h"

class OscModel {

public:

	OscModel() {
		LOG_NOTICE("Constructing OscModel");
	};

	~OscModel() {
		LOG_NOTICE("Destructing OscModel");
	};

	// INT

	void registerEvent(string address, int arg1, osc_ct comparisonType, string helpMessage, string functionToCall) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_BOTH_INTERNAL_USE, helpMessage, functionToCall, -1, -1);
			return;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams0<OscMessage> inputParams(oscMessage);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, int arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, param1, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, param1, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, int arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, bool variableIsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, &param1, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, &param1, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, &param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, int arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1, Param2 param2) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, int arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 param2, bool parameter1IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, int arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 &param2, bool parameter1IsPointer, bool parameter2IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, &param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	// FLOAT

	void registerEvent(string address, float arg1, osc_ct comparisonType, string helpMessage, string functionToCall) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_BOTH_INTERNAL_USE, helpMessage, functionToCall, -1.0f, -1.0f);
			return;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams0<OscMessage> inputParams(oscMessage);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, float arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, param1, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, param1, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, float arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, bool variableIsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, &param1, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, &param1, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, &param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, float arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1, Param2 param2) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, float arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 param2, bool parameter1IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, float arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 &param2, bool parameter1IsPointer, bool parameter2IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, &param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	// STRING

	void registerEvent(string address, string arg1, osc_ct comparisonType, string helpMessage, string functionToCall) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_BOTH_INTERNAL_USE, helpMessage, functionToCall, (string)"", (string)"");
			return;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams0<OscMessage> inputParams(oscMessage);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, string arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, param1, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, param1, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, string arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, bool variableIsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, &param1, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, &param1, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, &param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, string arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1, Param2 param2) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, string arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 param2, bool parameter1IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, string arg1, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 &param2, bool parameter1IsPointer, bool parameter2IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, &param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	// INTS

	void registerEvent(string address, int arg1, int arg2, osc_ct comparisonType, string helpMessage, string functionToCall) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_BOTH_INTERNAL_USE, helpMessage, functionToCall, -1, -1);
			return;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams0<OscMessage> inputParams(oscMessage);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, int arg1, int arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, param1, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, param1, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, int arg1, int arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, bool variableIsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, &param1, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, &param1, -1);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, &param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, int arg1, int arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1, Param2 param2) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, int arg1, int arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 param2, bool parameter1IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, int arg1, int arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 &param2, bool parameter1IsPointer, bool parameter2IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, &param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	// FLOATS

	void registerEvent(string address, float arg1, float arg2, osc_ct comparisonType, string helpMessage, string functionToCall) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_BOTH_INTERNAL_USE, helpMessage, functionToCall, -1.0f, -1.0f);
			return;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams0<OscMessage> inputParams(oscMessage);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, float arg1, float arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, param1, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, param1, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, float arg1, float arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, bool variableIsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, &param1, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, &param1, -1.0f);
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, &param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, float arg1, float arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1, Param2 param2) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, float arg1, float arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 param2, bool parameter1IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, float arg1, float arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 &param2, bool parameter1IsPointer, bool parameter2IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, &param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	// STRINGS

	void registerEvent(string address, string arg1, string arg2, osc_ct comparisonType, string helpMessage, string functionToCall) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_BOTH_INTERNAL_USE, helpMessage, functionToCall, (string)"", (string)"");
			return;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams0<OscMessage> inputParams(oscMessage);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, string arg1, string arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, param1, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, param1, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1>
	void registerEvent(string address, string arg1, string arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, bool variableIsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_ONE_INTERNAL_USE, helpMessage, functionToCall, &param1, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
			registerEvent(address, arg1, arg2, kOSC_PASS_PARAM_TWO_INTERNAL_USE, helpMessage, functionToCall, &param1, (string)"");
			return;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass both param1, byteOne AND bytTwo as well!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams1<OscMessage, Param1> inputParams(oscMessage, &param1);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, string arg1, string arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 param1, Param2 param2) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, string arg1, string arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 param2, bool parameter1IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

	template<class Param1, class Param2>
	void registerEvent(string address, string arg1, string arg2, osc_ct comparisonType, string helpMessage, string functionToCall, Param1 &param1, Param2 &param2, bool parameter1IsPointer, bool parameter2IsPointer) {

		if (comparisonType == kOSC_PASS_PARAM_ONE || comparisonType == kOSC_PASS_PARAM_TWO || comparisonType == kOSC_PASS_PARAM_BOTH) {
			LOG_ERROR("Maximum of two parameters, so can't pass the midi values as well as two params!!");
			abort();
		} else if (comparisonType == kOSC_PASS_PARAM_ONE_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_ONE;
		} else if (comparisonType == kOSC_PASS_PARAM_TWO_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_TWO;
		} else if (comparisonType == kOSC_PASS_PARAM_BOTH_INTERNAL_USE) {
			comparisonType = kOSC_PASS_PARAM_BOTH;
		}

		OscMessage oscMessage(address, arg1, arg2, (double)0, comparisonType);
		InputParams2<OscMessage, Param1, Param2> inputParams(oscMessage, &param1, &param2);
		registerInputToFunction(functionToCall, inputParams, helpMessage);
	}

//	void unregisterEvent(string uniqueFunctionID) {
//		map<string, DelegateMemento>::iterator it = _functionModel->_registeredFunctions.find(uniqueFunctionID);
//
//		if (it != _functionModel->_registeredFunctions.end()) {
//
//			LOG_VERBOSE("Deleting function map for: " + uniqueFunctionID);
//
//
//
//			//TODO: delete usage!!
//			//storeUsage(inputParameters.getMessageMap(), usage);
//
//		} else {
//
//			LOG_VERBOSE("No function registered for: " + uniqueFunctionID);
//			abort();
//
//		}
//	}

	template <class InputParams>
	void registerInputToFunction(string uniqueFunctionID, InputParams inputParameters, string usage = "") {

		map<string, DelegateMemento>::iterator it = _functionModel->_registeredFunctions.find(uniqueFunctionID);

		if (it != _functionModel->_registeredFunctions.end()) {

			LOG_VERBOSE("Adding function map for: " + uniqueFunctionID);
			MappedDelegate<InputParams> md(uniqueFunctionID, it->second, inputParameters);
			setMappedDelegate(inputParameters.getMessageMap(), md);

			if (usage == "") {
				int nParams = inputParameters.getNumParams();
				ostringstream params;
				switch (nParams) {
					case 0:
						params << "()";
						break;
					case 1:
						params << "(" << inputParameters.getParam1() << ")";
						break;
					case 2:
						params << "(" << inputParameters.getParam1() << ", " << inputParameters.getParam1() << ")";
						break;
				}

				usage = uniqueFunctionID + params.str();
			}

			storeUsage(inputParameters.getMessageMap(), usage);

		} else {

			LOG_VERBOSE("No function registered for: " + uniqueFunctionID);
			abort();

		}

	}

	string getAllOscUsageAsList(bool log = false) {

		ostringstream out;
		for (int i = 0; i < _oscUsage.size(); i++) {
			out << _oscUsage[i] << endl;
		}

		if (log) {
			LOG_NOTICE(out.str());
		}

		return out.str();

	};

	void executeFunction(OscMessage oscMessage) {

		//oscMessage.print(true);

		map<OscMessage, DelegateType>::iterator it_type;

		it_type = _mappedOscType.find(oscMessage);

		bool comparisonMatched = false;
		OscMessage originalOscMessage = it_type->first;
		//originalOscMessage.print(true);
		osc_ct comparisonType = originalOscMessage.getComparisonType();

		if (it_type != _mappedOscType.end()) {
			switch (comparisonType) {
				case kOSC_ANY:
					comparisonMatched = true;
					break;
				case kOSC_PASS_PARAM_ONE:
				case kOSC_PASS_PARAM_TWO:
				case kOSC_PASS_PARAM_BOTH:
					comparisonMatched = true;
					break;
				case kOSC_EQUAL:
					if (originalOscMessage == oscMessage) comparisonMatched = true;
					break;
				case kOSC_GREATER:
					if (originalOscMessage > oscMessage) comparisonMatched = true;
					break;
				case kOSC_LESSER:
					if (originalOscMessage < oscMessage) comparisonMatched = true;
					break;
				case kOSC_EQUAL_GREATER:
					if (originalOscMessage >= oscMessage) comparisonMatched = true;
					break;
				case kOSC_EQUAL_LESSER:
					if (originalOscMessage <= oscMessage) comparisonMatched = true;
					break;
					// TODO: Ranges!!
			}
		}

		if (comparisonMatched) {

			DelegateType dt = it_type->second;

			switch (dt) {
				case GO_DELEGATE_NONE:
				{
					map<OscMessage, MappedDelegate<InputParams0<OscMessage>			> >	 ::iterator	it0;
					it0  = _mappedOscParams0.find(oscMessage);
					DelegateMemento dm = it0->second.getDelegateMemento();
					Delegate0 funcToExec;
					funcToExec.SetMemento(dm);
					funcToExec();
					break;
				}
				case GO_DELEGATE_STRING:
				{
					map<OscMessage, MappedDelegate<InputParams1<OscMessage, string	> > >::iterator	it1s;
					it1s = _mappedOscParamsString1.find(oscMessage);
					DelegateMemento dm = it1s->second.getDelegateMemento();
					DelegateString1 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(oscMessage.getArgString1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(oscMessage.getArgString2());
					} else funcToExec(it1s->second.getInputParams().getParam1());
					break;
				}
				case GO_DELEGATE_INT:
				{
					map<OscMessage, MappedDelegate<InputParams1<OscMessage, int		> > >::iterator	it1i;
					it1i = _mappedOscParamsInt1.find(oscMessage);
					DelegateMemento dm = it1i->second.getDelegateMemento();
					DelegateInt1 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(oscMessage.getArgInt1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(oscMessage.getArgInt2());
					} else funcToExec(it1i->second.getInputParams().getParam1());
					break;
				}
				case GO_DELEGATE_FLOAT:
				{
					map<OscMessage, MappedDelegate<InputParams1<OscMessage, float	> > >::iterator	it1f;
					it1f = _mappedOscParamsFloat1.find(oscMessage);
					DelegateMemento dm = it1f->second.getDelegateMemento();
					DelegateFloat1 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						cout << oscMessage.getArgFloat1() << endl;
						funcToExec(oscMessage.getArgFloat1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(oscMessage.getArgFloat2());
					} else funcToExec(it1f->second.getInputParams().getParam1());
					break;
				}
				case GO_DELEGATE_BOOL:
				{
					map<OscMessage, MappedDelegate<InputParams1<OscMessage, bool	> > >::iterator	it1b;
					it1b = _mappedOscParamsBool1.find(oscMessage);
					DelegateMemento dm = it1b->second.getDelegateMemento();
					DelegateBool1 funcToExec;
					funcToExec.SetMemento(dm);
					funcToExec(it1b->second.getInputParams().getParam1());
					break;
				}
				case GO_DELEGATE_STRING_STRING:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, string, string > > >::iterator	it2ss;
					it2ss = _mappedOscParamsStringString2.find(oscMessage);
					DelegateMemento dm = it2ss->second.getDelegateMemento();
					DelegateStringString2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2ss->second.getInputParams().getParam1(), oscMessage.getArgString1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2ss->second.getInputParams().getParam1(), oscMessage.getArgString2());
					} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
						funcToExec(oscMessage.getArgString1(), oscMessage.getArgString2());
					} else funcToExec(it2ss->second.getInputParams().getParam1(), it2ss->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_INT_INT:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, int, int > > >::iterator	it2ii;
					it2ii = _mappedOscParamsIntInt2.find(oscMessage);
					DelegateMemento dm = it2ii->second.getDelegateMemento();
					DelegateIntInt2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2ii->second.getInputParams().getParam1(), oscMessage.getArgInt1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2ii->second.getInputParams().getParam1(), oscMessage.getArgInt2());
					} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
						funcToExec(oscMessage.getArgInt1(), oscMessage.getArgInt2());
					} else funcToExec(it2ii->second.getInputParams().getParam1(), it2ii->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_FLOAT_FLOAT:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, float, float > > >::iterator	it2ff;
					it2ff = _mappedOscParamsFloatFloat2.find(oscMessage);
					DelegateMemento dm = it2ff->second.getDelegateMemento();
					DelegateFloatFloat2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2ff->second.getInputParams().getParam1(), oscMessage.getArgFloat1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2ff->second.getInputParams().getParam1(), oscMessage.getArgFloat2());
					} else if (comparisonType == kOSC_PASS_PARAM_BOTH) {
						funcToExec(oscMessage.getArgFloat1(), oscMessage.getArgFloat2());
					} else funcToExec(it2ff->second.getInputParams().getParam1(), it2ff->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_BOOL_BOOL:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, bool, bool > > >::iterator	it2bb;
					it2bb = _mappedOscParamsBoolBool2.find(oscMessage);
					DelegateMemento dm = it2bb->second.getDelegateMemento();
					DelegateBoolBool2 funcToExec;
					funcToExec.SetMemento(dm);
					funcToExec(it2bb->second.getInputParams().getParam1(), it2bb->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_STRING_INT:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, string, int > > >::iterator	it2si;
					it2si = _mappedOscParamsStringInt2.find(oscMessage);
					DelegateMemento dm = it2si->second.getDelegateMemento();
					DelegateStringInt2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2si->second.getInputParams().getParam1(), oscMessage.getArgInt1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2si->second.getInputParams().getParam1(), oscMessage.getArgInt2());
					} else funcToExec(it2si->second.getInputParams().getParam1(), it2si->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_STRING_FLOAT:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, string, float > > >::iterator	it2sf;
					it2sf = _mappedOscParamsStringFloat2.find(oscMessage);
					DelegateMemento dm = it2sf->second.getDelegateMemento();
					DelegateStringFloat2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2sf->second.getInputParams().getParam1(), oscMessage.getArgFloat1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2sf->second.getInputParams().getParam1(), oscMessage.getArgFloat2());
					} else funcToExec(it2sf->second.getInputParams().getParam1(), it2sf->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_STRING_BOOL:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, string, bool > > >::iterator	it2sb;
					it2sb = _mappedOscParamsStringBool2.find(oscMessage);
					DelegateMemento dm = it2sb->second.getDelegateMemento();
					DelegateStringBool2 funcToExec;
					funcToExec.SetMemento(dm);
					funcToExec(it2sb->second.getInputParams().getParam1(), it2sb->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_INT_STRING:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, int, string > > >::iterator	it2is;
					it2is = _mappedOscParamsIntString2.find(oscMessage);
					DelegateMemento dm = it2is->second.getDelegateMemento();
					DelegateIntString2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2is->second.getInputParams().getParam1(), oscMessage.getArgString1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2is->second.getInputParams().getParam1(), oscMessage.getArgString2());
					} else funcToExec(it2is->second.getInputParams().getParam1(), it2is->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_INT_FLOAT:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, int, float > > >::iterator	it2if;
					it2if = _mappedOscParamsIntFloat2.find(oscMessage);
					DelegateMemento dm = it2if->second.getDelegateMemento();
					DelegateIntFloat2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2if->second.getInputParams().getParam1(), oscMessage.getArgFloat1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2if->second.getInputParams().getParam1(), oscMessage.getArgFloat2());
					} else funcToExec(it2if->second.getInputParams().getParam1(), it2if->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_INT_BOOL:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, int, bool > > >::iterator	it2ib;
					it2ib = _mappedOscParamsIntBool2.find(oscMessage);
					DelegateMemento dm = it2ib->second.getDelegateMemento();
					DelegateIntBool2 funcToExec;
					funcToExec.SetMemento(dm);
					funcToExec(it2ib->second.getInputParams().getParam1(), it2ib->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_FLOAT_STRING:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, float, string > > >::iterator	it2fs;
					it2fs = _mappedOscParamsFloatString2.find(oscMessage);
					DelegateMemento dm = it2fs->second.getDelegateMemento();
					DelegateFloatString2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2fs->second.getInputParams().getParam1(), oscMessage.getArgString1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2fs->second.getInputParams().getParam1(), oscMessage.getArgString2());
					} else funcToExec(it2fs->second.getInputParams().getParam1(), it2fs->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_FLOAT_INT:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, float, int > > >::iterator	it2fi;
					it2fi = _mappedOscParamsFloatInt2.find(oscMessage);
					DelegateMemento dm = it2fi->second.getDelegateMemento();
					DelegateFloatInt2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2fi->second.getInputParams().getParam1(), oscMessage.getArgInt1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2fi->second.getInputParams().getParam1(), oscMessage.getArgInt2());
					} else funcToExec(it2fi->second.getInputParams().getParam1(), it2fi->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_FLOAT_BOOL:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, float, bool > > >::iterator	it2fb;
					it2fb = _mappedOscParamsFloatBool2.find(oscMessage);
					DelegateMemento dm = it2fb->second.getDelegateMemento();
					DelegateFloatBool2 funcToExec;
					funcToExec.SetMemento(dm);
					funcToExec(it2fb->second.getInputParams().getParam1(), it2fb->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_BOOL_STRING:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, bool, string > > >::iterator	it2bs;
					it2bs = _mappedOscParamsBoolString2.find(oscMessage);
					DelegateMemento dm = it2bs->second.getDelegateMemento();
					DelegateBoolString2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2bs->second.getInputParams().getParam1(), oscMessage.getArgString1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2bs->second.getInputParams().getParam1(), oscMessage.getArgString2());
					} else funcToExec(it2bs->second.getInputParams().getParam1(), it2bs->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_BOOL_INT:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, bool, int > > >::iterator	it2bi;
					it2bi = _mappedOscParamsBoolInt2.find(oscMessage);
					DelegateMemento dm = it2bi->second.getDelegateMemento();
					DelegateBoolInt2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2bi->second.getInputParams().getParam1(), oscMessage.getArgInt1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2bi->second.getInputParams().getParam1(), oscMessage.getArgInt2());
					} else funcToExec(it2bi->second.getInputParams().getParam1(), it2bi->second.getInputParams().getParam2());
					break;
				}
				case GO_DELEGATE_BOOL_FLOAT:
				{
					map<OscMessage, MappedDelegate<InputParams2<OscMessage, bool, float > > >::iterator	it2bf;
					it2bf = _mappedOscParamsBoolFloat2.find(oscMessage);
					DelegateMemento dm = it2bf->second.getDelegateMemento();
					DelegateBoolFloat2 funcToExec;
					funcToExec.SetMemento(dm);
					if (comparisonType == kOSC_PASS_PARAM_ONE) {
						funcToExec(it2bf->second.getInputParams().getParam1(), oscMessage.getArgFloat1());
					} else if (comparisonType == kOSC_PASS_PARAM_TWO) {
						funcToExec(it2bf->second.getInputParams().getParam1(), oscMessage.getArgFloat2());
					} else funcToExec(it2bf->second.getInputParams().getParam1(), it2bf->second.getInputParams().getParam2());
					break;
				}
			}
		}

	}

private:

	void storeUsage(OscMessage oscMessage, string usage) {

		string c = oscMessage.getAddress();
		ostringstream usestream;

		usestream << "'" << c << "'" << " - " << usage;

		usage = usestream.str();

		LOG_VERBOSE("Created osc usage: " + usage);

		_oscUsage.push_back(usage);
	}

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams0<OscMessage> > md) {
		LOG_VERBOSE("Store p0");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_NONE));
		_mappedOscParams0.insert(pair<OscMessage, MappedDelegate< InputParams0<OscMessage> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams1<OscMessage, string> > md) {
		LOG_VERBOSE("Store p1 string");
		//oscMessage.print(true);
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_STRING));
		_mappedOscParamsString1.insert(pair< OscMessage, MappedDelegate< InputParams1< OscMessage, string> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams1<OscMessage, float> > md) {
		LOG_VERBOSE("Store p1 float");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_FLOAT));
		_mappedOscParamsFloat1.insert(pair< OscMessage, MappedDelegate<InputParams1<OscMessage, float> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams1<OscMessage, int> > md) {
		LOG_VERBOSE("Store p1 int");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_INT));
		_mappedOscParamsInt1.insert(pair< OscMessage, MappedDelegate< InputParams1<OscMessage, int> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams1<OscMessage, bool> > md) {
		LOG_VERBOSE("Store p1 bool");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_BOOL));
		_mappedOscParamsBool1.insert(pair< OscMessage, MappedDelegate<InputParams1<OscMessage, bool> >	> (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, string, string> > md) {
		LOG_VERBOSE("Store p2 string string");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_STRING_STRING));
		_mappedOscParamsStringString2.insert(pair< OscMessage, MappedDelegate< InputParams2< OscMessage, string, string> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, string, float> > md) {
		LOG_VERBOSE("Store p2 string float");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_STRING_FLOAT));
		_mappedOscParamsStringFloat2.insert(pair< OscMessage, MappedDelegate<InputParams2<OscMessage, string, float> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, string, int> > md) {
		LOG_VERBOSE("Store p2 string int");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_STRING_INT));
		_mappedOscParamsStringInt2.insert(pair< OscMessage, MappedDelegate< InputParams2<OscMessage, string, int> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, string, bool> > md) {
		LOG_VERBOSE("Store p2 string bool");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_STRING_BOOL));
		_mappedOscParamsStringBool2.insert(pair< OscMessage, MappedDelegate<InputParams2<OscMessage, string, bool> >	> (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, float, float> > md) {
		LOG_VERBOSE("Store p2 float float");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_FLOAT_FLOAT));
		_mappedOscParamsFloatFloat2.insert(pair< OscMessage, MappedDelegate< InputParams2< OscMessage, float, float> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, float, string> > md) {
		LOG_VERBOSE("Store p2 float string");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_FLOAT_STRING));
		_mappedOscParamsFloatString2.insert(pair< OscMessage, MappedDelegate<InputParams2<OscMessage, float, string> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, float, int> > md) {
		LOG_VERBOSE("Store p2 float int");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_FLOAT_INT));
		_mappedOscParamsFloatInt2.insert(pair< OscMessage, MappedDelegate< InputParams2<OscMessage, float, int> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, float, bool> > md) {
		LOG_VERBOSE("Store p2 float bool");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_FLOAT_BOOL));
		_mappedOscParamsFloatBool2.insert(pair< OscMessage, MappedDelegate<InputParams2<OscMessage, float, bool> >	> (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, int, int> > md) {
		LOG_VERBOSE("Store p2 int int");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_INT_INT));
		_mappedOscParamsIntInt2.insert(pair< OscMessage, MappedDelegate< InputParams2< OscMessage, int, int> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, int, string> > md) {
		LOG_VERBOSE("Store p2 int string");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_INT_STRING));
		_mappedOscParamsIntString2.insert(pair< OscMessage, MappedDelegate<InputParams2<OscMessage, int, string> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, int, float> > md) {
		LOG_VERBOSE("Store p2 int float");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_INT_FLOAT));
		_mappedOscParamsIntFloat2.insert(pair< OscMessage, MappedDelegate< InputParams2<OscMessage, int, float> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, int, bool> > md) {
		LOG_VERBOSE("Store p2 int bool");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_INT_BOOL));
		_mappedOscParamsIntBool2.insert(pair< OscMessage, MappedDelegate<InputParams2<OscMessage, int, bool> >	> (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, bool, bool> > md) {
		LOG_VERBOSE("Store p2 bool bool");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_BOOL_BOOL));
		_mappedOscParamsBoolBool2.insert(pair< OscMessage, MappedDelegate< InputParams2< OscMessage, bool, bool> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, bool, string> > md) {
		LOG_VERBOSE("Store p2 bool string");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_BOOL_STRING));
		_mappedOscParamsBoolString2.insert(pair< OscMessage, MappedDelegate<InputParams2<OscMessage, bool, string> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, bool, float> > md) {
		LOG_VERBOSE("Store p2 bool float");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_BOOL_FLOAT));
		_mappedOscParamsBoolFloat2.insert(pair< OscMessage, MappedDelegate< InputParams2<OscMessage, bool, float> > > (oscMessage, md));
	};

	void setMappedDelegate(OscMessage oscMessage, MappedDelegate<InputParams2<OscMessage, bool, int> > md) {
		LOG_VERBOSE("Store p2 bool int");
		_mappedOscType.insert(pair<OscMessage, DelegateType> (oscMessage, GO_DELEGATE_BOOL_INT));
		_mappedOscParamsBoolInt2.insert(pair< OscMessage, MappedDelegate<InputParams2<OscMessage, bool, int> >	> (oscMessage, md));
	};

	// making these public for now until i work out a nicer way to return
	map< OscMessage, DelegateType >													_mappedOscType;
	vector<string>																	_oscUsage;

	map< OscMessage, MappedDelegate<InputParams0<OscMessage> > >					_mappedOscParams0;

	map< OscMessage, MappedDelegate<InputParams1<OscMessage, string> > >			_mappedOscParamsString1;
	map< OscMessage, MappedDelegate<InputParams1<OscMessage, float> > >				_mappedOscParamsFloat1;
	map< OscMessage, MappedDelegate<InputParams1<OscMessage, int> > >				_mappedOscParamsInt1;
	map< OscMessage, MappedDelegate<InputParams1<OscMessage, bool> > >				_mappedOscParamsBool1;

	map< OscMessage, MappedDelegate<InputParams2<OscMessage, string, string> > >	_mappedOscParamsStringString2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, string, float> > >		_mappedOscParamsStringFloat2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, string, int> > >		_mappedOscParamsStringInt2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, string, bool> > >		_mappedOscParamsStringBool2;

	map< OscMessage, MappedDelegate<InputParams2<OscMessage, float, float> > >		_mappedOscParamsFloatFloat2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, float, string> > >		_mappedOscParamsFloatString2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, float, int> > >		_mappedOscParamsFloatInt2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, float, bool> > >		_mappedOscParamsFloatBool2;

	map< OscMessage, MappedDelegate<InputParams2<OscMessage, int, int> > >			_mappedOscParamsIntInt2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, int, string> > >		_mappedOscParamsIntString2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, int, float> > >		_mappedOscParamsIntFloat2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, int, bool> > >			_mappedOscParamsIntBool2;

	map< OscMessage, MappedDelegate<InputParams2<OscMessage, bool, bool> > >		_mappedOscParamsBoolBool2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, bool, string> > >		_mappedOscParamsBoolString2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, bool, float> > >		_mappedOscParamsBoolFloat2;
	map< OscMessage, MappedDelegate<InputParams2<OscMessage, bool, int> > >			_mappedOscParamsBoolInt2;

};

typedef Singleton<OscModel> OscModelSingleton;					// Global declaration

static OscModel * _oscModel	= OscModelSingleton::Instance();	// this is my idea of a compromise:
// no defines, keeps introspection
// but allows data sharing between classes

#endif
