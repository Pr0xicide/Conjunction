#include "stdafx.h"

#include "Level.h"
#include "Game.h"

namespace capstone {

	/**
	 * Level constructor.
	 * 
	 * @param game			pointer to the game application
	 *
	 */
    Level::Level(Game* game)
		: mGame(game)
		, mRadius(100)
	{
		initLevel();
    }

	/**
	 * Level destructor.
	 */
    Level::~Level() {

        // Destroy the scene manager, which automatically destroys everything
        if (mSceneMgr)
            Ogre::Root::getSingleton().destroySceneManager(mSceneMgr);

    }


//=====================================================================================
//
// #Initialization
//
//=====================================================================================

	/**
	 * This function will initialize the variables used in the Level class.
	 */
	void Level::initLevel() {

		// Get the Ogre root.
		Ogre::Root& ogreRoot = Ogre::Root::getSingleton();

        // Instantiate the scene manager.
        mSceneMgr = ogreRoot.createSceneManager(Ogre::ST_GENERIC);

        // Set ambient light.
        mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25f, 0.25f, 0.25f));

        // Create a directional light.
        mLight = mSceneMgr->createLight();
        mLight->setType(Ogre::Light::LT_DIRECTIONAL);
        mLight->setDirection(Ogre::Vector3(-1, -2, -1).normalisedCopy());

		// Instantiate the camera.
        mCamera = mSceneMgr->createCamera("PlayerCam");
        mCamera->setNearClipDistance(1);
        mCamera->setFarClipDistance(5000);
        mCamera->setAutoAspectRatio(true);
        mCamera->setPosition(0, 2, 0);

	}

}
