#ifndef capstone_DEADSTATE_H_
#define capstone_DEADSTATE_H_

#include "Enemy.h"

namespace capstone {

	class StateMachineBase;
	class DeadState : public StateMachineBase {


	public:
		// Private constructor and destructor (Singleton design pattern).
		DeadState(int num);
		~DeadState(); // Memory purposes
		void ExecuteState(Enemy* enemy,Ogre::Real deltaT);
		
	};

}

#endif