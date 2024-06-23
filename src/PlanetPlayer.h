#ifndef capstone_PLANETPLAYER_H_
#define capstone_PLANETPLAYER_H_

#include "Player.h"

namespace capstone {

	// Foward declarations.
	class PlanetLevel;

	/**
	 * This class provides the player when the robot is on the planet.
	 */
	class PlanetPlayer : public Player {

		// Class properties.
		static Ogre::Real				camFoward;
		static Ogre::Real				camHeight;
		static Ogre::Real				camSideways;
		static Ogre::Real				turnSpeed;
		static Ogre::Degree				yaw;
		static Ogre::Degree				pitch;

		bool							isAirborne;

		PlanetLevel*					level;

		Ogre::Entity*					robotEntity;

		Ogre::Degree					walkSpeed;
		Ogre::Degree					yawSpeed;

		Ogre::Real						walkAnimTime;
		Ogre::Real						jumpSpeed;
		Ogre::Real						wSpeed;
		Ogre::Real						ySpeed;

		std::vector<Ogre::SceneNode*>	nodeList;
		Ogre::SceneNode*				robotSceneNode;
		Ogre::SceneNode*				robotParentNode;
		Ogre::SceneNode*				cameraSceneNode;
		Ogre::SceneNode*				cameraPivot;

		Ogre::String					selectedLocation;

		Ogre::Vector3					jumpVelocity;
		Ogre::Vector3					keyDirection;

	public:

		// Constructor and destructor.
		PlanetPlayer(Ogre::Camera* camera, PlanetLevel* level);
		~PlanetPlayer();

		// Main update method/function.
		void update(Ogre::Real deltaT); 

		// Getters.
		Ogre::SceneNode*	getSceneNode()	{ return robotSceneNode; }

		// Setters.
		void resetProperties();

	private:

		// Initialization methods/functions.
		void setupBody(Ogre::SceneManager* sceneMgr);
		void setupCamera(Ogre::Camera* camera);
		void setupAnimations();

		void updateRobot(Ogre::Real deltaT);
		void updateCameraGoal(Ogre::Real deltaT);

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