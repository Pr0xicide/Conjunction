#ifndef capstone_PLAYERIDLESTATE_H_
#define capstone_PLAYERIDLESTATE_H_

#include "AnimationState.h"

namespace capstone {

	/**
	 * Class definition for the player's idle animation state.
	 */
	class PlayerIdleState : public AnimationState {

	public:

		// Constructor & destructor.
		PlayerIdleState(Ogre::Entity* entity);
		virtual ~PlayerIdleState();

		void update(Ogre::Real deltaT);

	};

}

#endif