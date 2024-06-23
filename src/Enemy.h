#ifndef capstone_ENEMY_H_
#define capstone_ENEMY_H_

#include "StateMachineBase.h"
#include "IdleState.h"
#include "DieState.h"
#include "DeadState.h"
#include "WanderState.h"
#include "AttackState.h"
#include "ApproachState.h"

namespace capstone {

	// Forward declared classes.
	class SpaceLevel;

	/**
	 * This class is the parent/base class of all enemies.
	 */
    class Enemy {

		// Declare instance variables.
		bool					mIsAlive;
		int						health;
		std::string				meshName;

		SpaceLevel*				level;

		Ogre::Entity*			entity;
		Ogre::Entity*			oentity; //AC
		Ogre::Real				height;
		Ogre::Real				hitRadius;
		Ogre::Real				timeSinceLastShot;
		Ogre::SceneNode*		enemyNode;
		Ogre::SceneNode*		ogreHead; //AC
		Ogre::SceneNode*		projNode;
		Ogre::Vector3			position;
		Ogre::Vector3			prevVel;

		// Used for timing chain explosions while dying.
        static Ogre::Real       msMinChainExplosionDelay;
        static Ogre::Real       msMaxChainExplosionDelay;
		
		// Sounds
		FMOD::Channel* mJetstreamChannel;

	public:
		StateMachineBase*		mState;

		//Player in Range
		bool CheckPlayerInRange();

		//Idle Method.
		void Idle(Ogre::Real deltaT);

		//Wander Method.
		void Wander(Ogre::Real deltaT);

		//Die Method.
		void Die();

		//Attack
		void Attack(Ogre::Real deltaT);

		//Approach
		void Approach(Ogre::Real deltaT);

		//Drop material
		void DropMaterial();

		void Runaway(Ogre::Real deltaT);

		// AI states enum
		enum AIState {
			AI_INIT,        // initial state (used only at creation)
            AI_IDLE,        // idle state (standing, looking around)
            AI_WANDER,      // wander state (walking around)
            AI_CHASE,       // move towards player
		};

        // Constructor and destructor.
        Enemy(SpaceLevel* level, std::string meshName, Ogre::Vector3 position,
			  Ogre::Real speed, Ogre::Degree fov, Ogre::Real visionRange,
			  Ogre::Degree turnSpeed, int health);
        ~Enemy();

		// Update.
		virtual void update(Ogre::Real deltaT);

		// Getters.
		bool isPlayerAlive();
		bool isAlive() const										{ return mIsAlive; }
		bool canSee(const Ogre::SceneNode* target) const;
        bool canSeePlayer() const;
        bool canHear(const Ogre::SceneNode* target) const;
        bool canHearPlayer() const;
		bool canAssist() const;
		bool canFire() const;
        bool canDetect(const Ogre::SceneNode* target) const			{ return canHear(target) || canSee(target); }
        bool canDetectPlayer() const								{ return canHearPlayer() || canSeePlayer(); }
		Ogre::Real getHeight() const								{ return height; }
		Ogre::Real getHitRadius() const								{ return hitRadius; }
		Ogre::SceneNode* getSceneNode()								{ return enemyNode; }
		Ogre::Vector3 getEnemyPosition() const						{ return enemyNode->getPosition(); }
		Ogre::Vector3 getCentroid() const
		{
            Ogre::Vector3 pos = enemyNode->getPosition();
			//2.25 is the height that needs to be modified
            return Ogre::Vector3(pos.x, pos.y + 0.5f * 2.25f, pos.z);
        }

		// Setters.
		void setAIState(AIState state);
		void applyDamage(int amount);
		void stopJetSound();
		void setupSounds();

	private:

		AIState			mAIState;
		Ogre::Real		mSpeed;
		Ogre::Degree	mFOV;
		Ogre::Real		mVisionRange;
		Ogre::Real		mHearingRange;
		Ogre::Real		mAggroRange;
		Ogre::Real		mFireRange;
		Ogre::Degree	mTurnSpeed;

		// Abstract methods/functions.
		virtual void setupBody(Ogre::SceneManager* sceneMgr);
		virtual void setupAnimations();
		
	};

}

#endif