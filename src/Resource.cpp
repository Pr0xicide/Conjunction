#include "stdafx.h"

#include "Resource.h"
#include "Levels.h"
#include "SpacePlayer.h"

namespace capstone {

	/**
	 * Resource constructor.
	 *
	 * @param level				pointer to the level that gain information
	 * @param pos				position of where the particle should be when created
	 *
	 */
	Resource::Resource(SpaceLevel* level, Ogre::Vector3 pos) 
		: level(level)
		, isDisposable(false)
		, position(pos)
		, speed(75)
	{

	}

	/**
	 * Resource destructor.
	 */
	Resource::~Resource() {

	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will setup the scene node for the particle.
	 */
	void Resource::setup() {

		// Get the scene manager from the level.
		Ogre::SceneManager* mSceneMgr = level->getSceneManager();

        // Create a "flare" billboard.
        mBillboardSet = mSceneMgr->createBillboardSet(1);
        mBillboardSet->setMaterialName("Flare");
        mBillboardSet->setDefaultDimensions(30.0f, 30.0f);
        mBillboardSet->createBillboard(0, 0, 0, Ogre::ColourValue(0, 0, 1.0f));

		// Create ribbon trail.
        Ogre::Real trailLength = 130;
        Ogre::Real fadeSpeed = speed / trailLength;
        mTrail = mSceneMgr->createRibbonTrail();
		mTrail->setMaterialName("Trail");
        mTrail->setMaxChainElements(50);
		mTrail->setTrailLength(trailLength);
		mTrail->setInitialColour(0, 1.0f, 1.0f, 0);
		mTrail->setColourChange(0, fadeSpeed, fadeSpeed, fadeSpeed, fadeSpeed);
		mTrail->setInitialWidth(0, 1.0f);

		// Create a scene node and attach an entity to it.
		mEntity = mSceneMgr->createEntity("cube.mesh");
		mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mSceneNode->attachObject(mBillboardSet);

		// Attach trail.
		mSceneMgr->getRootSceneNode()->attachObject(mTrail);
		mTrail->addNode(mSceneNode);

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
	void Resource::update(Ogre::Real deltaT) {

		// Get the player position.
		Ogre::Vector3 player = level->getPlayer()->getSceneNode()->getPosition();

		// Get the direction to the player from this particle.
		Ogre::Vector3 toPlayer = player - mSceneNode->getPosition();
		Ogre::Vector3 dir = toPlayer.normalisedCopy();

		// Apply velocity.
		mSceneNode->translate(dir * speed * deltaT, Ogre::Node::TS_WORLD);

	}

}
