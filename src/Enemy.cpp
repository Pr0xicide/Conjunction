#include "stdafx.h"

#include "Enemy.h"
#include "GlobalVariables.h"
#include "Util.h"
#include "Explosions.h"
#include "SpaceLevel.h"
#include "SpacePlayer.h"
#include "Projectile.h"
#include "MusicManager.h"

namespace capstone {

	Ogre::Real Enemy::msMinChainExplosionDelay = 0.05f;
    Ogre::Real Enemy::msMaxChainExplosionDelay = 0.1f;

	/**
	 * Enemy constructor.
	 *
	 * @param level			pointer to the space level
	 * @param meshName		name of the mesh to be used
	 * @param position		position of the enemy in space
	 * @param speed			speed of the unit
	 * @param fov			unit's Field of Vision
	 * @param visionRange	the angle for the fov
	 * @param turnSpeed		how fast this unit can turn
	 * @param health		amount of health this enemy has
	 *
	 */
	Enemy::Enemy(SpaceLevel* level, std::string meshName, Ogre::Vector3 position,
			  Ogre::Real speed, Ogre::Degree fov, Ogre::Real visionRange,
			  Ogre::Degree turnSpeed, int health) 
		: level(level)
		, entity(NULL)
		, oentity(NULL)
		, enemyNode(NULL)
		, ogreHead(NULL)
		, projNode(NULL)
		, position(position)
		, meshName(meshName)
		, mSpeed(speed)
		, mFOV(fov)
		, mVisionRange(visionRange)	//AC found this not being useded what so ever
		, mAIState(AI_INIT)
		, mIsAlive(true)
		, health(health)
		, mAggroRange(70)
		, mFireRange(100)
		, timeSinceLastShot(0)
		, prevVel(Ogre::Vector3::ZERO)
	{
		setupBody(level->getSceneManager());
		setupAnimations();
		mState = new IdleState(0);
	}
	
	/**
	 * Enemy destructor.
	 */
	Enemy::~Enemy() {

		// Get scene manager from the PlanetLevel class.
		Ogre::SceneManager* sceneMgr = level->getSceneManager();

		// Destroy scene nodes.
		sceneMgr->destroySceneNode(enemyNode);
		sceneMgr->destroySceneNode(projNode);

		// Destroy entities.
		sceneMgr->destroyEntity(entity);

	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function is used to create the model for the enemy.
	 *
	 * @param sceneMgr		pointer to scene manager to create the model
	 *
	 */
	void Enemy::setupBody(Ogre::SceneManager* sceneMgr) {

		// Create a scene node for the enemy.
		enemyNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		//AC
		//ogreHead = sceneMgr->getRootSceneNode()->createChildSceneNode();

		projNode = enemyNode->createChildSceneNode();

		// Attach model to the enemy character.
		entity = sceneMgr->createEntity(meshName);
		//AC
		//oentity = sceneMgr->createEntity("ogrehead.mesh");

		Ogre::Vector3 center = entity->getBoundingBox().getCenter();
		Ogre::Vector3 max = entity->getBoundingBox().getMaximum();
		height = max.y - center.y;
		hitRadius = (height * 2) * 0.7f;

		// Attach entity to scene node.
		enemyNode->attachObject(entity);
		//AC
		//ogreHead->attachObject(oentity);
		// Prevent enemy from rolling when tracking.
		enemyNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);

		// Set position.
		enemyNode->setPosition(position);
		//ogreHead->setPosition(position);	//AC
		projNode->setPosition(position);

		// Create a point light for this enemy so that players can see them.
		Ogre::Light* pointLight = sceneMgr->createLight();
		pointLight->setType(Ogre::Light::LT_POINT);
		pointLight->setPosition(0,50,50);
		pointLight->setDiffuseColour(Ogre::ColourValue(255, 255, 255));
		pointLight->setSpecularColour(Ogre::ColourValue(255, 255, 255));
		pointLight->setAttenuation(10000000000, 1, 0, 0);
		enemyNode->attachObject(pointLight);

		// set initial AI state
        setAIState(AI_IDLE);
	}
	
	void Enemy::setupSounds(){
		mJetstreamChannel = MusicManager::getInstance()->playJetstreamSound(enemyNode->getPosition(), Ogre::Vector3::ZERO, mJetstreamChannel);
	}

	void Enemy::stopJetSound(){
		mJetstreamChannel = MusicManager::getInstance()->stopJetstreamSound(mJetstreamChannel);
	}

	/**
	 * This funciton sets up all the animations for the enemy unit.
	 */
	void Enemy::setupAnimations() {

	}


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This funciton updates the enemy.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void Enemy::update(Ogre::Real deltaT) {

		// get current game time
        Ogre::Real t = level->getTime();

		// get enemy's velocity
		Ogre::Vector3 vel;
		if(deltaT > 0)
			vel = (enemyNode->getPosition() - prevVel) / deltaT;
		else
			vel = Ogre::Vector3::ZERO;

		mJetstreamChannel = MusicManager::getInstance()->setAttributes(enemyNode->getPosition(), vel, mJetstreamChannel);
		//std::cout << "Enemy Position: " << enemyNode->getPosition() << std::endl;

		prevVel = enemyNode->getPosition();

        // perform an update depending on which AI state the robot is currently in
        //
       /*switch (mAIState) {

		case AI_IDLE:
			
			if (canDetectPlayer()) {
				setAIState(AI_CHASE);
				break;
			}

			break;

		case AI_CHASE:
			//currently lacking enemy reorientation towards player, math in util need to be done for vector3
			SpacePlayer* player = level->getPlayer();
	
			const Ogre::Vector3& pos = enemyNode->getPosition();
            const Ogre::Vector3& playerPos = player->getHeadNode()->getPosition();

			Ogre::Vector3 toTarget = playerPos - pos;

			toTarget.normalise();

			enemyNode->translate(mSpeed * deltaT * toTarget.UNIT_SCALE, Ogre::Node::TS_LOCAL);
		}*/
	}


// =====================================================================================
//
// #Combat Handling
//
// =====================================================================================

	/**
	 * This funciton updates the enemy.
	 *
	 * @param amount		amount of damage to take away from
	 *
	 */
	void Enemy::applyDamage(int amount) {

		// Decrement health.
		health -= amount;

		std::cout << health << std::endl;

		// Determine if this enemy is still alive.
		if (health <= 0) {
			health = 0;
			mIsAlive = false;
		}
		
	}


// =====================================================================================
//
// #AI States
//
// =====================================================================================
	
	/**
	 * This function will set this unit's AI state.
	 *
	 * @param			the new AI state to enter
	 *
	 */
	void Enemy::setAIState(AIState nextAIState) {
		// refuse to re-enter current state
        if (nextAIState == mAIState)
            return;

        // get current game time
        Ogre::Real t = level->getTime();

        // try to enter the given state
        //

		switch (nextAIState) {

			case AI_IDLE:  //---- try to enter the Idle state ----//

				// state change complete
				mAIState = AI_IDLE;
				break;

			case AI_WANDER:  //---- try to enter the Walk state ----//

				// state change complete
				mAIState = AI_WANDER;
				break;

			case AI_CHASE:

				mAIState = AI_CHASE;
		}
	}

	/**
	 * This function will execute this unit's idle state.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void Enemy::Idle(Ogre::Real deltaT) {
		// Can't become idle if not alive.
		if (isAlive()) {
			mSpeed = 0;
			enemyNode->translate(mSpeed * deltaT * gForward, Ogre::Node::TS_LOCAL);
		}
	}

	/**
	 * This function will execute this unit's wander state.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void Enemy::Wander(Ogre::Real deltaT) {

		// Pick variable turn speed in degrees per second (biased towards turning left)
		Ogre::Degree turnSpeed(Ogre::Math::RangeRandom(-45, 90));

		// Turn the enemy.
		enemyNode->yaw(deltaT * turnSpeed);
		//ogreHead->yaw(deltaT * turnSpeed);//AC
		// Move forward
		enemyNode->translate(20 * deltaT * gForwardZ, Ogre::Node::TS_LOCAL);
		//AC
		//ogreHead->translate(20 * deltaT * gForwardZ, Ogre::Node::TS_LOCAL);
	
	}

	/**
	 * This function will execute this unit's attack state.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void Enemy::Attack(Ogre::Real deltaT){

		if(timeSinceLastShot < level->getTime()){

			projNode->setPosition(enemyNode->_getDerivedPosition());
			projNode->setOrientation(enemyNode->_getDerivedOrientation());
			projNode->yaw(Ogre::Degree(-90));

			Ogre::Vector3 pos = projNode->getPosition();
			Ogre::Quaternion ori = projNode->getOrientation();

			std::cout << "Fire!!" << std::endl;

			MusicManager::getInstance()->playProjectileSound();

			Projectile* p = new Projectile(level, this, pos, ori, 50, "Flare", 350, 0, Ogre::ColourValue(4, 4, 4), 7, 10);
			level->addObject(p);

			timeSinceLastShot = level->getTime() + 3;
		}

	}

	/**
	 * This function will execute this unit's approach state.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void Enemy::Approach(Ogre::Real deltaT){
		SpacePlayer* player = level->getPlayer();

        const Ogre::Vector3& pos = enemyNode->getPosition();
		const Ogre::Vector3& playerPos = player->getSceneNode()->_getDerivedPosition();

		enemyNode->lookAt(playerPos, Ogre::Node::TS_PARENT, Ogre::Vector3::UNIT_Z);

		//This allows the Enemy to follow the player
        Ogre::Vector3 toTarget = playerPos - pos;	
        toTarget.normalise();

		enemyNode->translate(30* deltaT * toTarget, Ogre::Node::TS_WORLD);
	}


	/**
	 * This function will execute this unit's runaway state.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void Enemy::Runaway(Ogre::Real deltaT) {

		// Get positions.
		SpacePlayer* player = level->getPlayer();
        const Ogre::Vector3& pos = enemyNode->getPosition();
		const Ogre::Vector3& playerPos = player->getSceneNode()->_getDerivedPosition();

		// This allows the Enemy to Runaway from the player.
        Ogre::Vector3 toTarget = playerPos - pos;	
        toTarget.normalise();

		// Translate backwards.
		enemyNode->translate(30* deltaT * toTarget * gForwardZ, Ogre::Node::TS_LOCAL);

	}

	/**
	 * This function will kill this enemy unit.
	 */
	void Enemy::Die() {

		mJetstreamChannel = MusicManager::getInstance()->stopJetstreamSound(mJetstreamChannel);

		std::vector<Explosions*> allExp;
		int totalExp;
        // add a big explosion that obscures the entity's disappearance
		for(int i =0; i < 5;i++){
        Ogre::Vector3 center = getCentroid();
        Ogre::Vector3 explPos;
        Ogre::Real explSize;

        Ogre::Real r = 30.75f;
        Ogre::Real h = 30.25f;

        // A fireball near the center of the enemy.
        explPos.x = center.x + Ogre::Math::RangeRandom(-r, r);
        explPos.z = center.z + Ogre::Math::RangeRandom(-r, r);
        explPos.y = center.y + Ogre::Math::RangeRandom(-0.15f * h, 0.35f * h);
        explSize = Ogre::Math::RangeRandom(4, 6);
		Explosions* e1 = new Explosions(level, 50.0f, explPos);
		allExp.push_back(e1);
		level->addObject(e1);

        // A fireball at a random offset above the center of the enemy.
        explPos.x = center.x + Ogre::Math::RangeRandom(-r, r);
        explPos.z = center.z + Ogre::Math::RangeRandom(-r, r);
        explPos.y = center.y + Ogre::Math::RangeRandom(0.35f * h, h);
        explSize = Ogre::Math::RangeRandom(4, 6);
        Explosions* e2 = new Explosions(level, 50.0f, explPos);
		allExp.push_back(e2);
		level->addObject(e2);

        // A fireball at a random offset below center of the enemy.
        explPos.x = center.x + Ogre::Math::RangeRandom(-r, r);
        explPos.z = center.z + Ogre::Math::RangeRandom(-r, r);
        explPos.y = center.y + Ogre::Math::RangeRandom(-0.15f * h, -0.5f * h);
        explSize = Ogre::Math::RangeRandom(4, 6);
        Explosions* e3 = new Explosions(level, 50.0f, explPos);
		allExp.push_back(e3);
		level->addObject(e3);
		totalExp = allExp.size();

		if(i == 4){

			for(int i =0; i < totalExp; i++){
				delete allExp[i];
			}
		}

		}
		
	}
	

// =====================================================================================
//
// #AI Logic
//
// =====================================================================================

	bool Enemy::isPlayerAlive() {
		SpacePlayer* player = level->getPlayer();
		return player->isAlive();
	}

	bool Enemy::canSee(const Ogre::SceneNode* target) const {

        const Ogre::Vector3& pos = enemyNode->getPosition();
        const Ogre::Vector3& targetPos = target->_getDerivedPosition();

        Ogre::Vector2 P(pos.x, pos.z);
        Ogre::Vector2 T(targetPos.x, targetPos.z);
        Ogre::Vector2 toTargetV2 = T - P;

		Ogre::Vector3 toTarget = targetPos - pos;

        Ogre::Real dist = toTarget.length();


        if (dist > mVisionRange){
			return false;
		}else{
			//Work on proper calculation to get the players position
			//ogreHead->setPosition(targetPos); //AC
		}

        Ogre::Vector3 forward = enemyNode->getOrientation() * gForward;
        Ogre::Vector2 heading(forward.x, forward.z);

        return GetAbsYaw(heading, toTargetV2) < 0.5f * mFOV;
    }

	bool Enemy::canSeePlayer() const {

		if(!level->getPlayer()->isAlive())
			return false;

		if(canSee(level->getPlayer()->getHeadNode()))
			std::cout << "I can see you..." << std::endl;

		return canSee(level->getPlayer()->getHeadNode());
	}

	bool Enemy::canHear(const Ogre::SceneNode* target) const
    {
        const Ogre::Vector3& pos = enemyNode->getPosition();
        const Ogre::Vector3& targetPos = target->_getDerivedPosition();

        return pos.squaredDistance(targetPos) < mHearingRange * mHearingRange;
    }

    bool Enemy::canHearPlayer() const
    {
		if(!level->getPlayer()->isAlive())
			return false;

		return canHear(level->getPlayer()->getHeadNode());
    }

	bool Enemy::canFire() const
	{
		const Ogre::Vector3& pos = enemyNode->getPosition();
		const Ogre::Vector3& targetPos = level->getPlayer()->getSceneNode()->_getDerivedPosition();

        Ogre::Vector2 P(pos.x, pos.z);
        Ogre::Vector2 T(targetPos.x, targetPos.z);
        Ogre::Vector2 toTarget = T - P;
        Ogre::Real dist = toTarget.length();

        if (dist <= mFireRange){
			enemyNode->lookAt(targetPos, Ogre::Node::TS_PARENT, Ogre::Vector3::UNIT_Z);
            return true;
		}
		else
			return false;
	}

	bool Enemy::canAssist() const
	{
		std::vector<Enemy*> friends;
		friends = level->getEnemies();

		for(int i=0;i<friends.size();i++){
			
			if(friends[i] != this){

				const Ogre::Vector3& pos = enemyNode->getPosition();
				const Ogre::Vector3& targetPos = friends[i]->getEnemyPosition();

				/*Ogre::Vector2 P(pos.x, pos.z);
				Ogre::Vector2 T(targetPos.x, targetPos.z);
				Ogre::Vector2 toTarget = T - P;*/
				Ogre::Vector3 toTarget = targetPos - pos;
				Ogre::Real dist = toTarget.length();

				if(dist < mAggroRange && friends[i]->AI_CHASE)
					return true;
			}
		}

		return false;
	}

}
