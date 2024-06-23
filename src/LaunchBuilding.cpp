#include "stdafx.h"

#include "Buildings.h"
#include "PlanetLevel.h"

namespace capstone {

	/**
	 * LaunchBuilding constructor.
	 * 
	 * @param level			pointer to the level to get variables
	 * @param position		position of the building
	 *
	 */
	LaunchBuilding::LaunchBuilding(PlanetLevel* level, Ogre::Vector3 position) 
		: InteractiveBuilding(level)
	{

		// Change radius.
		mRadius = 55;

		// Get the scene manager from the level class.
		Ogre::SceneManager* sceneMgr = level->getSceneManager();

		// Create scene node and entity.
		mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		mEntity = sceneMgr->createEntity("Launchpad.mesh");
		mSceneNode->attachObject(mEntity);

		// Set position, and orientation.
		mSceneNode->setPosition(position.x, position.y - 25, position.z);

		initProperties();
		readXML();
	}

	/**
	 * LaunchBuilding destructor.
	 */
	LaunchBuilding::~LaunchBuilding() {

	}


// =====================================================================================
//
// #Events
//
// =====================================================================================

	/**
	 * This function will handle the event when the player interacts with
	 * this building.
	 */
	void LaunchBuilding::interact() {

		std::cout << "Enter ->" << name << std::endl;
		Game::getInstance()->switchState(SpaceGameplayState::getInstance());

	}

}
