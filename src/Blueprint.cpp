#include "stdafx.h"

#include "Blueprint.h"
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
	Blueprint::Blueprint()
		: mName("")
		, mMeshName("")
		, mEntity(NULL)
		, mPartType(0)
		, mSceneNode(NULL)
		, mStatIncrease(0)
		, mSkill(NULL)
		, mRadius(0)
		, playerClose(false)
		, isEquipped(false)
		, metal(0)
		, darkMatter(0)
		, carbon(0)
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
	 * Constructor for a blueprint of a part.
	 */
	Blueprint::Blueprint(Ogre::String name, Ogre::String meshName, Ogre::Entity* entity, int partType,
		Ogre::SceneNode* node, Ogre::Real statIncrease, Ogre::String skillName,
		int metal, int darkMatter, int carbon)
		: mName(name)
		, mMeshName(meshName)
		, mEntity(entity)
		, mPartType(partType)
		, mSceneNode(node)
		, mStatIncrease(statIncrease)
		, mSkill(NULL)
		, mRadius(20)
		, playerClose(false)
		, isEquipped(false)
		, metal(metal)
		, darkMatter(darkMatter)
		, carbon(carbon)
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
		, mSkillName(skillName)
	{
		mSkill = new MeleeSkill(mSkillName);
	}

	/**
	 * Constructor for a part with a projectile skill.
	 */
	Blueprint::Blueprint(Ogre::String name, Ogre::String meshName, Ogre::Entity* entity, int partType,
		Ogre::SceneNode* node, Ogre::Real statIncrease, Ogre::String skillName,
		int metal, int darkMatter, int carbon,
		Ogre::Real size, Ogre::String material, Ogre::Real speed, 
		Ogre::Real acceleration, Ogre::ColourValue colourValue,
		Ogre::Real lifeSpan)
		: mName(name)
		, mMeshName(meshName)
		, mEntity(entity)
		, mPartType(partType)
		, mSceneNode(node)
		, mStatIncrease(statIncrease)
		, mSkill(NULL)
		, mRadius(20)
		, playerClose(false)
		, isEquipped(false)
		, metal(metal)
		, darkMatter(darkMatter)
		, carbon(carbon)
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
	 * Class destructor.
	 */
	Blueprint::~Blueprint() {
	}

}