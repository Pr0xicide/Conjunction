#include "stdafx.h"

#include "GameObject.h"
#include "Controllers.h"
#include "Levels.h"

namespace capstone {

	/**
	 * GameObject constructor.
	 *
	 * @param c				pointer to the controller
	 * @param mMeshName		name of the mesh to create for the entity
	 * @param mMaterial		name of the material to use for the entity mesh
	 * @param s				pointer for the scene node get its orientation
	 * @param m				pointer to the scene manager
	 *
	 */
	GameObject::GameObject(EditorController* c, std::string mMeshName, std::string mMaterial, Ogre::SceneNode* s, Ogre::SceneManager* m) 
		: mController(c)
		, mMeshName(mMeshName)
		, mMaterialName(mMaterial)
		, mSceneNode(s)
		, mSceneMgr(m)
		, isDisposable(false)
	{
		setupObject();
	}

	/**
	 * Overloaded constructor when loading objects during startup.
	 *
	 * @param c				pointer to the controller
	 * @param mMeshName		name of the mesh to create for the entity
	 * @param mMaterial		name of the material to use for the entity mesh
	 * @param scale			scale of the object
	 * @param p				pitch rotationa value
	 * @param y				yaw rotationa value
	 * @param r				roll rotationa value
	 * @param m				pointer to the scene manager
	 *
	 */
	GameObject::GameObject(EditorController* c, std::string mMeshName, std::string mMaterial, float scale, 
		Ogre::Degree p, Ogre::Degree y, Ogre::Degree r, Ogre::SceneManager* m) 
		: mController(c)
		, mMeshName(mMeshName)
		, mMaterialName(mMaterial)
		, mSceneMgr(m)
		, isDisposable(false)
	{
		loadObject(scale,p,y,r);
	}

	/**
	 * GameObject destructor.
	 */
	GameObject::~GameObject() {

		mSceneMgr->destroySceneNode(mObjectNode);
		mSceneMgr->destroyEntity(mEntity);

	}


//=====================================================================================
//
// #Initialization
//
//=====================================================================================

	/**
	 * This function will setup the game object during runtime.
	 */
	void GameObject::setupObject() {

		// Create a new entity and store it in a new scene node.
		mEntity = mSceneMgr->createEntity(mMeshName);
		mEntity->setMaterialName(mMaterialName);
		mRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mObjectNode = mRootNode->createChildSceneNode();
		mObjectNode->attachObject(mEntity);

		// Set position, orientation and scale.
		mObjectNode->setPosition(0, mController->getLevel()->getPlanetRadius(), 0);
		mRootNode->setOrientation(mSceneNode->_getDerivedOrientation());
		mObjectNode->scale(mSceneNode->getScale());

	}

	/**
	 * This function will create an object from the XML file and load it onto the planet.
	 *
	 * @param scale			scale of the object
	 * @param p				pitch rotationa value
	 * @param y				yaw rotationa value
	 * @param r				roll rotationa value
	 *
 	 */
	void GameObject::loadObject(float scale, Ogre::Degree p, Ogre::Degree y, Ogre::Degree r) {

		// Create a new entity and store it in a new scene node.
		mEntity = mSceneMgr->createEntity(mMeshName);
		mEntity->setMaterialName(mMaterialName);
		mRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mObjectNode = mRootNode->createChildSceneNode();
		mObjectNode->attachObject(mEntity);
		mObjectNode->yaw(Ogre::Degree(-90));

		// Set position, orientation and scale.
		mObjectNode->setPosition(0, mController->getLevel()->getPlanetRadius(), 0);
		mRootNode->pitch(p);
		mRootNode->yaw(y);
		mRootNode->roll(r);
		mObjectNode->scale(scale,scale,scale);

	}


//=====================================================================================
//
// #Update/Render
//
//=====================================================================================

	/**
	 * This function updates the game object.
	 *
	 * @param deltaT		time since last frame
	 *
	 */
	void GameObject::update(Ogre::Real deltaT) {

	}

}