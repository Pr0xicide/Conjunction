#include "stdafx.h"

#include "AnimationState.h"
#include "PlayerWalkState.h"

namespace capstone {

	/**
	 * PlayerWalkState constructor.
	 *
	 * @param entity			entity object to get the animations state
	 * @param name				name of the animation that will loaded
	 *
	 */
	PlayerWalkState::PlayerWalkState(Ogre::Entity* entity) 
		: AnimationState(entity, "Walk")
	{

		// Set animation state properities.
		state->setLoop(true);
		state->setEnabled(false);

	}

	/**
	 * PlayerWalkState destructor.
	 */
	PlayerWalkState::~PlayerWalkState() {

	}


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/** 
	 * This function updates the animation state.
	 *
	 * @param deltaT			time since last frame
	 *
	 */
	void PlayerWalkState::update(Ogre::Real deltaT) {	
		state->addTime(deltaT);
	}

}