#ifndef capstone_ROOMPLAYER_H_
#define capstone_ROOMPLAYER_H_

#include "Player.h"

namespace capstone {

	/**
	 * Class definition for when the player is in any of the rooms.
	 */
	class RoomPlayer : public Player {

		// Instance variables.
		static Ogre::Real				camFoward;
		static Ogre::Real				camHeight;
		static Ogre::Real				camSideways;
		static Ogre::Degree				yaw;
		static Ogre::Degree				pitch;
		static Ogre::Real				turnSpeed;

		AnimationState*					animState;
		std::vector<AnimationState*>	animList;
		std::vector<Ogre::SceneNode*>	nodes;

		int								roomID;

		// Variables for the robot player.
		Ogre::Entity*					mEntity;
		Ogre::SceneNode*				mSceneNode;

		// Room camera.
		Ogre::SceneManager*				sceneMgr;
		Ogre::SceneNode*				cameraSceneNode;
		Ogre::SceneNode*				cameraPivot;

		// Three types of parts for now.
		Ogre::Bone*						mHeadBone;
		Ogre::Bone*						mLeftArmBone;
		Ogre::Bone*						mRightArmBone;

		// Nodes for place the parts onto.
		std::vector<Ogre::SceneNode*>	nodesList;
		Ogre::SceneNode*				headNode;
		Ogre::SceneNode*				leftArmNode;
		Ogre::SceneNode*				rightArmNode;
		Ogre::SceneNode*				leftShoulderNode;
		Ogre::SceneNode*				rightShoulderNode;

		// Nodes for spawning projectiles and such.
		Ogre::SceneNode*				headSpawnNode;
		Ogre::SceneNode*				leftSpawnNode;
		Ogre::SceneNode*				rightSpawnNode;
		Ogre::SceneNode*				leftShoulderSpawnNode;
		Ogre::SceneNode*				rightShoulderSpawnNode;

	public:

		// Constructor & destructor.
		RoomPlayer(Ogre::Camera* camera, int roomID);
		~RoomPlayer();

		// Update/Render function.
		void update(Ogre::Real deltaT);

	private:

		// Initialization methods/functions.
		void setupBody(Ogre::SceneManager* sceneMgr);
		void setupCamera(Ogre::Camera* camera);
		void setupAnimations();

		// OIS::KeyListener callbacks.
		bool keyPressed(const OIS::KeyEvent& arg);
		bool keyReleased(const OIS::KeyEvent& arg);

		// OIS::MouseListener callbacks.
		bool mouseMoved(const OIS::MouseEvent& arg);
		bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	};

}

#endif