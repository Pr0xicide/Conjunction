#include "stdafx.h"

#include "PassiveSkill.h"
#include "SpaceLevel.h"
#include "SpacePlayer.h"
#include "Projectile.h"
#include "MusicManager.h"

namespace capstone {

	/**
	 * Default PassiveSkill constructor.
	 */
	PassiveSkill::PassiveSkill()
		: mLevel(NULL)
		, mPlayer(NULL)
		, mSkillName("")
		, mCoolDownTime(0)
	{

	}

	/**
	 * Overloaded PassiveSkill constructor.
	 */
	PassiveSkill::PassiveSkill(SpaceLevel* level, SpacePlayer* player,
					const Ogre::String skillName, const Ogre::Real coolDownTime)
		: mLevel(level)
		, mPlayer(player)
		, mSkillName(skillName)
		, mCoolDownTime(coolDownTime)
	{

	}

	/**
	 * PassiveSkill destructor.
	 */
	PassiveSkill::~PassiveSkill(){

	}


//=====================================================================================
//
// #Interaction
//
//=====================================================================================
	
	/**
	 * REMOVE THIS LATER.
	 */
	void PassiveSkill::executeHeadSkill() {
		std::cout << "The player used: " << mSkillName << "!" << std::endl;
	}

	/**
	 * This function will execute the skill thats on the left arm part.
	 */
	void PassiveSkill::executeLeftSkill() {
		std::cout << "The player used: " << mSkillName << "!" << std::endl;
	}

	/**
	 * This function will execute the skill thats on the right arm part.
	 */
	void PassiveSkill::executeRightSkill() {
		std::cout << "The player used: " << mSkillName << "!" << std::endl;
	}

	/**
	 * This function will execute the skill thats on the right shoulder part.
	 */
	void PassiveSkill::executeRightShoulderSkill() {
	
	}

	/**
	 * This function will execute the skill thats on the left shoulder part.
	 */
	void PassiveSkill::executeLeftShoulderSkill() {

	}

}