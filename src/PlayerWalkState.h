#ifndef capstone_PLAYERWALKSTATE_H_
#define capstone_PLAYERWALKSTATE_H_

#include "AnimationState.h"

namespace capstone {

	/**
	 * Class definition for the player's walk animation state.
	 */
	class PlayerWalkState : public AnimationState {

	public:

		// Constructor & destructor.
		PlayerWalkState(Ogre::Entity* entity);
		virtual ~PlayerWalkState();

		void update(Ogre::Real deltaT);

	};

}

#endif