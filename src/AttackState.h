#ifndef capstone_ATTACKSTATE_H_
#define capstone_ATTACKSTATE_H_

#include "Enemy.h"

namespace capstone {

	class StateMachineBase;
	class AttackState : public StateMachineBase {

	public:
		// Private constructor and destructor (Singleton design pattern).
		AttackState(int num);
		~AttackState(); // Memory purposes

		void ExecuteState(Enemy* enemy, Ogre::Real deltaT);
		
	};

}

#endif