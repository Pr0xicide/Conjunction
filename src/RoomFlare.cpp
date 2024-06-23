#include "stdafx.h"

#include "Rooms.h"

namespace capstone {

	/**
	 * RoomFlare constructor.
	 *
	 * @param roomLevel				pointer to the room level to get properties
	 * @param position				the positioning of the flare
	 *
	 */
	RoomFlare::RoomFlare(Room* roomLevel, Ogre::Vector3 position) 
		: room(roomLevel)
		, position(position)
		, mSize(10.0f)
		, mSpeed(35.0f)
	{
		setupFlare();
	}

	/**
	 * RoomFlare destructor.
	 */
	RoomFlare::~RoomFlare() {

	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will setup up the flare to be used in the room.
	 */
	void RoomFlare::setupFlare() {

		// Tweakable scale factors.
		sfX = Ogre::Math::RangeRandom(-2500, 2500);
		sfY = 1.0f;
		sfZ = Ogre::Math::RangeRandom(-2500, 2500);
		sfTimeScale = 1.0f;

		// Hypocycloid magic numbers.
		a = (1.0f / 7.0f);
		b = 2.0f;
		accumulator = 0.0f;
		int animate = 1;

		// Get the scene manager from the room level.
		Ogre::SceneManager* mSceneMgr = room->getSceneManager();

        // Create a "flare" billboard.
		mColour = Ogre::ColourValue(0,1,1);
        mBillboardSet = mSceneMgr->createBillboardSet(1);
        mBillboardSet->setMaterialName("Flare");
        mBillboardSet->setDefaultDimensions(mSize, mSize);
        mBillboardSet->createBillboard(0, 0, 0, mColour);
		
        // Create the trail
        Ogre::Real trailLength = 10;
        Ogre::Real fadeSpeed = mSpeed / trailLength;
        mTrail = mSceneMgr->createRibbonTrail();
		mTrail->setMaterialName("Trail");
        mTrail->setMaxChainElements(50);
		mTrail->setTrailLength(trailLength);
		mTrail->setInitialColour(0, 1, 0.8f, 0);
		mTrail->setColourChange(0, fadeSpeed, fadeSpeed, fadeSpeed, fadeSpeed);
		mTrail->setInitialWidth(0, 0.25f);

		// New point light.
		mLight = mSceneMgr->createLight();
		mLight->setType(Ogre::Light::LT_POINT);
		mLight->setPosition(position);
		mLight->setDiffuseColour(mColour);
		mLight->setAttenuation(1000, 1, 0, 0);

		// Create a scene node for this flare.
		mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        mSceneNode->attachObject(mBillboardSet);
		mSceneNode->attachObject(mLight);
		mSceneNode->setPosition(position);
		mTrail->addNode(mSceneNode);

	}


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will update the projectile in space.
	 *
	 * @param deltaT			time since last frame
	 */
	void RoomFlare::update(Ogre::Real deltaT) {

		accumulator += deltaT * sfTimeScale;

		if (animate) {

			if (mSceneNode) {

				// Procedural hypocycloid.
				float computedX1 = (1.0f - a)*cos(a * accumulator);
				float computedX2 = a * b * cos((1.0f - a)*accumulator);
				float computedX = sfX*(computedX1 + computedX2);

				float computedZ1 = (1.0f - a)*sin(a * accumulator);
				float computedZ2 = a * b * sin((1.0f - a)*accumulator);
				float computedZ = sfZ*(computedZ1 + computedZ2);

				// Move.
				mSceneNode->setPosition(computedX, mSceneNode->getPosition().y, computedZ);
			}

		}


	}

}
