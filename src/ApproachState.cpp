#include "stdafx.h"

#include "ApproachState.h"
#include "StateMachineBase.h"

namespace capstone {
	
	/**
	 * ApproachState constructor.
	 */
	ApproachState::ApproachState(int num) {
		EnemyType = (EnemyTypes) num;
		AIState = (AIStates) 4;
	}

	/**
	 * ApproachState destructor.
	 */
	ApproachState::~ApproachState() {
	
	}

	void ApproachState::ExecuteState(Enemy* enemy, Ogre::Real deltaT){
		//enemy->Approach(deltaT);

		//// Enemy in range Attack
		//if(enemy->canFire()){
		//	enemy->mState = new AttackState(0);
		//}else{
		//	enemy->mState = new ApproachState(0);
		//}

		if(!enemy->isPlayerAlive() || !enemy->canDetectPlayer()){
			enemy->mState = new IdleState(0);
		}
		else if(enemy->canFire()){
			enemy->mState = new AttackState(0);
		}
		else{
			enemy->Approach(deltaT);
		}

	}
	
}

