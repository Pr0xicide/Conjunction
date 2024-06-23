#include "stdafx.h"

#include "ProjectileSkill.h"
#include "SpaceLevel.h"
#include "SpacePlayer.h"
#include "Projectile.h"
#include "MusicManager.h"
#include "Attributes.h"

namespace capstone{

	/**
	 * Default ProjectileSkill constructor.
	 */
	ProjectileSkill::ProjectileSkill()
		: mLevel(NULL)
		, mPlayer(NULL)
		, mSkillName("")
		, mCoolDownTime(0)
		, mSize(0)
		, mMaterial("")
		, mSpeed(0)
		, mAcceleration(0)
		, mColour(0,0,0)
		, mLifeSpan(0)
		, dmgStat(10)
	{

	}

	/**
	 * Overloaded ProjectileSkill constructor.
	 */
	ProjectileSkill::ProjectileSkill(const Ogre::String skillName, Ogre::Real dmg)
		: mLevel(NULL)
		, mPlayer(NULL)
		, mSkillName(skillName)
		, mCoolDownTime(0)
		, mSize(0)
		, mMaterial("")
		, mSpeed(0)
		, mAcceleration(0)
		, mColour(0,0,0)
		, mLifeSpan(0)
		, dmgStat(dmg)
	{

	}

	/**
	 * Overloaded ProjectileSkill constructor.
	 */
	ProjectileSkill::ProjectileSkill(SpaceLevel* level, SpacePlayer* player,
						const Ogre::String skillName, const Ogre::Real coolDownTime,
						const Ogre::Real size, const Ogre::String material,
						const Ogre::Real speed, const Ogre::Real acceleration,
						const Ogre::ColourValue colour, const Ogre::Real lifeSpan, Ogre::Real dmg)
		: mLevel(level)
		, mPlayer(player)
		, mSkillName(skillName)
		, mCoolDownTime(coolDownTime)
		, mSize(size)
		, mMaterial(material)
		, mSpeed(speed)
		, mAcceleration(acceleration)
		, mColour(colour)
		, mLifeSpan(lifeSpan)
		, dmgStat(dmg)
	{
		
	}

	/**
	 * ProjectileSkill destructor.
	 */
	ProjectileSkill::~ProjectileSkill(){

	}


//=====================================================================================
//
// #Interaction
//
//=====================================================================================
	
	/**
	 * REMOVE THIS LATER.
	 */
	void ProjectileSkill::executeHeadSkill() {

		Ogre::Vector3 pos = mPlayer->getHeadSpawnNode()->_getDerivedPosition();
		Ogre::Quaternion ori = mPlayer->getHeadSpawnNode()->_getDerivedOrientation();

		Projectile* p = new Projectile(mLevel, mPlayer, pos, ori,
										mSize, mMaterial, mSpeed, mAcceleration,
										mColour, mLifeSpan, dmgStat);

		mLevel->addObject(p);
	}

	/**
	 * This function will execute the skill thats on the left arm part.
	 */
	void ProjectileSkill::executeLeftSkill() {

		Attributes::getInstance()->incrementShotCount();

		Ogre::Vector3 pos = mPlayer->getLeftSpawnNode()->_getDerivedPosition();
		Ogre::Quaternion ori = mPlayer->getLeftSpawnNode()->_getDerivedOrientation();

		Projectile* p = new Projectile(mLevel, mPlayer, pos, ori,
										mSize, mMaterial, mSpeed, mAcceleration,
										mColour, mLifeSpan, dmgStat);

		MusicManager::getInstance()->playProjectileSound();

		mLevel->addObject(p);
	}

	/**
	 * This function will execute the skill thats on the right arm part.
	 */
	void ProjectileSkill::executeRightSkill() {

		Attributes::getInstance()->incrementShotCount();

		Ogre::Vector3 pos = mPlayer->getRightSpawnNode()->_getDerivedPosition();
		Ogre::Quaternion ori = mPlayer->getRightSpawnNode()->_getDerivedOrientation();

		Projectile* p = new Projectile(mLevel, mPlayer, pos, ori,
										mSize, mMaterial, mSpeed, mAcceleration,
										mColour, mLifeSpan, dmgStat);

		MusicManager::getInstance()->playProjectileSound();

		mLevel->addObject(p);
	}

	/**
	 * This function will execute the skill thats on the right shoulder part.
	 */
	void ProjectileSkill::executeRightShoulderSkill() {

		/*Ogre::Vector3 playerNode = mPlayer->getRightShoulderSpawnNode()->getPosition();
		Ogre::Vector3 pos = mPlayer->getRightShoulderSpawnNode()->convertLocalToWorldPosition(playerNode);
		Ogre::Quaternion ori = mPlayer->getRightShoulderSpawnNode()->_getDerivedOrientation();*/

		Attributes::getInstance()->incrementShotCount();

		Ogre::Vector3 pos = mPlayer->getRightShoulderSpawnNode()->_getDerivedPosition();
		Ogre::Quaternion ori = mPlayer->getRightShoulderSpawnNode()->_getDerivedOrientation();

		pos.y += 7;
		pos.z -= 0.9f;

		Projectile* p = new Projectile(mLevel, mPlayer, pos, ori,
										mSize, mMaterial, mSpeed, mAcceleration,
										mColour, mLifeSpan, dmgStat);

		std::cout << "RightProjectile: " << pos << std::endl;

		MusicManager::getInstance()->playRocketSound();

		mLevel->addObject(p);

	}

	/**
	 * This function will execute the skill thats on the left shoulder part.
	 */
	void ProjectileSkill::executeLeftShoulderSkill() {

		/*Ogre::Vector3 playerNode = mPlayer->getLeftShoulderSpawnNode()->getPosition();
		Ogre::Vector3 pos = mPlayer->getLeftShoulderSpawnNode()->convertLocalToWorldPosition(playerNode);
		Ogre::Quaternion ori = mPlayer->getLeftShoulderSpawnNode()->_getDerivedOrientation();*/

		Attributes::getInstance()->incrementShotCount();

		Ogre::Vector3 pos = mPlayer->getLeftShoulderSpawnNode()->_getDerivedPosition();
		Ogre::Quaternion ori = mPlayer->getLeftShoulderSpawnNode()->_getDerivedOrientation();

		pos.y += 7;
		pos.z += 0.9f;

		Projectile* p = new Projectile(mLevel, mPlayer, pos, ori,
										mSize, mMaterial, mSpeed, mAcceleration,
										mColour, mLifeSpan, dmgStat);

		std::cout << "LeftProjectile: " << pos << std::endl;

		MusicManager::getInstance()->playRocketSound();

		mLevel->addObject(p);

	}

}