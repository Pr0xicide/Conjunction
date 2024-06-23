#include "stdafx.h"

#include "Buildings.h"
#include "PlanetLevel.h"

namespace capstone {

	/**
	 * Constructor for the room building.
	 * 
	 * @param level			pointer to the level to get variables
	 * @param position		position of the building
	 *
	 */
	EquipmentBuilding::EquipmentBuilding(PlanetLevel* level, Ogre::Vector3 position)
		: InteractiveBuilding(level)
	{
		
		// Get the scene manager from the level class.
		Ogre::SceneManager* sceneMgr = level->getSceneManager();

		// Create scene node and entity.
		mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		mEntity = sceneMgr->createEntity("EquipmentBuilding.mesh");
		mEntity->setMaterialName("EquipmentBuilding");
		mSceneNode->attachObject(mEntity);

		// Set position, orientation and scale.
		mSceneNode->setPosition(position);
		mSceneNode->scale(0.35f, 0.35f, 0.35f);
		mSceneNode->roll(Ogre::Degree(-90));
		mSceneNode->yaw(Ogre::Degree(-90));

		initProperties();
		readXML();

	}

	/**
	 * EquipmentBuilding destructor.
	 */
	EquipmentBuilding::~EquipmentBuilding() {

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
	void EquipmentBuilding::interact() {

		// Enter the building that will allow the robot to equip parts.
		std::cout << "Enter ->" << name << std::endl;
		RoomGameplayState::getInstance()->setRoomId(id);
		Game::getInstance()->switchState(RoomGameplayState::getInstance());

	}

}