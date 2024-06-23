#include "stdafx.h"

#include "MeleeSkill.h"
#include "SpaceLevel.h"
#include "SpacePlayer.h"
#include "Projectile.h"
#include "MusicManager.h"

namespace capstone{

	/**
	 * Default MeleeSkill constructor.
	 */
	MeleeSkill::MeleeSkill()
		: mLevel(NULL)
		, mPlayer(NULL)
		, mSkillName("")
		, mCoolDownTime(0)
	{

	}

	/**
	 * Overloaded MeleeSkill constructor.
	 */
	MeleeSkill::MeleeSkill(const Ogre::String skillName)
		: mLevel(NULL)
		, mPlayer(NULL)
		, mSkillName(skillName)
		, mCoolDownTime(0)
	{

	}

	/**
	 * Overloaded MeleeSkill constructor.
	 */
	MeleeSkill::MeleeSkill(SpaceLevel* level, SpacePlayer* player,
					const Ogre::String skillName, const Ogre::Real coolDownTime)
		: mLevel(level)
		, mPlayer(player)
		, mSkillName(skillName)
		, mCoolDownTime(coolDownTime)
	{

	}

	/**
	 * MeleeSkill destructor.
	 */
	MeleeSkill::~MeleeSkill(){

	}


//=====================================================================================
//
// #Interaction
//
//=====================================================================================

	/**
	 * REMOVE THIS LATER.
	 */
	void MeleeSkill::executeHeadSkill() {
		std::cout << "The player used: " << mSkillName << "!" << std::endl;
	}

	/**
	 * This function will execute the skill thats on the left arm part.
	 */
	void MeleeSkill::executeLeftSkill(){
		std::cout << "The player used: " << mSkillName << "!" << std::endl;
	}

	/**
	 * This function will execute the skill thats on the right arm part.
	 */
	void MeleeSkill::executeRightSkill(){
		std::cout << "The player used: " << mSkillName << "!" << std::endl;
	}

	/**
	 * This function will execute the skill thats on the right shoulder part.
	 */
	void MeleeSkill::executeRightShoulderSkill() {
	
	}

	/**
	 * This function will execute the skill thats on the left shoulder part.
	 */
	void MeleeSkill::executeLeftShoulderSkill() {

	}

}