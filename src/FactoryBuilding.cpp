#include "stdafx.h"

#include "Buildings.h"
#include "PlanetLevel.h"

namespace capstone {

	/**
	 * FactoryBuilding constructor.
	 * 
	 * @param level			pointer to the level to get variables
	 * @param position		position of the building
	 *
	 */
	FactoryBuilding::FactoryBuilding(PlanetLevel* level, Ogre::Vector3 position)
		: InteractiveBuilding(level)
	{
		
		// Get the scene manager from the level class.
		Ogre::SceneManager* sceneMgr = level->getSceneManager();

		// Create scene node and entity.
		mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		mEntity = sceneMgr->createEntity("factory.mesh");
		mEntity->setMaterialName("factory");
		mSceneNode->attachObject(mEntity);

		// Set position, orientation and scale.
		mSceneNode->setPosition(position);
		mSceneNode->scale(0.35f, 0.35f, 0.35f);
		mSceneNode->roll(Ogre::Degree(90));
		mSceneNode->yaw(Ogre::Degree(90));

		initProperties();
		readXML();

	}

	/**
	 * FactoryBuilding destructor.
	 */
	FactoryBuilding::~FactoryBuilding() {

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
	void FactoryBuilding::interact() {

		std::cout << "Enter ->" << name << std::endl;
		RoomGameplayState::getInstance()->setRoomId(id);
		Game::getInstance()->switchState(RoomGameplayState::getInstance());
		
	}
	
}
