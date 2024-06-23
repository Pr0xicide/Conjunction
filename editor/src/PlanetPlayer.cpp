#include "stdafx.h"

#include "Player.h"
#include "PlanetPlayer.h"
#include "PlanetLevel.h"

namespace capstone {

	Ogre::Real PlanetPlayer::moveSpeed = 25;

	/** 
	 * PlanetPlayer constructor.
	 * Parameters:
	 * camera - pointer to the camera in order to position it
	 */
	PlanetPlayer::PlanetPlayer(Ogre::Camera* camera, PlanetLevel* level) 
		: robotEntity(NULL)
		, robotSceneNode(NULL)
		, level(level)
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

		// Destroy entities and scene nodes.
		sceneMgr->destroySceneNode(robotSceneNode);
		sceneMgr->destroySceneNode(mCameraPivot);
		sceneMgr->destroySceneNode(mCameraGoal);
		sceneMgr->destroySceneNode(mCameraNode);

		sceneMgr->destroyEntity(robotEntity);
		sceneMgr->destroyEntity(mSword1);
		sceneMgr->destroyEntity(mSword2);
	}

	/**
	 * =====================================================================================
	 * #Initialization
	 * =====================================================================================
	 */

	/**
	 * This function will create the robot for the player.
	 * Parameters:
	 * sceneMgr - pointer to the scene manager to create objects
	 */
	void PlanetPlayer::setupBody(Ogre::SceneManager* sceneMgr) {

		// Create scene node.
		robotSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

		// Create entity for the robot.
		robotEntity = sceneMgr->createEntity("Sinbad.mesh");

		// Attach entity to scene node.
		robotSceneNode->attachObject(robotEntity);

		// Set position for the player.
		//robotSceneNode->setPosition(0,25,0);

		// Swords and attach them to the sheath.
		mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh"); // Left hand
		mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh"); // Right hand
		robotEntity->attachObjectToBone("Sheath.L", mSword1);
		robotEntity->attachObjectToBone("Sheath.R", mSword2);

		mKeyDirection = Ogre::Vector3::ZERO;
		mVerticalVelocity = 0;

	}

	/**
	 * This function will setup the camera so its behind the player.
	 * Parameters:
	 * camera - pointer to the game's current camera
	 */
	void PlanetPlayer::setupCamera(Ogre::Camera* camera) {
		
		// Create a pivot point.
		mCameraPivot = camera->getSceneManager()->getRootSceneNode()->createChildSceneNode();

		// Place where the camera should be.
		mCameraGoal = mCameraPivot->createChildSceneNode(Ogre::Vector3(0, 0, 15));

		// Where the camera actually is.
		mCameraNode = camera->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

		// Set fixed yaw axis.
		mCameraPivot->setFixedYawAxis(true);
		mCameraGoal->setFixedYawAxis(true);
		mCameraNode->setFixedYawAxis(true);

		// Reduce the clipping planes.
		camera->setNearClipDistance(0.1f);
		//camera->setFarClipDistance(100);
		mCameraNode->attachObject(camera);

		mPivotPitch = 0;
	}

	/**
	 * This function will setup all the animations for the player.
	 */
	void PlanetPlayer::setupAnimations() {
		
		// Animations are applied by calculating a weighted cumulative total.
		robotEntity->getSkeleton()->setBlendMode(Ogre::SkeletonAnimationBlendMode::ANIMBLEND_CUMULATIVE);

		// Define all animations for Sinbad.
		Ogre::String animNames[] =
		{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

		// Initialize the animation list.
		for (int i = 0; i < NUM_ANIMS; i++) {
			mAnims[i] = robotEntity->getAnimationState(animNames[i]);
			mAnims[i]->setLoop(true);
			mFadingIn[i] = false;
			mFadingOut[i] = false;
		}

		// Setup the initial animation state.
		setBaseAnimation(ANIM_IDLE_BASE, false);
		setTopAnimation(ANIM_IDLE_TOP, false);

		// "Relax" the hands since we're not holding anything.
		mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);

		// Set the default state the swords are not drawn.
		mSwordsDrawn = false;

	}

	/**
	 * This function will set the animation for the entire body for Sinbad.
	 * Parameters:
	 * id - which state the player is in
	 */
	void PlanetPlayer::setBaseAnimation(AnimID id, bool reset = false)
	{
		if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS) {
			// If we have an old animation, fade it out.
			mFadingIn[mBaseAnimID] = false;
			mFadingOut[mBaseAnimID] = true;
		}

		mBaseAnimID = id;

		if (id != ANIM_NONE) {
			// If we have a new animation, enable it and fade it in.
			mAnims[id]->setEnabled(true);
			mAnims[id]->setWeight(0);
			mFadingOut[id] = false;
			mFadingIn[id] = true;
			if (reset) 
				mAnims[id]->setTimePosition(0);
		}
	}

	/**
	 * This function will set the animation for the upper torso for Sinbad.
	 * Parameters:
	 * id - which state the player is in
	 */
	void PlanetPlayer::setTopAnimation(AnimID id, bool reset = false)
	{
		if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS) {
			// If we have an old animation, fade it out.
			mFadingIn[mTopAnimID] = false;
			mFadingOut[mTopAnimID] = true;
		}

		mTopAnimID = id;

		if (id != ANIM_NONE) {
			// If we have a new animation, enable it and fade it in.
			mAnims[id]->setEnabled(true);
			mAnims[id]->setWeight(0);
			mFadingOut[id] = false;
			mFadingIn[id] = true;
			if (reset) mAnims[id]->setTimePosition(0);
		}
	}

	/**
	 * =====================================================================================
	 * #Update
	 * =====================================================================================
	 */

	/** 
	 * This function is the main update method that updates everything.
	 * Parameters:
	 * deltaT - time since last frame
	 */
	void PlanetPlayer::update(Ogre::Real deltaT) {
		updateBody(deltaT);
		updateAnimations(deltaT);
		updateCamera(deltaT);
	}

	/** 
	 * This function updates the body for the robot.
	 * Parameters:
	 * deltaT - time since last frame
	 */
	void PlanetPlayer::updateBody(Ogre::Real deltaT) {

		// Create an ogre vector variable used to calculate the goal direction.
		mGoalDirection = Ogre::Vector3::ZERO;

		if (mKeyDirection != Ogre::Vector3::ZERO && mBaseAnimID != ANIM_DANCE) {
			
			// Calculate the goal direction in world based on player's key directions.
			mGoalDirection += mKeyDirection.z * mCameraNode->getOrientation().zAxis();
			mGoalDirection += mKeyDirection.x * mCameraNode->getOrientation().xAxis();
			mGoalDirection.y = 0;
			mGoalDirection.normalise();

			// Create a quaternion in the direction.
			Ogre::Quaternion toGoal = robotSceneNode->getOrientation().zAxis().getRotationTo(mGoalDirection);

			// Calculate how much the character has to turn to face goal direction.
			Ogre::Real yawToGoal = toGoal.getYaw().valueDegrees();

			// How much the character CAN turn this frame.
			Ogre::Real yawAtSpeed = yawToGoal / Ogre::Math::Abs(yawToGoal) * deltaT * TURN_SPEED;

			// Reduce "turnability" if we're in midair.
			if (mBaseAnimID == ANIM_JUMP_LOOP) 
				yawAtSpeed *= 0.2f;

			// Turn as much as we can, but not more than we need to.
			if (yawToGoal < 0) 
				yawToGoal = std::min<Ogre::Real>(0, std::max<Ogre::Real>(yawToGoal, yawAtSpeed));

			else if (yawToGoal > 0) 
				yawToGoal = std::max<Ogre::Real>(0, std::min<Ogre::Real>(yawToGoal, yawAtSpeed));
			
			// Rotate node.
			robotSceneNode->yaw(Ogre::Degree(yawToGoal));

			// move in current body direction (not the goal direction)
			robotSceneNode->translate(0, 0, deltaT * RUN_SPEED * mAnims[mBaseAnimID]->getWeight(), Ogre::Node::TS_LOCAL);

		}

		// If the state is jumping.
		if (mBaseAnimID == ANIM_JUMP_LOOP) {

			// Add a vertical offset too, and apply gravity.
			robotSceneNode->translate(0, mVerticalVelocity * deltaT, 0, Ogre::Node::TS_LOCAL);
			mVerticalVelocity -= GRAVITY * deltaT;
			
			// Get position of the node.
			Ogre::Vector3 pos = robotSceneNode->getPosition();

			// If we've hit the ground, change to landing state.
			if (pos.y <= CHAR_HEIGHT) {
				pos.y = CHAR_HEIGHT;
				robotSceneNode->setPosition(pos);
				setBaseAnimation(ANIM_JUMP_END, true);
				mTimer = 0;
			}
		}


	}

	/** 
	 * This function updates the animations for the robot.
	 * Parameters:
	 * deltaT - time since last frame
	 */
	void PlanetPlayer::updateAnimations(Ogre::Real deltaT) {
		
		// Set speed of both parts.
		Ogre::Real baseAnimSpeed = 1;
		Ogre::Real topAnimSpeed = 1;

		// Increment timer.
		mTimer += deltaT;

		// If the swords are drawn.
		if (mTopAnimID == ANIM_DRAW_SWORDS) {
			// Flip the draw swords animation.
			topAnimSpeed = mSwordsDrawn ? -1.0f : 1.0f;

			// Half-way through the animation is when the hand grasps the handles.
			if (mTimer >= mAnims[mTopAnimID]->getLength() / 2 && mTimer - deltaT < mAnims[mTopAnimID]->getLength() / 2) {
				
				// Transfer the swords from the sheaths to the hands.
				robotEntity->detachAllObjectsFromBone();
				robotEntity->attachObjectToBone(mSwordsDrawn ? "Sheath.L" : "Handle.L", mSword1);
				robotEntity->attachObjectToBone(mSwordsDrawn ? "Sheath.R" : "Handle.R", mSword2);

				// Change the hand state to grab or let go.
				mAnims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
				mAnims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);
			}

			// If the animation is finished, return to the orginal state.
			if (mTimer >= mAnims[mTopAnimID]->getLength()) {
				
				if (mBaseAnimID == ANIM_IDLE_BASE) 
					setTopAnimation(ANIM_IDLE_TOP);

				else {
					setTopAnimation(ANIM_RUN_TOP);
					mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
				}

				// Switch flag.
				mSwordsDrawn = !mSwordsDrawn;
			}
		}

		// If the current animation state is when the swords are slashed vertically.
		else if (mTopAnimID == ANIM_SLICE_VERTICAL || mTopAnimID == ANIM_SLICE_HORIZONTAL) {
			
			// If the animation is finished, return to the orginal state.
			if (mTimer >= mAnims[mTopAnimID]->getLength())
			{
				if (mBaseAnimID == ANIM_IDLE_BASE) 
					setTopAnimation(ANIM_IDLE_TOP);

				else {
					setTopAnimation(ANIM_RUN_TOP);
					mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
				}

			}

			// Prevent hips swaying from side to side when slicing.
			if (mBaseAnimID == ANIM_IDLE_BASE) 
				baseAnimSpeed = 0;

		}

		// If the current animation state is when the player is jumping.
		else if (mBaseAnimID == ANIM_JUMP_START) {

			// When the animation is finished.
			if (mTimer >= mAnims[mBaseAnimID]->getLength()) {
				setBaseAnimation(ANIM_JUMP_LOOP, true);

				// Apply a jump acceleration.
				mVerticalVelocity = JUMP_ACCEL;
			}
		}

		// If the current animation state is when the player is done jumping reaching the floor/terrain.
		else if (mBaseAnimID == ANIM_JUMP_END) {

			if (mTimer >= mAnims[mBaseAnimID]->getLength()) {

				// Once landed.
				if (mKeyDirection == Ogre::Vector3::ZERO) {
					setBaseAnimation(ANIM_IDLE_BASE);
					setTopAnimation(ANIM_IDLE_TOP);
				}

				else {
					setBaseAnimation(ANIM_RUN_BASE, true);
					setTopAnimation(ANIM_RUN_TOP, true);
				}
			}
		}

		// Increment the current base and top animation times.
		if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaT * baseAnimSpeed);
		if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(deltaT * topAnimSpeed);

		// Apply smooth transitioning between our animations.
		fadeAnimations(deltaT);

	}

	/** 
	 * This function updates the camera for the robot.
	 * Parameters:
	 * deltaT - time since last frame
	 */
	void PlanetPlayer::updateCamera(Ogre::Real deltaT) {

		// Place the camera pivot roughly at the character's shoulder.
		mCameraPivot->setPosition(robotSceneNode->getPosition() + Ogre::Vector3::UNIT_Y * CAM_HEIGHT);

		// Move the camera smoothly to the goal.
		Ogre::Vector3 goalOffSet = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
		mCameraNode->translate(goalOffSet * deltaT * 9.0f);

		// Always look at the pivot.
		mCameraNode->lookAt(mCameraPivot->_getDerivedPosition(), Ogre::Node::TS_WORLD);
		
	}

	/**
	 * This function will update the camera's goal position by either pitching or yawing BUT NOT ROLLING.
	 * Parameters:
	 * deltaYaw - difference from yawing
	 * deltaPitch - difference from pitching
	 * deltaZoom - difference from zooming
	 */
	void PlanetPlayer::updateCameraGoal(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom) {
		mCameraPivot->yaw(Ogre::Degree(deltaYaw), Ogre::Node::TS_WORLD);

		// Bound the pitch.
		if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0) && !(mPivotPitch + deltaPitch < -60 && deltaPitch < 0)) {
			mCameraPivot->pitch(Ogre::Degree(deltaPitch), Ogre::Node::TS_LOCAL);
			mPivotPitch += deltaPitch;
		}
		
		Ogre::Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
		Ogre::Real distChange = deltaZoom * dist;

		// Bound the zoom.
		if (!(dist + distChange < 8 && distChange < 0) && !(dist + distChange > 25 && distChange > 0))
			mCameraGoal->translate(0, 0, distChange, Ogre::Node::TS_LOCAL);
	}

	/**
	 * This function will fade the animations.
	 * Parameters:
	 * deltaT - time since last frame
	 */
	void PlanetPlayer::fadeAnimations(Ogre::Real deltaT)
	{
		// Loop through all animation states.
		for (int i = 0; i < NUM_ANIMS; i++)
		{
			// Slowly fade this animation in until it has full weight.
			if (mFadingIn[i])
			{
				Ogre::Real newWeight = mAnims[i]->getWeight() + deltaT * ANIM_FADE_SPEED;
				mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
				if (newWeight >= 1) mFadingIn[i] = false;
			}

			// Slowly fade this animation out until it has no weight, and then disable it.
			else if (mFadingOut[i])
			{
				Ogre::Real newWeight = mAnims[i]->getWeight() - deltaT * ANIM_FADE_SPEED;
				mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
				if (newWeight <= 0)
				{
					mAnims[i]->setEnabled(false);
					mFadingOut[i] = false;
				}
			}
		}
	}

	/**
	 * =====================================================================================
	 * #Event Listeners
	 * =====================================================================================
	 */

	/**
	 * This function will handle all keyboard events, when the user strikes any key.
	 */
	bool PlanetPlayer::keyPressed(const OIS::KeyEvent& arg) {

		// If 'Q' is pressed, take out the swords.
		if (arg.key == OIS::KC_Q && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)) {
			setTopAnimation(ANIM_DRAW_SWORDS, true);
			mTimer = 0;
		}

		else if (arg.key == OIS::KC_E) {
			std::cout << "E pressed" << std::endl;
			level->setLevelFinish();
		}

		// Track of the player's intended direction.
		else if (arg.key == OIS::KC_W) 
			mKeyDirection.z = -1;
		else if (arg.key == OIS::KC_A) 
			mKeyDirection.x = -1;
		else if (arg.key == OIS::KC_S) 
			mKeyDirection.z = 1;
		else if (arg.key == OIS::KC_D) 
			mKeyDirection.x = 1;

		// If spacebar key is pressed AND both parts are idle.
		else if (arg.key == OIS::KC_SPACE && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)) {
			// Jump if on ground.
			setBaseAnimation(ANIM_JUMP_START, true);
			setTopAnimation(ANIM_NONE);
			mTimer = 0;
		}

		// Start running if not already moving and the player wants to move.
		if (!mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_IDLE_BASE) {
			setBaseAnimation(ANIM_RUN_BASE, true);

			if (mTopAnimID == ANIM_IDLE_TOP) 
				setTopAnimation(ANIM_RUN_TOP, true);
		}

        return true;
	}

	/**
	 * This function will handle all keyboard events, when the user releases from any key.
	 */
	bool PlanetPlayer::keyReleased(const OIS::KeyEvent& arg) {
		// Track of the player's intended direction
		if (arg.key == OIS::KC_W && mKeyDirection.z == -1) 
			mKeyDirection.z = 0;
		else if (arg.key == OIS::KC_A && mKeyDirection.x == -1) 
			mKeyDirection.x = 0;
		else if (arg.key == OIS::KC_S && mKeyDirection.z == 1) 
			mKeyDirection.z = 0;
		else if (arg.key == OIS::KC_D && mKeyDirection.x == 1) 
			mKeyDirection.x = 0;

		if (mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_RUN_BASE) {
			// Stop running if already moving and the player doesn't want to move.
			setBaseAnimation(ANIM_IDLE_BASE);
			if (mTopAnimID == ANIM_RUN_TOP) setTopAnimation(ANIM_IDLE_TOP);
		}

        return true;
	}

	/**
	 * This function will handle for when the mouse moves.
	 */
	bool PlanetPlayer::mouseMoved(const OIS::MouseEvent& arg) {
		// Update camera goal based on mouse movement.
		updateCameraGoal(-0.05f * arg.state.X.rel, -0.05f * arg.state.Y.rel, -0.0005f * arg.state.Z.rel);

        return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked.
	 */
	bool PlanetPlayer::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		// If swords are out.
		if (mSwordsDrawn && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)) {
			if (id == OIS::MB_Left) 
				setTopAnimation(ANIM_SLICE_VERTICAL, true);
			else if (id == OIS::MB_Right) 
				setTopAnimation(ANIM_SLICE_HORIZONTAL, true);
			mTimer = 0;
		}

        return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked is over.
	 */
	bool PlanetPlayer::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		// If swords are out.
		if (mSwordsDrawn && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)) {
			if (id == OIS::MB_Left) 
				setTopAnimation(ANIM_SLICE_VERTICAL, true);
			else if (id == OIS::MB_Right) 
				setTopAnimation(ANIM_SLICE_HORIZONTAL, true);
			mTimer = 0;
		}

        return true;
	}

}