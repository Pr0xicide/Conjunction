#include "stdafx.h"

#include "IdleState.h"
#include "StateMachineBase.h"

namespace capstone {
	
	/**
	 * IdleState constructor.
	 */
	IdleState::IdleState(int num) {
		EnemyType = (EnemyTypes) num;
		AIState = (AIStates) 0;
	}

	/**
	 * IdleState destructor.
	 */
	IdleState::~IdleState() {
	
	}

	void IdleState::ExecuteState(Enemy* enemy,Ogre::Real deltaT){
		/*enemy->Idle(deltaT);

		if(enemy->canDetectPlayer()){
			enemy->mState = new AttackState(0);
		}else{
			enemy->mState = new WanderState(0);
		}*/

		/*if(enemy->canDetectPlayer() || enemy->canAssist()){
			enemy->mState = new ApproachState(0);
		}
		else if(enemy->canFire()){
			enemy->mState = new AttackState(0);
		}
		else if(rand()%10 <= 8){
			enemy->mState = new WanderState(0);
		}
		else{
			enemy->Idle(deltaT);
		}*/
		enemy->mState = new WanderState(0);
	}
	
}

