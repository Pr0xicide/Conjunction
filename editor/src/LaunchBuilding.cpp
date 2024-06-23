#include "stdafx.h"

#include "Buildings.h"
#include "Levels.h"

namespace capstone {

	/**
	 * LaunchBuilding constructor.
	 * 
	 * @param level			pointer to the level to get variables
	 *
	 */
	LaunchBuilding::LaunchBuilding(PlanetLevel* level) 
		: InteractiveBuilding(level)
	{

		// Get the scene manager from the level class.
		Ogre::SceneManager* sceneMgr = level->getSceneManager();

		// Create scene node and entity.
		mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		mEntity = sceneMgr->createEntity("Launchpad.mesh");
		mSceneNode->attachObject(mEntity);

		// Set position, and orientation.
		mSceneNode->setPosition(0, level->getPlanetRadius() - 25, 0);

	}

	/**
	 * LaunchBuilding destructor.
	 */
	LaunchBuilding::~LaunchBuilding() {

	}

}