#ifndef capstone_INTERACTIVEBUILDING_H_
#define capstone_INTERACTIVEBUILDING_H_

#include "Game.h"

namespace capstone {

	// Forward declaration.
	class PlanetLevel;

	/**
	 * This class provides the base functionally of the buildings that the player can enter into.
	 */
	class InteractiveBuilding {

	protected:

		// Class variables.
		PlanetLevel*				mLevel;
		
		Ogre::Entity*				mEntity;
		Ogre::SceneNode*			mSceneNode;
		Ogre::Real					mRadius;

	public:

		InteractiveBuilding(PlanetLevel* level);
		virtual ~InteractiveBuilding();

	private:

	};

}

#endif