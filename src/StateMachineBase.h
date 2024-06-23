#ifndef capstone_STATEMACHINEBASE_H_
#define capstone_STATEMACHINEBASE_H_

namespace capstone{
	class Enemy;
	class StateMachineBase {

	protected:
		enum EnemyTypes
		{
			EnemyType_Invalid = -1,	// Checking if EnemyType is messed up
			EnemyType_Base = 0, //Regular Enemy By Default
			EnemyType_Suicidal = 1, //Enemy is Suicidal to kill themselves
			EnemyType_Powerup = 3,  // Enemy Support unit allows Other Enemy to shoot more projectiles
		} EnemyType;

		enum AIStates
		{
			AIStates_Invalid = -1,	//Checking if AIState is messed up
			AIStates_Idle = 0,		//IDLE state 0
			AIStates_Wander,		//Wander state 1
			AIStates_Runnaway,		//Runnaway state 2
			AIStates_Attack,		//Attack state 3
			AIStates_Approach,		//Approach state 4
			AIStates_Die,			//Die state 5
			AIStates_Dead,			//Dead state 6
			AIStates_Gethelp,		//GetHelp state 7
		} AIState;
	public:
		//Getting the Setting the Current State
		virtual void ExecuteState(Enemy* enemy , Ogre::Real deltaT) = 0;
	};
}

#endif