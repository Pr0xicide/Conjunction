#ifndef capstone_ROOMBUILDING_H_
#define capstone_ROOMBUILDING_H_

#include "InteractiveBuilding.h"

namespace capstone {

	// Forward declarations
	class PlanetLevel;

	/**
	 * Class definition of the equipment building where the player can equip parts.
	 */
	class EquipmentBuilding : public InteractiveBuilding {

	public:

		// Constructor and Destructor
		EquipmentBuilding(PlanetLevel* level, Ogre::Vector3 position);
		~EquipmentBuilding();

		// Events.
		void interact();

	private:

		void initProperties()	{ InteractiveBuilding::id = 1; InteractiveBuilding::name = "Armory"; }

	};

}

#endif