#include "stdafx.h"

#include "GlobalVariables.h"
#include "Levels.h"
#include "gamestates.h"
#include "Player.h"
#include "RoomPlayer.h"
#include "Game.h"
#include "Part.h"
#include "CompletePart.h"
#include "ReallocState.h"
#include "Attributes.h"
#include "Projectile.h"
#include "MusicManager.h"
#include "skills.h"
#include "Projectile.h"

namespace capstone {

	// Initialize static variables.
	Ogre::Real RoomPlayer::camFoward = -175;
	Ogre::Real RoomPlayer::camHeight = 90;
	Ogre::Real RoomPlayer::camSideways = 5;
	Ogre::Degree RoomPlayer::yaw = Ogre::Degree(-90.0f);
	Ogre::Degree RoomPlayer::pitch = Ogre::Degree(-20.0f);
	Ogre::Real RoomPlayer::turnSpeed = 10.0f;


	/** 
	 * RoomPlayer constructor.
	 *
	 * @param camera			pointer to the camera to place on the player
	 * @param roomID			ID of the room to identify which room the player is in
	 *
	 */
	RoomPlayer::RoomPlayer(Ogre::Camera* camera, int roomID) 
		: roomID(roomID)
	{
		sceneMgr = camera->getSceneManager();
		setupBody(sceneMgr);
		setupCamera(camera);
		setupAnimations();
	}

	/** 
	 * RoomPlayer destructor.
	 */
	RoomPlayer::~RoomPlayer() {

		// Destroy scene nodes.
		for (unsigned i = 0; i < nodes.size(); i++)
			sceneMgr->destroySceneNode(nodes[i]);

	}


//=====================================================================================
//
// #Initialization
//
//=====================================================================================

	/**
	 * This function will create the robot for the player.
	 * 
	 * @param sceneMgr			pointer to the scene manager to create objects
	 *
	 */
	void RoomPlayer::setupBody(Ogre::SceneManager* sceneMgr) {

		// Create the robot entity and attach it to the scene node.
		mEntity = sceneMgr->createEntity("robot.mesh");
		mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		mSceneNode->attachObject(mEntity);
		mSceneNode->scale(0.1f, 0.1f, 0.1f);
		nodes.push_back(mSceneNode);

	}

	/** 
	 * This function will setup the camera so its behind the player.
	 *
	 * @param camera			pointer to the game's current camera
	 *
	 */
	void RoomPlayer::setupCamera(Ogre::Camera* camera) {

		// Create a scene node for the camera.
		cameraPivot = mSceneNode->createChildSceneNode();
		cameraSceneNode = cameraPivot->createChildSceneNode();
		nodes.push_back(cameraPivot);
		nodes.push_back(cameraSceneNode);

		// Position camera.
		cameraSceneNode->translate(camFoward,camHeight,camSideways);

		// Attach camera.
		cameraSceneNode->attachObject(camera);
		cameraSceneNode->yaw(yaw);
		cameraSceneNode->pitch(pitch);

	}

	/**
	 * This function will setup all the animations for the player.
	 */
	void RoomPlayer::setupAnimations() {

		// Set default walking state.
		animList.push_back(new PlayerIdleState(mEntity));
		animList.push_back(new PlayerWalkState(mEntity));
		animState = animList[0];

	}


//=====================================================================================
//
// #Update/Render
//
//=====================================================================================

	/** 
	 * This function updates the player.
	 * 
	 * @param deltaT			time since last frame
	 *
	 */
	void RoomPlayer::update(Ogre::Real deltaT) {

	}


//=====================================================================================
//
// #Event Listeners
//
//=====================================================================================

	/**
	 * This function will handle all keyboard events, when the user strikes any key.
	 */
	bool RoomPlayer::keyPressed(const OIS::KeyEvent& arg) {
		return true;
	}

	/**
	 * This function will handle all keyboard events, when the user releases from any key.
	 */
	bool RoomPlayer::keyReleased(const OIS::KeyEvent& arg) {
		return true;
	}

	/**
	 * This function will handle for when the mouse moves.
	 */
	bool RoomPlayer::mouseMoved(const OIS::MouseEvent& arg) {
		return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked.
	 */
	bool RoomPlayer::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked is over.
	 */
	bool RoomPlayer::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		return true;
	}

}
