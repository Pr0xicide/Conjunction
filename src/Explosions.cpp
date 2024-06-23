#include "stdafx.h"

#include "GlobalVariables.h"
#include "Util.h"
#include "SpaceLevel.h"
#include "Explosions.h"

namespace capstone {
	
	 // static id number
    int Explosions::msMatId = 0;

	/**
	 * Explosions constructor.
	 *
	 * @param spaceLevel				pointer to the space level to gain properties
	 * @param fireballSize				size of the explosion
	 * @param position					position where the effect should play in 3D space
	 *
	 */
    Explosions::Explosions(SpaceLevel* spaceLevel,
                                         Ogre::Real fireballSize,
                                         const Ogre::Vector3& position)
        : mSpaceLevel(spaceLevel)
        , mSceneNode(NULL)
        , mBillboardSet(NULL)
        , mFireballSize(fireballSize)
        , mDisposalTime(3)
    {
        // generate a unique name for the material clone
        std::stringstream ss;
        ss << "FireballExplosion" << msMatId++;
        mMatName = ss.str();
        
        // clone the material
        // - without cloning the material, all billboards will share the same animation cycle
        //   and will appear synchronized, which is undesirable
        // - cloning the material and resetting the current frame to 0 is an easy but inefficient way
        //   of overcoming this problem
        // - a better way would be to use a custom shader for the texture animation
        //

        Ogre::MaterialPtr matTemplatePtr = Ogre::MaterialManager::getSingleton().getByName("FireballExplosion");
        Ogre::MaterialPtr matPtr = matTemplatePtr->clone(mMatName);

        // set animation frame to the beginning
        //
		// here was a change AC
        matTemplatePtr->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setCurrentFrame(0);
        
        // create billboard set containing animated explosions
        mBillboardSet = spaceLevel->getSceneManager()->createBillboardSet(1);
        mBillboardSet->setMaterialName(mMatName);
        mBillboardSet->setDefaultDimensions(mFireballSize, mFireballSize);

        // generate random rotation
        // - this makes the effect appear more unique every time
        Ogre::Radian angle(Ogre::Math::RangeRandom(0, Ogre::Math::TWO_PI));

        // create billboard
        Ogre::Billboard* bb = mBillboardSet->createBillboard(0, 0, 0);
        bb->setRotation(angle);

        mSceneNode = spaceLevel->getSceneManager()->getRootSceneNode()->createChildSceneNode();
        mSceneNode->setPosition(position);
        mSceneNode->attachObject(mBillboardSet);

        mDisposalTime = mSpaceLevel->getTime();
    }

	/**
	 * Explosions destructor.
	 */
    Explosions::~Explosions() {
        mSpaceLevel->getSceneManager()->destroyBillboardSet(mBillboardSet);
        Ogre::MaterialManager::getSingleton().remove(mMatName);
        mSpaceLevel->getSceneManager()->destroySceneNode(mSceneNode);
    }

	/**
	 * If this function will determine if the explosion should stop playing.
	 *
	 * @return a boolean to determine if the explosion effect is done
	 *
	 */
    bool Explosions::isDisposable() const {
        return mSpaceLevel->getTime() >= mDisposalTime;
    }

}

