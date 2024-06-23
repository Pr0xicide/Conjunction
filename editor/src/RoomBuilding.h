#ifndef capstone_ROOMBUILDING_H_
#define capstone_ROOMBUILDING_H_

#include "InteractiveBuilding.h"

namespace capstone {

	/**
	 * Class definition of the equipment building where the player can equip parts.
	 */
	class EquipmentBuilding : public InteractiveBuilding {

	public:

		// Constructor and Destructor
		EquipmentBuilding(PlanetLevel* level);
		~EquipmentBuilding();

	};

}

#endif