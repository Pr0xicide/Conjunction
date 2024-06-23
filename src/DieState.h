#ifndef capstone_DIESTATE_H_
#define capstone_DIESTATE_H_

#include "Enemy.h"

namespace capstone {
	class StateMachineBase;
	class DieState : public StateMachineBase {

	public:
		// Private constructor and destructor (Singleton design pattern).
		DieState(int num);
		~DieState(); // Memory purposes

		void ExecuteState(Enemy* enemy,Ogre::Real deltaT);
		
	};

}

#endif