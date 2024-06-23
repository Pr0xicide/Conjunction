#include "stdafx.h"

#include "AnimationState.h"

namespace capstone {

	/**
	 * AnimationState constructor.
	 *
	 * @param entity			entity object to get the animations state
	 * @param name				name of the animation that will loaded
	 *
	 */
	AnimationState::AnimationState(Ogre::Entity* entity, Ogre::String name) 
		: entity(entity)
		, name(name)
	{
		// Setup the animation state based off the name.
		state = entity->getAnimationState(name);
	}

	/**
	 * AnimationState destructor.
	 */
	AnimationState::~AnimationState() {

	}

// =====================================================================================
//
// #Switching
//
// =====================================================================================

	/**
	 * This function will enable this animation state.
	 */
	void AnimationState::enter() {
		state->setEnabled(true);
	}

	/**
	 * This function will disable this animation state.
	 */
	void AnimationState::exit() {
		state->setEnabled(false);
	}

}