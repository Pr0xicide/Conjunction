#ifndef capstone_PASSIVESKILL_H_
#define capstone_PASSIVESKILL_H_

#include "Skill.h"

namespace capstone {

	// Foward declarations.
	class SpaceLevel;
	class SpacePlayer;

	/**
	 * Child class for all passive bonuses that can used by the player.
	 */
	class PassiveSkill : public Skill {

		// Instance variables.
		SpaceLevel*				mLevel;
		SpacePlayer*			mPlayer;

		Ogre::Real				mCoolDownTime;

		Ogre::String			mSkillName;
		

	public:

		// Constructors and destructor.
		PassiveSkill();
		PassiveSkill(SpaceLevel* level, SpacePlayer* player,
					const Ogre::String skillName, const Ogre::Real coolDownTime);
		~PassiveSkill();

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