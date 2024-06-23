#include "stdafx.h"

#include "DieState.h"
#include "StateMachineBase.h"

namespace capstone {
	
	/**
	 * DieState constructor.
	 */
	DieState::DieState(int num) {
		EnemyType = (EnemyTypes) num;
		AIState = (AIStates) 5;
	}

	/**
	 * DieState destructor.
	 */
	DieState::~DieState() {
	
	}

	void DieState::ExecuteState(Enemy* enemy,Ogre::Real deltaT){
		//Animate the explosion
		enemy->Die();
		//enemy->mState = new DeadState(0);
		/*// has the explosion ended 
		if(false){
			enemy->mState = new DieState(0);
		}else if(true) {
			enemy->mState = new DeadState(0);
		}*/

	}
	
}

