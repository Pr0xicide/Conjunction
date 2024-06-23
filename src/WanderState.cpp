#include "stdafx.h"

#include "WanderState.h"
#include "StateMachineBase.h"

namespace capstone {
	
	/**
	 * WanderState constructor.
	 */
	WanderState::WanderState(int num) {
		EnemyType = (EnemyTypes) num;
		AIState = (AIStates) 1;
	}

	/**
	 * WanderState destructor.
	 */
	WanderState::~WanderState() {
	
	}

	void WanderState::ExecuteState(Enemy* enemy,Ogre::Real deltaT){
		//AC enemy->canDetectPlayer()
		if(enemy->canSeePlayer() /*|| enemy->canAssist()*/){
			enemy->mState = new ApproachState(0);
		}
		else if(enemy->canFire()){
			enemy->mState = new AttackState(0);
		}
		/*else if(rand()%10 > 8){
			enemy->mState = new IdleState(0);
		}*/
		else{
			enemy->Wander(deltaT);
		}

	}
	
}

