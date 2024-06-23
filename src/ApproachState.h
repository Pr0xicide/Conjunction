#ifndef capstone_APPROACHSTATE_H_
#define capstone_APPROACHSTATE_H_

#include "Enemy.h"

namespace capstone {

	class StateMachineBase;
	class ApproachState : public StateMachineBase {

	public:
		// Private constructor and destructor (Singleton design pattern).
		ApproachState(int num);
		~ApproachState(); // Memory purposes

		void ExecuteState(Enemy* enemy, Ogre::Real deltaT);
		
	};

}

#endif