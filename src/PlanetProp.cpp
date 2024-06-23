#include "stdafx.h"

#include "PlanetProp.h"
#include "Levels.h"

namespace capstone {

	/**
	 * PlanetProp constructor.
	 *
	 * @param level				pointer the level to get more info
	 * @param objStruct			structure containing information about the object that needs to be created
	 *
	 */
	PlanetProp::PlanetProp(PlanetLevel* level, PlanetObjectStruct* objStruct) 
		: level(level)
		, objStruct(objStruct)
		, mEntity(NULL)
		, mRootNode(NULL)
		, mSceneNode(NULL)
	{
		mSceneMgr = level->getSceneManager();
		setupObject();
	}

	/** 
	 * PlanetProp destructor.
	 */
	PlanetProp::~PlanetProp() {
		delete objStruct;
	}


//=====================================================================================
//
// #Initialization
//
//=====================================================================================

	/**
	 * This function will setup the object on the planet.
	 */
	void PlanetProp::setupObject() {

		// Create the entity and attach it to the scene node.
		mEntity = mSceneMgr->createEntity(objStruct->meshName);
		mEntity->setMaterialName(objStruct->materialName);
		mRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mSceneNode = mRootNode->createChildSceneNode();
		mSceneNode->setPosition(0, level->getRadius() - 3.5f, 0);
		mSceneNode->attachObject(mEntity);

		// Set object position and scale.
		mRootNode->pitch(objStruct->pitch);
		mRootNode->yaw(objStruct->yaw);
		mRootNode->roll(objStruct->roll);
		mSceneNode->scale(objStruct->scale,objStruct->scale,objStruct->scale);

	}

}
