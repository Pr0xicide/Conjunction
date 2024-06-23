#include "stdafx.h"

#include "AnimationState.h"
#include "PlayerShootState.h"

namespace capstone {

	/**
	 * PlayerShootState constructor.
	 *
	 * @param entity			entity object to get the animations state
	 * @param name				name of the animation that will loaded
	 *
	 */
	PlayerShootState::PlayerShootState(Ogre::Entity* entity) 
		: AnimationState(entity, "Shoot")
	{
		// Set animation state properities.
		state->setLoop(false);
		state->setEnabled(false);
	}

	/**
	 * PlayerShootState destructor.
	 */
	PlayerShootState::~PlayerShootState() {

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
	void PlayerShootState::update(Ogre::Real deltaT) {	
		state->addTime(deltaT);
	}


// =====================================================================================
//
// #Dying
//
// =====================================================================================

	/**
	 * PlayerDyingState constructor.
	 *
	 * @param entity			entity object to get the animations state
	 * @param name				name of the animation that will loaded
	 *
	 */
	PlayerDyingState::PlayerDyingState(Ogre::Entity* entity) 
		: AnimationState(entity, "Die")
	{
		// Set animation state properities.
		state->setLoop(false);
		state->setEnabled(false);
	}

	/**
	 * PlayerShootState destructor.
	 */
	PlayerDyingState::~PlayerDyingState() {

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
	void PlayerDyingState::update(Ogre::Real deltaT) {	
		state->addTime(deltaT);
	}


}