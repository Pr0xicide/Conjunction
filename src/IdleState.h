#ifndef capstone_IDLESTATE_H_
#define capstone_IDLESTATE_H_

#include "Enemy.h"

namespace capstone {
	class StateMachineBase;
	class IdleState : public StateMachineBase {


	public:
		// Private constructor and destructor (Singleton design pattern).
		IdleState(int num);
		~IdleState(); // Memory purposes
		void ExecuteState(Enemy* enemy,Ogre::Real deltaT);
		
	};

}

#endif