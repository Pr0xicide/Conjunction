#ifndef capstone_PLAYERSHOOTSTATE_H_
#define capstone_PLAYERSHOOTSTATE_H_

#include "AnimationState.h"

namespace capstone {

	/**
	 * Class definition for the player's shoot animation state.
	 */
	class PlayerShootState : public AnimationState {
	
	public:

		// Constructor & destructor.
		PlayerShootState(Ogre::Entity* entity);
		virtual ~PlayerShootState();

		void update(Ogre::Real deltaT);

	};

	/**
	 * Class definition for the player's dying animation state.
	 */
	class PlayerDyingState : public AnimationState {
	
	public:

		// Constructor & destructor.
		PlayerDyingState(Ogre::Entity* entity);
		virtual ~PlayerDyingState();

		void update(Ogre::Real deltaT);

	};

}

#endif