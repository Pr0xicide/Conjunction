#include "stdafx.h"

#include "GlobalVariables.h"
#include "Projectile.h"
#include "Part.h"
#include "Skill.h"
#include "SpaceLevel.h"
#include "SpacePlayer.h"

namespace capstone {

	/**
	 * Default Projectile constructor.
	 */
	Projectile::Projectile() {
		
	}

	/**
	 * Overloaded Projectile constructor.
	 *
	 * @param shooter			source of the shooter			
	 * @param position			position of the projectile in space
	 * @param orientation		orientation of the projectile in space
	 * @param size				size of the projectile in space
	 * @param material		
	 * @param speed				speed of the projectile
	 * @param acceleration		
	 * @param colour			colour of the projectile in space
	 * @param lifeSpan			the lifespan of the projectile before it dies in space
	 * @param dmgAmount			how much damage this projectile will do
	 *
	 */
	Projectile::Projectile(SpaceLevel* level, void* shooter,
						   const Ogre::Vector3& position,
						   const Ogre::Quaternion& orientation,
						   const Ogre::Real size,
						   const Ogre::String material,
						   const Ogre::Real speed,
						   const Ogre::Real acceleration,
						   const Ogre::ColourValue colour,
						   const Ogre::Real lifeSpan,
						   int dmgAmount)
		: mLevel(level)
        , mShooter(shooter)
		, mSize(size)
		, mMaterial(material)
		, mSpeed(speed)
		, mAcceleration(acceleration)
		, mColour(colour)
		, mLifeSpan(lifeSpan)
        , mSceneNode(NULL)
        , mBillboardSet(NULL)
        , mTrail(NULL)
        , mVelocity(0, 0, 0)
		, mBirth(0.0)
		, mDestroy(false)
		, damageAmount(dmgAmount)
	{
		mBirth = mLevel->getTime();
		Ogre::SceneManager* sceneMgr = mLevel->getSceneManager();

        // create a "flare" billboard
        mBillboardSet = sceneMgr->createBillboardSet(1);
        mBillboardSet->setMaterialName(mMaterial);
        mBillboardSet->setDefaultDimensions(mSize, mSize);
        mBillboardSet->createBillboard(0, 0, 0, mColour);

        // create the trail
        Ogre::Real trailLength = 10;
        Ogre::Real fadeSpeed = mSpeed / trailLength;
        mTrail = sceneMgr->createRibbonTrail();
		mTrail->setMaterialName("Trail");
        mTrail->setMaxChainElements(50);
		mTrail->setTrailLength(trailLength);
		mTrail->setInitialColour(0, 1, 0.8f, 0);
		mTrail->setColourChange(0, fadeSpeed, fadeSpeed, fadeSpeed, fadeSpeed);
		mTrail->setInitialWidth(0, 0.25f);

		// create and set up the scene node
        mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
        mSceneNode->setPosition(position);
        mSceneNode->setOrientation(orientation);
        mSceneNode->attachObject(mBillboardSet);

		//mTrailNode->attachObject(mTrail);
        sceneMgr->getRootSceneNode()->attachObject(mTrail);
        mTrail->addNode(mSceneNode);

        // compute velocity
        //Ogre::Vector3 dir = orientation * gForward; //without gForward
		Ogre::Vector3 dir = orientation * gForward;
        mVelocity = mSpeed * dir;
	}

	/**
	 * Projectile destructor.
	 */
	Projectile::~Projectile(){
		 // remove from scene
        Ogre::SceneManager* sceneMgr = mSceneNode->getCreator();
        sceneMgr->destroyBillboardSet(mBillboardSet);
        sceneMgr->destroyRibbonTrail(mTrail);
        sceneMgr->destroySceneNode(mSceneNode);
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
	void Projectile::update(Ogre::Real deltaT)
    {
        // apply gravity
        mVelocity += mAcceleration * deltaT;

        // update position based on velocity
        Ogre::Vector3 disp = deltaT * mVelocity;
        mSceneNode->translate(disp);

		if((mBirth + mLifeSpan) <= mLevel->getTime())
			destroy();
    }

}