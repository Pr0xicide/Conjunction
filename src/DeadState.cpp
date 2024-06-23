#include "stdafx.h"

#include "DeadState.h"
#include "StateMachineBase.h"

namespace capstone {
	
	/**
	 * DeadState constructor.
	 */
	DeadState::DeadState(int num) {
		EnemyType = (EnemyTypes) num;
		AIState = (AIStates) 5;
	}

	/**
	 * DeadState destructor.
	 */
	DeadState::~DeadState() {
	
	}

	void DeadState::ExecuteState(Enemy* enemy,Ogre::Real deltaT){
		//Animate the explosion
		
		// has the explosion ended 
		if(false){
			enemy->mState = new DieState(0);
		}else if(true) {
			enemy->mState = new DeadState(0);
		}

	}
	
}

