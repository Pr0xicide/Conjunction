#include "stdafx.h"

#include "AnimationState.h"
#include "PlayerIdleState.h"

namespace capstone {

	/**
	 * PlayerIdleState constructor.
	 *
	 * @param entity			entity object to get the animations state
	 * @param name				name of the animation that will loaded
	 *
	 */
	PlayerIdleState::PlayerIdleState(Ogre::Entity* entity) 
		: AnimationState(entity, "Idle")
	{
		// Set animation state properities.
		state->setLoop(true);
		state->setEnabled(false);
	}

	/**
	 * PlayerIdleState destructor.
	 */
	PlayerIdleState::~PlayerIdleState() {

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
	void PlayerIdleState::update(Ogre::Real deltaT) {	
		state->addTime(deltaT * 0.25f);
	}

}