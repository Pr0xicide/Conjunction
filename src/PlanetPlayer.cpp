#include "stdafx.h"

#include "Player.h"
#include "PlanetPlayer.h"
#include "PlanetLevel.h"
#include "Game.h"
#include "gamestates.h"
#include "Buildings.h"
#include "GlobalVariables.h"
#include "MusicManager.h"

namespace capstone {

	// Initialize static variables.
	Ogre::Real PlanetPlayer::camFoward = -175;
	Ogre::Real PlanetPlayer::camHeight = 90;
	Ogre::Real PlanetPlayer::camSideways = 5;
	Ogre::Real PlanetPlayer::turnSpeed = 10.0f;
	Ogre::Degree PlanetPlayer::yaw = Ogre::Degree(-90.0f);
	Ogre::Degree PlanetPlayer::pitch = Ogre::Degree(-20.0f);


	/** 
	 * PlanetPlayer constructor.
	 *
	 * @param camera			pointer to the camera in order to position it
	 * @param level				pointer to the planet level
	 *
	 */
	PlanetPlayer::PlanetPlayer(Ogre::Camera* camera, PlanetLevel* level) 
		: robotEntity(NULL)
		, robotSceneNode(NULL)
		, level(level)
		, wSpeed(0)
		, ySpeed(0)
		, jumpSpeed(0)
		, jumpVelocity(Ogre::Vector3::ZERO)
		, isAirborne(false)
		, selectedLocation("selectedLocation.txt")
	{
		setupBody(camera->getSceneManager());
		setupCamera(camera);
		setupAnimations();
	}

	/** 
	 * PlanetPlayer destructor.
	 */
	PlanetPlayer::~PlanetPlayer() {

		// Get scene manager from the PlanetLevel class.
		Ogre::SceneManager* sceneMgr = level->getSceneManager();

		// Destroy scene nodes.
		for (unsigned i = 0; i < nodeList.size(); i++)
			sceneMgr->destroySceneNode(nodeList[i]);

		// Destroy entities.
		sceneMgr->destroyEntity(robotEntity);

	}


//======================================================================================
//
// #Initialization
//
//======================================================================================

	/**
	 * This function will create the robot for the player.
	 *
	 * @param sceneMgr			pointer to the scene manager to create objects
	 *
	 */
	void PlanetPlayer::setupBody(Ogre::SceneManager* sceneMgr) {

		//Create the parent node, position it at the center of the planet.
		robotParentNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		robotParentNode->setPosition(0,0,0);
		nodeList.push_back(robotParentNode);

		// Create scene node.
		robotSceneNode = robotParentNode->createChildSceneNode();
		nodeList.push_back(robotSceneNode);

		// Create entity for the robot.
		robotEntity = sceneMgr->createEntity("robot.mesh");
		//robotEntity->setCastShadows(true);

		// Attach entity to scene node.
		robotSceneNode->attachObject(robotEntity);

		// Set position, orientation and scale for the player.
		robotSceneNode->setPosition(0,level->getRadius(),0);
		robotSceneNode->scale(0.1f, 0.1f, 0.1f);
		robotSceneNode->yaw(Ogre::Degree(-90));
		robotParentNode->pitch(Ogre::Degree(-35));

		keyDirection = Ogre::Vector3::ZERO;

	}

	/**
	 * This function will setup the camera so its behind the player.
	 *
	 * @param camera			pointer to the game's current camera
	 *
	 */
	void PlanetPlayer::setupCamera(Ogre::Camera* camera) {
		
		// Create a scene node for the camera.
		cameraPivot = robotSceneNode->createChildSceneNode();
		nodeList.push_back(cameraPivot);
		cameraSceneNode = cameraPivot->createChildSceneNode();
		nodeList.push_back(cameraSceneNode);

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
	void PlanetPlayer::setupAnimations() {

		// Set default walking state.
		animList.push_back(new PlayerIdleState(robotEntity));
		animList.push_back(new PlayerWalkState(robotEntity));
		animState = animList[0];
		animState->enter();

	}

	/**
	 * This function will reset the robot's speed and time animation.
	 */
	void PlanetPlayer::resetProperties() {
		wSpeed = 0;
		ySpeed = 0;
		keyDirection = Ogre::Vector3::ZERO;
	}


//======================================================================================
//
// #Update/Render
//
//======================================================================================

	/** 
	 * This function is the main update method that updates everything.
	 *
	 * @param deltaT			time since last frame
	 *
	 */
	void PlanetPlayer::update(Ogre::Real deltaT) {
		updateRobot(deltaT);
		updateCameraGoal(deltaT);
		animState->update(deltaT);
	}

	/**
	 * This function will update the robot's position on the planet.
	 *
	 * @param deltaT			time since last frame
	 *
	 */
	void PlanetPlayer::updateRobot(Ogre::Real deltaT) {

		// Get a pointer to the keyboard so that we can check the state of WASD keys.
		OIS::Keyboard* kb = Game::getInstance()->getKeyboard();

		// Reset to zero every frame.
        resetProperties();

		if (!PlanetGameplayState::getInstance()->isDialogBoxOpen()) {

			bool shouldMove = false;

			// Compute movement direction from WASD keys.
			if (kb->isKeyDown(OIS::KC_W)) {
				shouldMove = true;
				keyDirection.x = 1;
			}

			if (kb->isKeyDown(OIS::KC_S)) {
				shouldMove = true;
				keyDirection.x = -1;
			}

			if (kb->isKeyDown(OIS::KC_A)) {
				shouldMove = true;
				keyDirection.z = -1;
			}

			if (kb->isKeyDown(OIS::KC_D)) {
				shouldMove = true;
				keyDirection.z = 1;
			}

			if (shouldMove) {
				wSpeed += 1250 / level->getRadius();
				changeAnimation(animList[1]);
			}

			walkSpeed = wSpeed;

			robotParentNode->pitch(walkSpeed * deltaT);
		}

		Ogre::Vector3 pos = robotSceneNode->getPosition();
		jumpVelocity.y += -100 * deltaT;
		pos += jumpVelocity * deltaT;

		if (pos.y <= level->getRadius()) {
			pos.y = level->getRadius();
			jumpVelocity = Ogre::Vector3::ZERO;
			isAirborne = false;
		}

		robotSceneNode->setPosition(pos);

	}

	/**
	 * This function will update the robot's camera goal.
	 *
	 * @param deltaT			time since last frame
	 *
	 */
	void PlanetPlayer::updateCameraGoal(Ogre::Real deltaT) {

		Ogre::Vector3 mGoalDirection = Ogre::Vector3::ZERO;

		if (keyDirection != Ogre::Vector3::ZERO) {

			if (keyDirection.x == 1) {
				Ogre::Real toGoal = cameraPivot->getOrientation().getYaw().valueDegrees();
				robotParentNode->yaw(Ogre::Degree(toGoal) * turnSpeed * deltaT);
				cameraPivot->yaw(-Ogre::Degree(toGoal) * turnSpeed * deltaT);
			}

			else if (keyDirection.x == -1) {
				Ogre::Real toGoal = cameraPivot->getOrientation().getYaw().valueDegrees();
				Ogre::Real yawToGoal = toGoal - 180;
				robotParentNode->yaw(Ogre::Degree(yawToGoal) * turnSpeed * deltaT);
				cameraPivot->yaw(-Ogre::Degree(yawToGoal) * turnSpeed * deltaT);
			}

			else if (keyDirection.z == 1) {
				Ogre::Real toGoal = cameraPivot->getOrientation().getYaw().valueDegrees();
				Ogre::Real yawToGoal = toGoal - 90;
				robotParentNode->yaw(Ogre::Degree(yawToGoal) * turnSpeed * deltaT);
				cameraPivot->yaw(-Ogre::Degree(yawToGoal) * turnSpeed * deltaT);
			}

			else if (keyDirection.z == -1) {
				Ogre::Real toGoal = cameraPivot->getOrientation().getYaw().valueDegrees();
				Ogre::Real yawToGoal = toGoal + 90;
				robotParentNode->yaw(Ogre::Degree(yawToGoal) * turnSpeed * deltaT);
				cameraPivot->yaw(-Ogre::Degree(yawToGoal) * turnSpeed * deltaT);
			}

		}

		else {
			changeAnimation(animList[0]);
		}

		keyDirection = Ogre::Vector3::ZERO;
		
	}


//======================================================================================
//
// #Event Listeners
//
//======================================================================================

	/**
	 * This function will handle all keyboard events, when the user strikes any key.
	 */
	bool PlanetPlayer::keyPressed(const OIS::KeyEvent& arg) {

		if (!PlanetGameplayState::getInstance()->isDialogBoxOpen()) {

			// Switch from the planet level to the space level.
			if (arg.key == OIS::KC_Q) {
				//std::cout << "Planet Level >> Space Level" << std::endl;
				//resetProperties();
				//Game::getInstance()->switchState(SpaceGameplayState::getInstance());
			}

			else if (arg.key == OIS::KC_PAUSE) {
				//std::cout << "Planet Level >> Pause Menu" << std::endl;
				resetProperties();
				Game::getInstance()->switchState(PauseMenuState::getInstance());
			}


			else if(arg.key == OIS::KC_SPACE) {
				if (!isAirborne) {
					MusicManager::getInstance()->playJumpSound();
					jumpVelocity.y = 35;
					isAirborne = true;
				}
			}

		}

		return true;
	}

	/**
	 * This function will handle all keyboard events, when the user releases from any key.
	 */
	bool PlanetPlayer::keyReleased(const OIS::KeyEvent& arg) {
		return true;
	}

	/**
	 * This function will handle for when the mouse moves.
	 */
	bool PlanetPlayer::mouseMoved(const OIS::MouseEvent& arg) {

		// Get X-axis mouse movement.
		Ogre::Degree rotSpeed(gFreelookMouseSpeed * gMouseSensitivity);

		// Get mouse displacement in screen space.
		int xDisp = arg.state.X.rel;
		int zDisp = arg.state.Z.rel;

		if (xDisp != 0) {
			Ogre::Degree yawAmt = -(Ogre::Real)xDisp * rotSpeed;
			cameraPivot->yaw(yawAmt);
		}

		return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked.
	 */
	bool PlanetPlayer::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked is over.
	 */
	bool PlanetPlayer::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		return true;
	}

}
