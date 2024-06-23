#include "stdafx.h"

#include "Buildings.h"
#include "Levels.h"

namespace capstone {

	/**
	 * Constructor for the room building.
	 * 
	 * @param level			pointer to the level to get variables
	 * @param position		position of the building
	 *
	 */
	EquipmentBuilding::EquipmentBuilding(PlanetLevel* level)
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
		mSceneNode->setPosition(level->getPlanetRadius(),0,0);
		mSceneNode->scale(0.35f, 0.35f, 0.35f);
		mSceneNode->roll(Ogre::Degree(-90));
		mSceneNode->yaw(Ogre::Degree(-90));

	}

	/**
	 * EquipmentBuilding destructor.
	 */
	EquipmentBuilding::~EquipmentBuilding() {

	}

}