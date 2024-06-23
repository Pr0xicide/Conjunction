#ifndef capstone_PLANETCONTROLLER_H_
#define capstone_PLANETCONTROLLER_H_

#include "PlayerController.h"

namespace capstone {

	class PlanetPlayer;

	/**
	 * This class will handle the controls for the player while on the planet.
	 */
	class PlanetController : public PlayerController  {

		PlanetPlayer* player;

	public:

		PlanetController(PlanetPlayer* p);
		virtual ~PlanetController();

		void onKeyPressed(const OIS::KeyEvent& arg);
		void onKeyReleased(const OIS::KeyEvent& arg);
		//void onMouseMoved(Player* p);
		//void onMouseDown(Player* p);
		//void onMouseUp(Player* p);

	};

}

#endif