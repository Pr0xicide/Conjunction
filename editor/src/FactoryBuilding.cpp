#include "stdafx.h"

#include "Buildings.h"
#include "Levels.h"

namespace capstone {

	/**
	 * FactoryBuilding constructor.
	 * 
	 * @param level			pointer to the level to get variables
	 * @param position		position of the building
	 *
	 */
	FactoryBuilding::FactoryBuilding(PlanetLevel* level)
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
		mSceneNode->setPosition(-level->getPlanetRadius(),0,0);
		mSceneNode->scale(0.35f, 0.35f, 0.35f);
		mSceneNode->roll(Ogre::Degree(90));
		mSceneNode->yaw(Ogre::Degree(90));

	}

	/**
	 * FactoryBuilding destructor.
	 */
	FactoryBuilding::~FactoryBuilding() {

	}
	
}
