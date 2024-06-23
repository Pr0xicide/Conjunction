#ifndef capstone_LAUNCHBUILDING_H_
#define capstone_LAUNCHBUILDING_H_

#include "InteractiveBuilding.h"

namespace capstone {

	/**
	 * Class definition for the launch building where the player can goto space.
	 */
	class LaunchBuilding : public InteractiveBuilding {

	public:

		LaunchBuilding(PlanetLevel* level);
		virtual ~LaunchBuilding();

	};
	
}

#endif