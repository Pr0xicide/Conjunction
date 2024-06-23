#include "stdafx.h"

#include "Resources.h"
#include "SpaceLevel.h"

namespace capstone {

	/**
	 * MetalScrap constructor.
	 *
	 * @param level				pointer to the level that gain information
	 * @param pos				position of where the particle should be when created
	 *
	 */
	MetalScrap::MetalScrap(SpaceLevel* level, Ogre::Vector3 pos) 
		: Resource(level, pos)
	{
		this->setup();
	}

	/**
	 * MetalScrap destructor.
	 */
	MetalScrap::~MetalScrap() {
		Ogre::SceneManager* mSceneMgr = level->getSceneManager();
		mSceneMgr->destroyBillboardSet(mBillboardSet);
		mSceneMgr->destroyRibbonTrail(mTrail);
		mSceneMgr->destroySceneNode(mSceneNode);
		mSceneMgr->destroyEntity(mEntity);
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================
	
	/**
	 * This function will setup the scene node for the particle.
	 */
	void MetalScrap::setup() {

		// Set id.
		resourceID = 2;

		// Define how much dark matter is in this particle.
		amount = rand() % 50 + 10;

		// Let the parent class create the scene node.
		Resource::setup();
		
		// Set properties.
		mEntity->setMaterialName("m_Metal");
		mSceneNode->attachObject(mEntity);
		mSceneNode->setPosition(position);
		mSceneNode->scale(0.01f,0.01f,0.01f);

	}


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will update the resource particle.
	 *
	 * @param delatT			time since last frame
	 *
	 */
	void MetalScrap::update(Ogre::Real deltaT) {
		Resource::update(deltaT);
	}

}
