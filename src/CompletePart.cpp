#include "stdafx.h"

#include "CompletePart.h"
#include "SpaceLevel.h"
#include "SpacePlayer.h"
#include "Room.h"
#include "GlobalVariables.h"
#include "Skill.h"
#include "MeleeSkill.h"
#include "ProjectileSkill.h"
#include "PassiveSkill.h"

namespace capstone {

	/**
	 * Default constructor in case no part is put into a realloc state.
	 */
	CompletePart::CompletePart()
		: mName("")
		, mEntity(NULL)
		, mPartType(0)
		, mSceneNode(NULL)
		, mStatIncrease(0)
		, mSkill(NULL)
		, mRadius(0)
		, playerClose(false)
		, isEquipped(false)
		, level(NULL)
		, player(NULL)
		, mPosition(Ogre::Vector3::ZERO)
		, mOrientation(Ogre::Quaternion(1,0,0,0))
		, mSize(0)
		, mMaterial("")
		, mSpeed(0)
		, mAcceleration(0)
		, mColour(0,0,0)
		, mLifeSpan(0)
		, mSkillType(MELEE_SKILL)
		, mSkillName("")
	{
		mSkill = new MeleeSkill("");
	}

	/**
	 * Constructor for a part with a melee skill.
	 */
	CompletePart::CompletePart(Ogre::String name, Ogre::Entity* entity, int partType,
		Ogre::SceneNode* node, Ogre::Real statIncrease, Ogre::String skillName)
		: mName(name)
		, mEntity(entity)
		, mPartType(partType)
		, mSceneNode(node)
		, mStatIncrease(statIncrease)
		, mSkill(NULL)
		, mRadius(20)
		, playerClose(false)
		, isEquipped(false)
		, level(NULL)
		, player(NULL)
		, mPosition(Ogre::Vector3::ZERO)
		, mOrientation(Ogre::Quaternion(1,0,0,0))
		, mSize(0)
		, mMaterial("")
		, mSpeed(0)
		, mAcceleration(0)
		, mColour(0,0,0)
		, mLifeSpan(0)
		, mSkillName(skillName)
		, mSkillType(MELEE_SKILL)
	{
		mSkill = new MeleeSkill(mSkillName);
	}

	/**
	 * Constructor for a part with a projectile skill.
	 */
	CompletePart::CompletePart(Ogre::String name, Ogre::Entity* entity, int partType,
		Ogre::SceneNode* node, Ogre::Real statIncrease, Ogre::String skillName,
		Ogre::Real size, Ogre::String material, Ogre::Real speed, 
		Ogre::Real acceleration, Ogre::ColourValue colourValue,
		Ogre::Real lifeSpan)
		: mName(name)
		, mEntity(entity)
		, mPartType(partType)
		, mSceneNode(node)
		, mStatIncrease(statIncrease)
		, mSkill(NULL)
		, mRadius(20)
		, playerClose(false)
		, isEquipped(false)
		, level(NULL)
		, player(NULL)
		, mPosition(Ogre::Vector3::ZERO)
		, mOrientation(Ogre::Quaternion(1,0,0,0))
		, mSize(size)
		, mMaterial(material)
		, mSpeed(speed)
		, mAcceleration(acceleration)
		, mColour(colourValue)
		, mLifeSpan(lifeSpan)
		, mSkillType(PROJECTILE_SKILL)
		, mSkillName(skillName)
	{
		mSkill = new ProjectileSkill(mSkillName, statIncrease);
	}

	/**
	 * Copy Constructor.
	 */
	CompletePart::CompletePart(const CompletePart& p)
		: mName(p.mName)
		, mEntity(p.mEntity)
		, mPartType(p.mPartType)
		, mSceneNode(p.mSceneNode)
		, mStatIncrease(p.mStatIncrease)
		, mSkill(p.mSkill)
		, mRadius(p.mRadius)
		, playerClose(p.playerClose)
		, isEquipped(p.isEquipped)
		, level(p.level)
		, player(p.player)
		, mPosition(p.mPosition)
		, mOrientation(p.mOrientation)
		, mSize(p.mSize)
		, mMaterial(p.mMaterial)
		, mSpeed(p.mSpeed)
		, mAcceleration(p.mAcceleration)
		, mColour(p.mColour)
		, mLifeSpan(p.mLifeSpan)
		, mSkillType(p.mSkillType)
		, mSkillName(p.mSkillName)
	{

	}

	/**
	 * Overloaded Copy Constructor.
	 */
	CompletePart::CompletePart(const CompletePart& p, SpaceLevel* l, SpacePlayer* sp)
		: mName(p.mName)
		, mEntity(p.mEntity)
		, mPartType(p.mPartType)
		, mSceneNode(p.mSceneNode)
		, mStatIncrease(p.mStatIncrease)
		, mSkill(p.mSkill)
		, mRadius(p.mRadius)
		, playerClose(p.playerClose)
		, isEquipped(p.isEquipped)
		, level(l)
		, player(sp)
		, mPosition(p.mPosition)
		, mOrientation(p.mOrientation)
		, mSize(p.mSize)
		, mMaterial(p.mMaterial)
		, mSpeed(p.mSpeed)
		, mAcceleration(p.mAcceleration)
		, mColour(p.mColour)
		, mLifeSpan(p.mLifeSpan)
		, mSkillType(p.mSkillType)
		, mSkillName(p.mSkillName)
	{
		switch (mSkillType) {

			case MELEE_SKILL:
				mSkill = new MeleeSkill(level, player, mSkillName, 0);
				break;
				
			case PROJECTILE_SKILL:
				mSkill = new ProjectileSkill(level, player, mSkillName, 0,
					mSize, mMaterial, mSpeed, mAcceleration, mColour, mLifeSpan, p.mStatIncrease);
				break;

			case PASSIVE_SKILL:
				mSkill = new PassiveSkill(level, player, mSkillName, 0);
				break;
		}
	}

	/**
	 * Overloaded Assignment Operator.
	 */
	CompletePart& CompletePart::operator= (const CompletePart& p)
	{
		mName = p.mName;
		mEntity = p.mEntity;
		mPartType = p.mPartType;
		mSceneNode = p.mSceneNode;
		mStatIncrease = p.mStatIncrease;
		mSkill = p.mSkill;
		mRadius = p.mRadius;
		playerClose = p.playerClose;
		isEquipped = p.isEquipped;
		level = p.level;
		player = p.player;
		mPosition = p.mPosition;
		mOrientation = p.mOrientation;
		mSize = p.mSize;
		mMaterial = p.mMaterial;
		mSpeed = p.mSpeed;
		mAcceleration = p.mAcceleration;
		mColour = p.mColour;
		mLifeSpan = p.mLifeSpan;
		mSkillType = p.mSkillType;
		mSkillName = p.mSkillName;

		return *this;
	}

	/**
	 * Class destructor.
	 */
	CompletePart::~CompletePart() {
	}

}