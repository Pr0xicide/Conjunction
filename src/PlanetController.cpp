#include "stdafx.h"

#include "controllers.h"
#include "Player.h"
#include "PlanetPlayer.h"
#include "gamestates.h"
#include "Game.h"

namespace capstone {

	/**
	 * PlanetController constructor.
	 */
	PlanetController::PlanetController(PlanetPlayer* p) 
		: PlayerController()
		, player(p)
	{

	}

	/**
	 * PlanetController destructor.
	 */
	PlanetController::~PlanetController() {

	}


// =====================================================================================
//
// #Keyboard Events
//
// =====================================================================================

	/**
	 *
	 */
	void PlanetController::onKeyPressed(const OIS::KeyEvent& arg) {

		// Goto the space level.
		if (arg.key == OIS::KC_E) {
			std::cout << "Planet Level >> Space Level" << std::endl;
			Game::getInstance()->switchState(SpaceGameplayState::getInstance());
		}
		
		// Switch from the planet level to the room.
		else if(arg.key == OIS::KC_Q) {
			Game::getInstance()->switchState(RoomGameplayState::getInstance());
		}

	}

	/**
	 *
	 */
	void PlanetController::onKeyReleased(const OIS::KeyEvent& arg) {

	}

}
