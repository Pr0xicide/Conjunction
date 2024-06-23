#ifndef capstone_LAUNCHBUILDING_H_
#define capstone_LAUNCHBUILDING_H_

#include "InteractiveBuilding.h"

namespace capstone {

	/**
	 * Class definition for the launch building where the player can goto space.
	 */
	class LaunchBuilding : public InteractiveBuilding {

	public:

		LaunchBuilding(PlanetLevel* level, Ogre::Vector3 position);
		virtual ~LaunchBuilding();

		// Events.
		void interact();

		// Getters.
		Ogre::String	getQuestion()		{ return "Enter Outer Space?"; }

	private:

		void initProperties()	{ InteractiveBuilding::id = 3; InteractiveBuilding::name = "Launch Pad"; }

	};
	
}

#endif