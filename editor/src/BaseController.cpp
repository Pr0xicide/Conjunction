#include "stdafx.h"

#include "BaseController.h"

namespace capstone {

	/**
	 * BaseController constructor.
	 *
	 * @param l			pointer to the level to place objects on
	 * @param c			camera pointer to re-adjust the position
	 * @param sm		pointer to the scene manager to create objects
	 *
	 */
	BaseController::BaseController(Level* l, Ogre::Camera* c, Ogre::SceneManager* sm) 
		: mLevel(l)
		, mCamera(c)
		, mSceneMgr(sm)
	{

	}

	/**
	 * BaseController destructor.
	 */
	BaseController::~BaseController() {

	}

}