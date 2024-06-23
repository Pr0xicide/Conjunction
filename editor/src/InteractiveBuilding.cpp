#include "stdafx.h"

#include "InteractiveBuilding.h"

namespace capstone {

	/**
	 * InteractiveBuilding constructor.
	 */
	InteractiveBuilding::InteractiveBuilding(PlanetLevel* level) 
		: mLevel(level)
		, mEntity(NULL)
		, mSceneNode(NULL)
		, mRadius(30)
	{

	}

	/**
	 * InteractiveBuilding destructor.
	 */
	InteractiveBuilding::~InteractiveBuilding() {

	}

}
