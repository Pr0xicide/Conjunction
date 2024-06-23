#include "stdafx.h"

#include "SpacePlayerEssentials.h"
#include "Player.h"
#include "SpacePlayer.h"
#include "Game.h"
#include "MusicManager.h"

namespace capstone {

	// Initialize static variables.
	Ogre::Degree SpacePlayer::yaw = Ogre::Degree(-90.0f);
	Ogre::Degree SpacePlayer::pitch = Ogre::Degree(-1.0f);
	Ogre::Real SpacePlayer::zShoulderPosition = 10.0f;
	Ogre::Real SpacePlayer::yShoulderPosition = 73.0f;
	Ogre::Real SpacePlayer::camFoward = -175;
	Ogre::Real SpacePlayer::camHeight = 45;
	Ogre::Real SpacePlayer::camSideways = 5;
	Ogre::Real SpacePlayer::turnSpeed = 10.0f;
	Ogre::Real SpacePlayer::MAX_HEALTH = 100;

	/** 
	 * SpacePlayer constructor.
	 *
	 * @param camera		pointer to the camera in order to position it
	 * @param level			pointer to the space level
	 *
	 */
	SpacePlayer::SpacePlayer(Ogre::Camera* camera, SpaceLevel* level, Ogre::Degree yaw1,
		Ogre::Real normalSpeed, Ogre::Real fastSpeed) 
		: robotEntity(NULL)
		, robotSceneNode(NULL)
		, level(level)
		, room(NULL)
		, inSpace(true)
		, robotMove(0,0,0)
		, headNode(NULL)
		, leftArmNode(NULL)
		, rightArmNode(NULL)
		, normalSpeed(normalSpeed)
		, fastSpeed(fastSpeed)
		, mHeadPart(NULL)
		, mLeftArmPart(NULL)
		, mRightArmPart(NULL)
		, mHealth(100)
		, hitRadius(50)
		, shouldShootAgain(false)
		, shoulderPartsCooldown(0)
	{
		setupBody(camera->getSceneManager());
		setupCamera(camera);
		setupAnimations();
	}

	/**
	 * Overloaded constructor for SpacePlayer to appear in the rooms.
	 *
	 * @param camera		pointer to the camera in order to position it
	 * @param room			pointer to the room level
	 * @param yaw1			rotation for the robot to face
	 * @param normalSpeed	normal movement for the robot
	 * @param fastSpeed		robot's turbo speed
	 *
	 */
	SpacePlayer::SpacePlayer(Ogre::Camera* camera, Room* room, Ogre::Degree yaw1,
		Ogre::Real normalSpeed, Ogre::Real fastSpeed)
		: robotEntity(NULL)
		, robotSceneNode(NULL)
		, level(NULL)
		, room(room)
		, inSpace(false)
		, isAirborne(false)
		, robotMove(0,0,0)
		, headNode(NULL)
		, leftArmNode(NULL)
		, rightArmNode(NULL)
		, normalSpeed(normalSpeed)
		, fastSpeed(fastSpeed)
		, mHeadPart(NULL)
		, mLeftArmPart(NULL)
		, mRightArmPart(NULL)
		, ySpeed(0)
		, yawSpeed(Ogre::Degree(100))
		, mHealth(1000)
		, shoulderPartsCooldown(0)
		, pausing(false)
	{
		setupBody(camera->getSceneManager());
		setupCamera(camera);
		setupAnimations();
	}

	/** 
	 * SpacePlayer destructor.
	 */
	SpacePlayer::~SpacePlayer() {

		// Get scene manager.
		Ogre::SceneManager* sceneMgr = NULL;

		// The scene manager either comes from the spaceLevel or the room, not both
		if(inSpace)
			sceneMgr = level->getSceneManager();
		else
			sceneMgr = room->getSceneManager();

		// Destroy scene nodes.
		if(inSpace)
			sceneMgr->destroySceneNode(cameraSceneNode);

		else {
			sceneMgr->destroySceneNode(cameraSceneNode);
			sceneMgr->destroySceneNode(mCameraPivot);
		}

		for (unsigned i = 0; i < nodes.size(); i++) {
			sceneMgr->destroySceneNode(nodes[i]);
		}

		// Destroy entities.
		sceneMgr->destroyEntity(robotEntity);
		
		// Destroy local part pointers.
		for (unsigned i = 0; i < parts.size(); i++)
			delete parts[i];

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
	void SpacePlayer::setupBody(Ogre::SceneManager* sceneMgr) {

		localMoveDir = Ogre::Vector3::ZERO;

		// Create scene node.
		robotSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		nodes.push_back(robotSceneNode);

		// Scene nodes for holding realloc parts
		headNode = robotSceneNode->createChildSceneNode();				nodes.push_back(headNode);
		leftArmNode = robotSceneNode->createChildSceneNode();			nodes.push_back(leftArmNode);
		rightArmNode = robotSceneNode->createChildSceneNode();			nodes.push_back(rightArmNode);
		leftShoulderNode = robotSceneNode->createChildSceneNode();		nodes.push_back(leftShoulderNode);
		rightShoulderNode = robotSceneNode->createChildSceneNode();		nodes.push_back(rightShoulderNode);

		// Scene nodes for spawning projectiles and such.
		headSpawnNode = robotSceneNode->createChildSceneNode();			nodes.push_back(headSpawnNode);
		leftSpawnNode = robotSceneNode->createChildSceneNode();			nodes.push_back(leftSpawnNode);
		rightSpawnNode = robotSceneNode->createChildSceneNode();		nodes.push_back(rightSpawnNode);
		leftShoulderSpawnNode = robotSceneNode->createChildSceneNode();	nodes.push_back(leftShoulderSpawnNode);
		rightShoulderSpawnNode = robotSceneNode->createChildSceneNode();nodes.push_back(rightShoulderSpawnNode);

		// Create entity for the robot.
		robotEntity = sceneMgr->createEntity("robot.mesh");

		// Attach entity to scene node.
		robotSceneNode->attachObject(robotEntity);

		// Set scale.
		robotSceneNode->scale(0.1f, 0.1f, 0.1f);

		// Get pointers to the bones.
        mHeadBone = robotEntity->getSkeleton()->getBone("Joint12");
        mLeftArmBone = robotEntity->getSkeleton()->getBone("Joint15");
        mRightArmBone = robotEntity->getSkeleton()->getBone("Joint18");

		// Create SceneNodes and place them right where the bones are.
		headNode->setPosition(mHeadBone->_getDerivedPosition());
		leftArmNode->setPosition(mLeftArmBone->_getDerivedPosition());
		rightArmNode->setPosition(mRightArmBone->_getDerivedPosition());
		leftShoulderNode->setPosition(0, yShoulderPosition, zShoulderPosition);
		rightShoulderNode->setPosition(0, yShoulderPosition, -zShoulderPosition);

		// Nodes where projectiles spawn from.
		headSpawnNode->setPosition(mHeadBone->_getDerivedPosition());
		leftSpawnNode->setPosition(mLeftArmBone->_getDerivedPosition());
		rightSpawnNode->setPosition(mRightArmBone->_getDerivedPosition());
		leftShoulderSpawnNode->setPosition(leftShoulderNode->_getDerivedPosition());
		rightShoulderSpawnNode->setPosition(rightShoulderNode->_getDerivedPosition());

		// Initialize local parts.
		mHeadPart = new CompletePart();					parts.push_back(mHeadPart);
		mLeftArmPart = new CompletePart();				parts.push_back(mLeftArmPart);
		mRightArmPart = new CompletePart();				parts.push_back(mRightArmPart);
		mLeftShoulderPart = new CompletePart();			parts.push_back(mLeftShoulderPart);
		mRightShoulderPart = new CompletePart();		parts.push_back(mRightShoulderPart);

	}

	/**
	 * This function will setup the camera so its behind the player.
	 *
	 * @param camera			pointer to the game's current camera
	 *
	 */
	void SpacePlayer::setupCamera(Ogre::Camera* camera) {
		
		if (inSpace) {

			// Create a scene node for the camera.
			cameraSceneNode = robotSceneNode->createChildSceneNode();

			// Position camera.
			cameraSceneNode->translate(-150,75,75);

			// Attach camera.
			cameraSceneNode->attachObject(camera);
			cameraSceneNode->yaw(Ogre::Degree(-90));

			// set the 3D sound listener attributes
			MusicManager::getInstance()->setListenerAttributes(camera->getPosition(), Ogre::Vector3::ZERO,
				camera->getDirection(), camera->getUp());

		}
		
		else {

			// Create a pivot at roughly the character's shoulder.
			mCameraPivot = robotSceneNode->createChildSceneNode();
			cameraSceneNode = mCameraPivot->createChildSceneNode();
			cameraSceneNode->translate(camFoward,camHeight,camSideways);

			// Attach camera.
			cameraSceneNode->attachObject(camera);
			cameraSceneNode->yaw(yaw);
			cameraSceneNode->pitch(pitch);
		}
	
		mCamera = camera;

	}

	/**
	 * This function will setup all the animations for the player.
	 */
	void SpacePlayer::setupAnimations() {

		// Set default walking state.
		animList.push_back(new PlayerIdleState(robotEntity));
		animList.push_back(new PlayerWalkState(robotEntity));

		if (!inSpace)
			animState = animList[0];

	}


//=====================================================================================
//
// #Update/Render
//
//=====================================================================================

	/** 
	 * This function is the main update method that updates everything.
	 * 
	 * @param deltaT			time since last frame
	 *
	 */
	void SpacePlayer::update(Ogre::Real deltaT) {
		if (mHealth <= 0.0f) {
			std::cout << "Player died Going to swtich state to game over" << std::endl;
			robotMove = Ogre::Vector3::ZERO;
			/*
			if(Game::getInstance()->getCurrentState() != GameOverState::getInstance()){
				std::cout << "Already in the game over state" << std::endl;
			}else{
				
				Game::getInstance()->switchState(GameOverState::getInstance());
			}*/
		}
		else {
			updateRobot(deltaT);
			if (!inSpace)
				animState->update(deltaT);
		}
	}

	/** 
	 * This function updates the robot positioning.
	 * 
	 * @param deltaT			time since last frame
	 *
	 */
	void SpacePlayer::updateRobot(Ogre::Real deltaT) {

		// Update cooldowns.
		if (Game::getInstance()->getTime() >= shoulderPartsCooldown) {
			shouldShootAgain = true;
		}

		// Get a pointer to the keyboard so that we can check the state of WASD keys.
		OIS::Keyboard* kb = Game::getInstance()->getKeyboard();

		// Compute movement direction vector from WASD keys.
        localMoveDir = Ogre::Vector3::ZERO;
        if (kb->isKeyDown(OIS::KC_W))				localMoveDir.x += 1;
        if (kb->isKeyDown(OIS::KC_S))				localMoveDir.x -= 1;

		if (inSpace) {
			if (kb->isKeyDown(OIS::KC_SPACE))		localMoveDir.y += 1;
			if (kb->isKeyDown(OIS::KC_LCONTROL))	localMoveDir.y -= 1;
			if (kb->isKeyDown(OIS::KC_A))			localMoveDir.z -= 1;
			if (kb->isKeyDown(OIS::KC_D))			localMoveDir.z += 1;
		}

		else {
			if (kb->isKeyDown(OIS::KC_A)) { 
				ySpeed = 1;
				robotSceneNode->yaw(yawSpeed * deltaT * ySpeed);
			}
			if (kb->isKeyDown(OIS::KC_D)) {
				ySpeed = -1;
				robotSceneNode->yaw(yawSpeed * deltaT * ySpeed);
			}
		}

		// Check if we need to move.
        if (!localMoveDir.isZeroLength()) {

            // Normalize to obtain a pure direction vector.
            localMoveDir.normalise();
            
            // Multiply direction vector by movement speed.
            if (kb->isKeyDown(OIS::KC_LSHIFT))
                localMoveDir *= fastSpeed;
            else
                localMoveDir *= normalSpeed;

			// Switch animation state.
			if (!inSpace) 
				changeAnimation(animList[1]);

			// Convert local velocity vector to world coordinates	getOrientation().
			mMoveVelocity = robotSceneNode->getOrientation() * localMoveDir;

			// Get current position of the player's feet.
			Ogre::Vector3 pos = robotSceneNode->getPosition();

	        // Apply velocity to change position.
		    pos += deltaT * mMoveVelocity * 0.5f;
			robotSceneNode->setPosition(pos);

			//update listener attributes
			MusicManager::getInstance()->setListenerAttributes(robotSceneNode->getPosition(), mMoveVelocity,
				mCamera->getDirection(), mCamera->getUp());
		}

		else {
			// Switch animation state.
			if (!inSpace) 
				changeAnimation(animList[0]);
		}

		if (!inSpace) {

			updateCameraGoal(deltaT);

			// Make sure robot does not leave terrain.
			if(robotSceneNode->getPosition().x > 250)
				robotSceneNode->setPosition(250, 0, robotSceneNode->getPosition().z);
			if(robotSceneNode->getPosition().z > 250)
				robotSceneNode->setPosition(robotSceneNode->getPosition().x, 0, 250);
			if(robotSceneNode->getPosition().x < -250)
				robotSceneNode->setPosition(-250, 0, robotSceneNode->getPosition().z);
			if(robotSceneNode->getPosition().z < -250)
				robotSceneNode->setPosition(robotSceneNode->getPosition().x, 0, -250);

		} 

	}

	/** 
	 * This function updates the camera goal for the room gameplay.
	 * 
	 * @param deltaT			time since last frame
	 *
	 */
	void SpacePlayer::updateCameraGoal(Ogre::Real deltaT) {

		Ogre::Vector3 mGoalDirection = Ogre::Vector3::ZERO;

		if (localMoveDir != Ogre::Vector3::ZERO) {
			Ogre::Real toGoal = mCameraPivot->getOrientation().getYaw().valueDegrees();
			robotSceneNode->yaw(Ogre::Degree(toGoal) * turnSpeed * deltaT);
			mCameraPivot->yaw(-Ogre::Degree(toGoal) * turnSpeed * deltaT);
		}

	}


//=====================================================================================
//
// #Event Listeners
//
//=====================================================================================

	/**
	 * This function will handle all keyboard events, when the user strikes any key.
	 */
	bool SpacePlayer::keyPressed(const OIS::KeyEvent& arg) {

		if(arg.key == OIS::KC_ESCAPE){
			if(!inSpace){
				pausing = true;
			}
		}
		if(arg.key == OIS::KC_N){
			if(inSpace){
				Game::getInstance()->switchState(GameOverState::getInstance());
			}
		}

		if (arg.key == OIS::KC_E) {
			if(!inSpace) {
				robotMove = Ogre::Vector3::ZERO;
				Game::getInstance()->switchState(PlanetGameplayState::getInstance());
			}
		}

		// Pause.
		if (arg.key == OIS::KC_PAUSE) {
			Game::getInstance()->switchState(PauseMenuState::getInstance());
		}

		// Realloc 1.
		if (arg.key == OIS::KC_1) {
			Attributes::getInstance()->setCurrentReallocState(Attributes::getInstance()->getR1());
			selectReallocState(Attributes::getInstance()->getR1());
		}

		// Realloc 2.
		if (arg.key == OIS::KC_2) {
			Attributes::getInstance()->setCurrentReallocState(Attributes::getInstance()->getR2());
			selectReallocState(Attributes::getInstance()->getR2());
		}

		// Realloc 3.
		if (arg.key == OIS::KC_3) {
			Attributes::getInstance()->setCurrentReallocState(Attributes::getInstance()->getR3());
			selectReallocState(Attributes::getInstance()->getR3());
		}
	

		if(!inSpace) {

			if(arg.key == OIS::KC_Q ){
				initEquip(room->getParts());
			}

			if(arg.key == OIS::KC_M) {
				initBuild(room->getParts());
			}

		}

		return true;

	}

	/**
	 * This function will handle all keyboard events, when the user releases from any key.
	 */
	bool SpacePlayer::keyReleased(const OIS::KeyEvent& arg) {
		return true;
	}

	/**
	 * This function will handle for when the mouse moves.
	 */
	bool SpacePlayer::mouseMoved(const OIS::MouseEvent& arg) {

		Ogre::Degree rotSpeed(gFreelookMouseSpeed * gMouseSensitivity);

		// Get mouse displacement in screen space.
		int xDisp = arg.state.X.rel;
		int yDisp = arg.state.Y.rel;

		if (inSpace) {
		
			if (xDisp != 0) {
				// compute yaw amount for this frame
				Ogre::Degree yawAmt = -(Ogre::Real)xDisp * rotSpeed;
				// use player root node to yaw
				robotSceneNode->yaw(yawAmt, Ogre::Node::TS_WORLD);
			}
			if (yDisp != 0) {
				// compute pitch amount for this frame
				Ogre::Degree pitchAmt = -(Ogre::Real)yDisp * rotSpeed;

				// get current pitch
				Ogre::Degree currPitch = robotSceneNode->getOrientation().getPitch();

				Ogre::Degree mMinPitch = Ogre::Degree(-75);
				Ogre::Degree mMaxPitch = Ogre::Degree(75);

				if(!inSpace) {
					mMinPitch = Ogre::Degree(-25);
					mMaxPitch = Ogre::Degree(25);
				}

				// Limit the pitch amount to stay within a valid range.
				if (currPitch + pitchAmt > mMaxPitch)
					pitchAmt = mMaxPitch - currPitch;
				else if (currPitch + pitchAmt < mMinPitch)
					pitchAmt = mMinPitch - currPitch;
				
				// use player head node to roll.
				robotSceneNode->roll(pitchAmt);
			}
		}

		else {
			Ogre::Degree yawAmt = -(Ogre::Real)xDisp * rotSpeed;
			mCameraPivot->yaw(yawAmt);
		}

		return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked.
	 */
	bool SpacePlayer::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {

		// If the player is in space, FIRE!
		if (inSpace) {

			// Both arm parts.
			if (id == OIS::MB_Left) {
				mLeftArmPart->getSkill()->executeLeftSkill();
				mRightArmPart->getSkill()->executeRightSkill();
			}

			// Head/shoulder parts.
			else if(id == OIS::MB_Right) {
				if (shouldShootAgain) {
					shouldShootAgain = false;
					shoulderPartsCooldown = Game::getInstance()->getTime() + 2;
					//mHeadPart->getSkill()->executeHeadSkill();
					mLeftShoulderPart->getSkill()->executeLeftShoulderSkill();
					Attributes::getInstance()->incrementHitCount();
					mRightShoulderPart->getSkill()->executeRightShoulderSkill();
					Attributes::getInstance()->incrementHitCount();
				}
			}

		}

		return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked is over.
	 */
	bool SpacePlayer::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		return true;
	}
	

//=====================================================================================
//
// #Room Gameplay
// Both of these init methods loop through the parts vector and checks if any of the 
// parts meet the requirements necessary.
//
//======================================================================================

	/**
	 * This function checks if the player is close to a part.
	 *
	 * @param parts			a vector of parts that the robot can equip
	 *
	 */
	void SpacePlayer::initEquip(std::vector<Part*> parts){
		Part* part = room->getClosestPart();
		if (part->getPlayerClose() && RoomGameplayState::getInstance()->getRoomId() == 1) {
			equipPart(part);
		}
	}

	/**
	 * Checks if the player is close to a part and checks if the part is a blueprint.
	 *
	 * @param parts			a vector of parts that the robot can equip
	 *
	 */
	void SpacePlayer::initBuild(std::vector<Part*> parts){

		// Loop through all the parts.
		for(unsigned i = 0; i < parts.size(); i++) {

			// If the player is close enough.
			//if(parts[i]->getPlayerClose() && !parts[i]->getExists())
			if(parts[i]->getPlayerClose() && RoomGameplayState::getInstance()->getRoomId() == 2) {
				buildPart(parts[i]);
			}
		}


	}

	/** 
	 * Equips the part where appropriate.
	 *
	 * @param part			a specific part that the robot will equip
	 *
	 */
	void SpacePlayer::equipPart(Part* part) {
		
		// Only plays the sound when equipping a new part.
		if(!part->getIsEquipped())
			MusicManager::getInstance()->playEquipSound();

		if(part->getPartType() == HEAD_PART) {
			//Attributes::getInstance()->getCurrentReallocState()->setHeadPart((CompletePart*)part);
			CompletePart* temp = Attributes::getInstance()->getCurrentReallocState()->getHeadPart();
			Attributes::getInstance()->getCurrentReallocState()->setHeadPart(new CompletePart(*(CompletePart*)part));
			part->setIsEquipped(true);
			delete temp;
		}

		else if(part->getPartType() == LEFT_ARM_PART) {
			//Attributes::getInstance()->getCurrentReallocState()->setLeftArmPart((CompletePart*)part);
			CompletePart* temp = Attributes::getInstance()->getCurrentReallocState()->getLeftArmPart();
			Attributes::getInstance()->getCurrentReallocState()->setLeftArmPart(new CompletePart(*(CompletePart*)part));
			part->setIsEquipped(true);
			delete temp;
		}

		else if(part->getPartType() == RIGHT_ARM_PART) {
			//Attributes::getInstance()->getCurrentReallocState()->setRightArmPart((CompletePart*)part);
			CompletePart* temp = Attributes::getInstance()->getCurrentReallocState()->getRightArmPart();
			Attributes::getInstance()->getCurrentReallocState()->setRightArmPart(new CompletePart(*(CompletePart*)part));
			part->setIsEquipped(true);
			delete temp;
		}

		else if(part->getPartType() == LEFT_SHOULDER_PART){
			//Attributes::getInstance()->getCurrentReallocState()->setLeftShoulderPart((CompletePart*)part);
			//Attributes::getInstance()->getCurrentReallocState()->getLeftShoulderPart()->setIsEquipped(false);
			CompletePart* temp = Attributes::getInstance()->getCurrentReallocState()->getLeftShoulderPart();
			Attributes::getInstance()->getCurrentReallocState()->setLeftShoulderPart(new CompletePart(*(CompletePart*)part));
			part->setIsEquipped(true);
			delete temp;
		}

		else if(part->getPartType() == RIGHT_SHOULDER_PART){
			//Attributes::getInstance()->getCurrentReallocState()->setRightShoulderPart((CompletePart*)part);
			CompletePart* temp = Attributes::getInstance()->getCurrentReallocState()->getRightShoulderPart();
			Attributes::getInstance()->getCurrentReallocState()->setRightShoulderPart(new CompletePart(*(CompletePart*)part));
			part->setIsEquipped(true);
			delete temp;
		}

		selectReallocState(Attributes::getInstance()->getCurrentReallocState());

	}

	/** 
	 * This function checks if the player has the resources necessary to create the part.
	 * Then decrements what the player is holding by the necessary amount.
	 * Finally, calls the buildPart method from the room class.
	 *
	 * @param part			a specific part that the robot will equip
	 *
	 */
	void SpacePlayer::buildPart(Part* part){

		// Determine if the player enough resources for each material type.
		bool metalCheck = Attributes::getInstance()->getMetal() >= part->getMetal();
		bool matterCheck = Attributes::getInstance()->getDarkMatter() >= part->getDarkMatter();
		bool carbonCheck = Attributes::getInstance()->getCarbon() >= part->getCarbon();

		// Boolean to determine if the player has all the resources required.
		bool check = metalCheck && matterCheck && carbonCheck;

		// Check to see if the player has enough resources.
		if(check) {

			MusicManager::getInstance()->playBuildSound();

			// Reduces the resources.
			Attributes::getInstance()->decrementResources(part->getMetal(), part->getDarkMatter(), part->getCarbon());
			
			// Build the part.
			room->buildPart(part);

		}

	}

	/**
	 * Makes sure that only one ReallocState is selected at one time.
	 *
	 * @param reallocState	pointer to the realloc state to go to
	 *
	 */
	void SpacePlayer::selectReallocState(ReallocState* reallocState) {

		ReallocState* r1 = Attributes::getInstance()->getR1();
		ReallocState* r2 = Attributes::getInstance()->getR2();
		ReallocState* r3 = Attributes::getInstance()->getR3();

		// If no realloc states are selected.
		if(r1->getIsSelected() == false && r2->getIsSelected() == false && r3->getIsSelected() == false) {
			// Set activate realloc state.
			reallocState->setIsSelected(true);
			realloc(reallocState);
			Attributes::getInstance()->setCurrentReallocState(reallocState);
		}

		else {

			// Set all realloc states to false.
			r1->setIsSelected(false);
			r2->setIsSelected(false);
			r3->setIsSelected(false);

			// Remove all objects from the robot.
			headNode->detachAllObjects();
			leftArmNode->detachAllObjects();
			rightArmNode->detachAllObjects();
			leftShoulderNode->detachAllObjects();
			rightShoulderNode->detachAllObjects();

			r1->setEquippedPartsFlags(false);
			r2->setEquippedPartsFlags(false);
			r3->setEquippedPartsFlags(false);

			// Set active realloc state.
			reallocState->setIsSelected(true);
			realloc(reallocState);
			Attributes::getInstance()->setCurrentReallocState(reallocState);
			Attributes::getInstance()->getCurrentReallocState()->setEquippedPartsFlags(true);
			assignLocalParts();
			MusicManager::getInstance()->playEquipSound();
		}
	}

	/**
	 * This function updates the local part pointers.
	 */
	void SpacePlayer::assignLocalParts(){

		CompletePart* head = Attributes::getInstance()->getCurrentReallocState()->getHeadPart();
		CompletePart* leftA = Attributes::getInstance()->getCurrentReallocState()->getLeftArmPart();
		CompletePart* rightA = Attributes::getInstance()->getCurrentReallocState()->getRightArmPart();
		CompletePart* leftS = Attributes::getInstance()->getCurrentReallocState()->getLeftShoulderPart();
		CompletePart* rightS = Attributes::getInstance()->getCurrentReallocState()->getRightShoulderPart();

		// Deep copy and pass in the level and the player.
		mHeadPart = new CompletePart(*head, level, this);
		mLeftArmPart = new CompletePart(*leftA, level, this);
		mRightArmPart = new CompletePart(*rightA, level, this);
		mLeftShoulderPart = new CompletePart(*leftS, level, this);
		mRightShoulderPart = new CompletePart(*rightS, level, this);
	}

	/**
	 * This function attaches parts to the corresponding bone.
	 *
	 * @param reallocState			which realloc state to attach the part to
	 *
	 */
	void SpacePlayer::realloc(ReallocState* reallocState){

		if(reallocState->getHeadPart()->getSceneNode() != NULL){
			reallocState->getHeadPart()->getSceneNode()->detachAllObjects();
			headNode->attachObject(reallocState->getHeadPart()->getEntity());
			headNode->setScale(reallocState->getHeadPart()->getSceneNode()->getScale());
			headNode->setOrientation(reallocState->getHeadPart()->getSceneNode()->getOrientation());
		}
			
		if(reallocState->getLeftArmPart()->getSceneNode() != NULL){
			reallocState->getLeftArmPart()->getSceneNode()->detachAllObjects();
			leftArmNode->attachObject(reallocState->getLeftArmPart()->getEntity());
			leftArmNode->setScale(reallocState->getLeftArmPart()->getSceneNode()->getScale());
			leftArmNode->setOrientation(reallocState->getLeftArmPart()->getSceneNode()->getOrientation());
		}
		
		if(reallocState->getRightArmPart()->getSceneNode() != NULL){
			reallocState->getRightArmPart()->getSceneNode()->detachAllObjects();
			rightArmNode->attachObject(reallocState->getRightArmPart()->getEntity());
			rightArmNode->setScale(reallocState->getRightArmPart()->getSceneNode()->getScale());
			rightArmNode->setOrientation(reallocState->getRightArmPart()->getSceneNode()->getOrientation());
		}
		
		if(reallocState->getLeftShoulderPart()->getSceneNode() != NULL){
			reallocState->getLeftShoulderPart()->getSceneNode()->detachAllObjects();
			leftShoulderNode->attachObject(reallocState->getLeftShoulderPart()->getEntity());
			leftShoulderNode->setScale(reallocState->getLeftShoulderPart()->getSceneNode()->getScale());
			leftShoulderNode->setOrientation(reallocState->getLeftShoulderPart()->getSceneNode()->getOrientation());
		}

		if(reallocState->getRightShoulderPart()->getSceneNode() != NULL){
			reallocState->getRightShoulderPart()->getSceneNode()->detachAllObjects();
			rightShoulderNode->attachObject(reallocState->getRightShoulderPart()->getEntity());
			rightShoulderNode->setScale(reallocState->getRightShoulderPart()->getSceneNode()->getScale());
			rightShoulderNode->setOrientation(reallocState->getRightShoulderPart()->getSceneNode()->getOrientation());
		}
	}

	/**
	 * This function sets the reallocStates in space to what was equipped in the room.
	 */
	void SpacePlayer::setCurrentReallocState(){
		selectReallocState(Attributes::getInstance()->getCurrentReallocState());
	}

	/**
	 * This function will apply damage to the player.
	 *
	 * @param damageAmount			the amount of damage that will dealt to the player		
	 *
	 */
	void SpacePlayer::applyDamage(Ogre::Real damageAmount) {

        mHealth -= damageAmount;
        if (mHealth < 0) {
            mHealth = 0;
		}

		// Set the health bar.
		CEGUI::ProgressBar*	bar = SpaceGameplayState::getInstance()->getHealthBar();
		bar->setProgress(mHealth/MAX_HEALTH);

    }

}