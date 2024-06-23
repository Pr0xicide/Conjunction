#ifndef capstone_MELEESKILL_H_
#define capstone_MELEESKILL_H_

#include "Skill.h"

namespace capstone{

	// Foward declarations.
	class SpaceLevel;
	class SpacePlayer;

	/**
	 * Child class for all melee skills that can used by the player.
	 */
	class MeleeSkill : public Skill {

		// Instance variables.
		SpaceLevel*				mLevel;
		SpacePlayer*			mPlayer;
		Ogre::String			mSkillName;
		Ogre::Real				mCoolDownTime;

	public:

		// Constructors and destructor.
		MeleeSkill();
		MeleeSkill(const Ogre::String skillName);
		MeleeSkill(SpaceLevel* level, SpacePlayer* player,
					const Ogre::String skillName, const Ogre::Real coolDownTime);
		~MeleeSkill();

		// Getters.
		Ogre::String getName()				{ return mSkillName; }

		// Interactions.
		void executeLeftSkill();
		void executeRightSkill();
		void executeHeadSkill();
		void executeRightShoulderSkill();
		void executeLeftShoulderSkill();

	};

}

#endif