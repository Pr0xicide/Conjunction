#include "stdafx.h"

#include "AttackState.h"
#include "StateMachineBase.h"

namespace capstone {
	
	/**
	 * AttackState constructor.
	 */
	AttackState::AttackState(int num) {
		EnemyType = (EnemyTypes) num;
		AIState = (AIStates) 3;
	}

	/**
	 * AttackState destructor.
	 */
	AttackState::~AttackState() {
	
	}

	void AttackState::ExecuteState(Enemy* enemy, Ogre::Real deltaT){
		/*enemy->Attack(deltaT);

		if(!enemy->isPlayerAlive()){
			enemy->mState = new IdleState(0);
		}else if (!enemy->canFire()){
			enemy->mState = new ApproachState(0);
		} else {
			enemy->mState = new AttackState(0);
		}*/

		if(!enemy->isPlayerAlive() || !enemy->canDetectPlayer()){
			enemy->mState = new IdleState(0);
		}
		else if(!enemy->canFire() && (enemy->canDetectPlayer() || enemy->canAssist())){
			enemy->mState = new ApproachState(0);
		}
		else if(enemy->canFire()){
			enemy->Attack(deltaT);
		}

	}
	
}

