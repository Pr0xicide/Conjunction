#ifndef capstone_PLANETPLAYER_H_
#define capstone_PLANETPLAYER_H_

#include "Player.h"

namespace capstone {

	// Foward declarations.
	class PlanetLevel;

	// Constants.
	#define NUM_ANIMS 13           // number of animations the character has
	#define CHAR_HEIGHT 5          // height of character's center of mass above ground
	#define CAM_HEIGHT 2           // height of camera above character's center of mass
	#define RUN_SPEED 20           // character running speed in units per second
	#define TURN_SPEED 500.0f      // character turning in degrees per second
	#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
	#define JUMP_ACCEL 40.0f       // character jump acceleration in upward units per squared second
	#define GRAVITY 90.0f          // gravity in downward units per squared second

	/**
	 * This class provides the player when the robot is on the planet.
	 * NOTE - using Sinbad for now (Jayvin)
	 */
	class PlanetPlayer : public Player {

		// Define animation states for sinbad.
		enum AnimID
		{
			ANIM_IDLE_BASE,
			ANIM_IDLE_TOP,
			ANIM_RUN_BASE,
			ANIM_RUN_TOP,
			ANIM_HANDS_CLOSED,
			ANIM_HANDS_RELAXED,
			ANIM_DRAW_SWORDS,
			ANIM_SLICE_VERTICAL,
			ANIM_SLICE_HORIZONTAL,
			ANIM_DANCE,
			ANIM_JUMP_START,
			ANIM_JUMP_LOOP,
			ANIM_JUMP_END,
			ANIM_NONE
		};

		// Enum for each part.
		AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
		AnimID mTopAnimID;                    // current top (upper-body) animation

		// Player properties.
		Ogre::Vector3 direction;
		static Ogre::Real moveSpeed;

		// Pointers for the forward declared classes.
		PlanetLevel* level;

		bool mFadingIn[NUM_ANIMS];            // which animations are fading in
		bool mFadingOut[NUM_ANIMS];           // which animations are fading out
		bool mSwordsDrawn;

		// Player properties.
		Ogre::Entity* robotEntity;
		Ogre::Entity* mSword1;
		Ogre::Entity* mSword2;

		Ogre::SceneNode* robotSceneNode;
		Ogre::SceneNode* mCameraPivot;
		Ogre::SceneNode* mCameraGoal;
		Ogre::SceneNode* mCameraNode;

		Ogre::Real mPivotPitch;
		Ogre::AnimationState* mAnims[NUM_ANIMS];    // master animation list

		Ogre::Vector3 mKeyDirection;      // player's local intended direction based on WASD keys
		Ogre::Vector3 mGoalDirection;     // actual intended direction in world-space
		
		Ogre::Real mVerticalVelocity;     // for jumping
		Ogre::Real mTimer;                // general timer to see how long animations have been playing

	public:

		// Constructor and destructor.
		PlanetPlayer(Ogre::Camera* camera, PlanetLevel* level);
		~PlanetPlayer();

		// Main update method/function.
		void update(Ogre::Real deltaT);

	private:

		// Initialization methods/functions.
		void setupBody(Ogre::SceneManager* sceneMgr);
		void setupCamera(Ogre::Camera* camera);
		void setupAnimations();
		void setBaseAnimation(AnimID id, bool reset);
		void setTopAnimation(AnimID id, bool reset);

		// Sub update methods/functions.
		void updateBody(Ogre::Real deltaT);
		void updateAnimations(Ogre::Real deltaT);
		void updateCamera(Ogre::Real deltaT);
		void updateCameraGoal(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom);
		void fadeAnimations(Ogre::Real deltaT);

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