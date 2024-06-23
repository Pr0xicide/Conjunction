#ifndef capstone_FACTORYBUILDING_H_
#define capstone_FACTORYBUILDING_H_

#include "InteractiveBuilding.h"

namespace capstone {

	/**
	 * Class definition for the factory building where the player can make weapons.
	 */
	class FactoryBuilding : public InteractiveBuilding {

	public:

		// Constructor and Destructor
		FactoryBuilding(PlanetLevel* level);
		~FactoryBuilding();

	};

}

#endif