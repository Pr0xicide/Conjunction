#ifndef capstone_WANDERSTATE_H_
#define capstone_WANDERSTATE_H_

#include "Enemy.h"

namespace capstone {
	class StateMachineBase;
	class WanderState : public StateMachineBase {

	public:
		// Private constructor and destructor (Singleton design pattern).
		WanderState(int num);
		~WanderState(); // Memory purposes

		void ExecuteState(Enemy* enemy,Ogre::Real deltaT);
		
	};

}

#endif