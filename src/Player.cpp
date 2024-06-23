#include "stdafx.h"

#include "Player.h"

namespace capstone {

	/** 
	 * Player constructor.
	 */
	Player::Player() {

	}

	/** 
	 * Player destructor.
	 */
	Player::~Player() {

		// Destory animation state machine.
		for (unsigned i = 0; i < animList.size(); i++) 
			delete animList[i];

	}


//======================================================================================
//
// #Animations
//
//======================================================================================

	/**
	 * This function will change the robot's animation state.
	 *
	 * @param state				new animation state to enter
	 *
	 */
	void Player::changeAnimation(AnimationState* state) {

		// Refuse to re-enter into the same animation state.
		if (animState == state) 
			return;

		animState->exit();
		animState = state;
		animState->enter();
	}

}