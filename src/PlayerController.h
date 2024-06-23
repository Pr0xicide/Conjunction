#ifndef capstone_PLAYERCONTROLLER_H_
#define capstone_PLAYERCONTROLLER_H_

#include "Player.h"
#include "SpacePlayer.h"
#include "PlanetPlayer.h"

namespace capstone {

	/**
	 * This class will handle the controls for the player depending where the player is.
	 */
	class PlayerController {

	public:

		PlayerController();
		virtual ~PlayerController();

	};

}

#endif