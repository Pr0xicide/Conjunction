#include "stdafx.h"

#include "ReallocState.h"
#include "GlobalVariables.h"
#include "SpacePlayer.h"
#include "ReallocStates.h"

namespace capstone{

	ReallocStates::ReallocStates(){

		r1 = new ReallocState();
		r2 = new ReallocState();
		r3 = new ReallocState();
	}

	// Destructor
	ReallocStates::~ReallocStates()
	{
		delete r1;
		delete r2;
		delete r3;
	}

	ReallocStates* ReallocStates::getInstance(){
		static ReallocStates reallocStates;
		return &reallocStates;
	}
}