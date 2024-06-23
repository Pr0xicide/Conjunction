#ifndef capstone_SPACEPLAYER_H_
#define capstone_SPACEPLAYER_H_

#include "Player.h"

namespace capstone {

	// Foward declarations.
	class SpaceLevel;
	class Room;
	class Part;
	class CompletePart;
	class ReallocState;

	class SpacePlayer : public Player {

		// Class properties.
		static Ogre::Degree				yaw;
		static Ogre::Degree				pitch;
		static Ogre::Real				zShoulderPosition;
		static Ogre::Real				yShoulderPosition;
		static Ogre::Real				camFoward;
		static Ogre::Real				camHeight;
		static Ogre::Real				camSideways;
		static Ogre::Real				turnSpeed;
		static Ogre::Real				MAX_HEALTH;

		bool							isAirborne;
		bool							inSpace;
		bool							shouldShootAgain;
		bool							pausing;

		Room*							room;
		SpaceLevel*						level;

		Ogre::Entity*					robotEntity;

		Ogre::SceneNode*				robotSceneNode;
		Ogre::SceneNode*				cameraSceneNode;
		Ogre::Camera*					mCamera;

		Ogre::Real						normalSpeed;
		Ogre::Real						fastSpeed;
		Ogre::Real						mHealth;
		Ogre::Real						ySpeed;
		Ogre::Real						shoulderPartsCooldown;
		Ogre::Degree					yawSpeed;

		Ogre::Vector3					localMoveDir;
		Ogre::Vector3					mMoveVelocity;
		Ogre::Vector3					jumpVelocity;

		// Robot to walk in Space
		Ogre::Vector3					robotMove;

		Ogre::Real						hitRadius;

		// Local pointers to the parts the player currently has equipped.
		std::vector<Part*>				parts;
		CompletePart*					mHeadPart;
		CompletePart*					mLeftArmPart;
		CompletePart*					mRightArmPart;
		CompletePart*					mLeftShoulderPart;
		CompletePart*					mRightShoulderPart;

		// Three types of parts for now.
		Ogre::Bone*						mHeadBone;
		Ogre::Bone*						mLeftArmBone;
		Ogre::Bone*						mRightArmBone;

		// Nodes for place the parts onto.
		std::vector<Ogre::SceneNode*>	nodes;
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

		// Room camera.
		Ogre::SceneNode*				mCameraPivot;

	public:

		// Constructor and destructor.
		SpacePlayer(Ogre::Camera* camera, SpaceLevel* level, Ogre::Degree yaw1 = Ogre::Degree(0), 
			Ogre::Real normalSpeed = 75, Ogre::Real fastSpeed = 300);
		SpacePlayer(Ogre::Camera* camera, Room* room, Ogre::Degree yaw1 = Ogre::Degree(0),
			Ogre::Real normalSpeed = 75, Ogre::Real fastSpeed = 300);
		~SpacePlayer();

		// Main update method/function.
		void update(Ogre::Real deltaT);

		// Getters.
		bool				getInSpace()						{ return inSpace; }
		bool				isAlive() const						{ return mHealth > 0; }	
		bool				isPausing() const					{ return pausing; }

		Ogre::Entity*		getRobotEntity()					{ return robotEntity; }

		Ogre::Real			getHealth() const					{ return mHealth; }

		Ogre::Real			getRadius() const					{ return hitRadius; }

		Ogre::SceneNode*	getSceneNode()						{ return robotSceneNode; }
		Ogre::SceneNode*	getHeadNode()						{ return headNode; }
		Ogre::SceneNode*	getLeftArmNode()					{ return leftArmNode; }
		Ogre::SceneNode*	getRightArmNode()					{ return rightArmNode; }
		Ogre::SceneNode*	getLeftShoulderNode()				{ return leftShoulderNode; }
		Ogre::SceneNode*	getRightShoulderNode()				{ return rightShoulderNode; }
		Ogre::SceneNode*	getLeftSpawnNode()					{ return leftSpawnNode; }
		Ogre::SceneNode*	getRightSpawnNode()					{ return rightSpawnNode; }
		Ogre::SceneNode*	getHeadSpawnNode()					{ return headSpawnNode; }
		Ogre::SceneNode*	getRightShoulderSpawnNode()			{ return rightShoulderSpawnNode; }
		Ogre::SceneNode*	getLeftShoulderSpawnNode()			{ return leftShoulderSpawnNode; }
		
		// Setters.
		void setCurrentReallocState();
		void setPausing(bool b)									{ pausing = b; }

		//Player getting dmg from other units
		void applyDamage(Ogre::Real damageAmount);

		void assignAttributesParts();

	private:

		// Initialization methods/functions.
		void setupBody(Ogre::SceneManager* sceneMgr);
		void setupCamera(Ogre::Camera* camera);
		void setupAnimations();

		// Helper update functions.
		void updateRobot(Ogre::Real deltaT);
		void updateCameraGoal(Ogre::Real deltaT);

		// Methods for switching realloc states.
		void selectReallocState(ReallocState* reallocState);
		void assignLocalParts();
		void realloc(ReallocState* reallocState);

		// Methods for equipping and building new parts in the room.
		void initEquip(std::vector<Part*> parts);
		void initBuild(std::vector<Part*> parts);
		void equipPart(Part* part);
		void buildPart(Part* part);

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