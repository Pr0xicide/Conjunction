#include "stdafx.h"

#include "Skill.h"
#include "Part.h"
#include "SpaceLevel.h"
#include "SpacePlayer.h"

namespace capstone{

	/**
	 *  Default Skill Constructor
	 */
	Skill::Skill()
		: skillName("")
		, coolDownTime(0)
	{

	}

	/**
	 *  Overloaded Skill Constructor
	 *
	 * @param name			name of the skills
	 *
	 */
	Skill::Skill(Ogre::String name)
		: skillName(name)
		, coolDownTime(0)
	{

	}

	/**
	 *  Skill Destructor
	 */
	Skill::~Skill() {

	}

}