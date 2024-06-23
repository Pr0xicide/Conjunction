#ifndef capstone_PROJECTILESKILL_H_
#define capstone_PROJECTILESKILL_H_

#include "Skill.h"

namespace capstone{

	// Foward declarations.
	class SpaceLevel;
	class SpacePlayer;

	/**
	 * Child class for all projectile skills that can used by the player.
	 */
	class ProjectileSkill : public Skill {

		// Instance variables.
		SpaceLevel*						mLevel;
		SpacePlayer*					mPlayer;

		Ogre::ColourValue				mColour;

		Ogre::Real						dmgStat;
		Ogre::Real						mCoolDownTime;	
		Ogre::Real						mSize;
		Ogre::Real						mSpeed;
		Ogre::Real						mAcceleration;
		Ogre::Real						mLifeSpan;

		Ogre::String					mSkillName;
		Ogre::String					mMaterial;

	public:

		// Constructors and destructor.
		ProjectileSkill();
		ProjectileSkill(const Ogre::String skillName, Ogre::Real dmg);
		ProjectileSkill(SpaceLevel* level, SpacePlayer* player,
						const Ogre::String skillName, const Ogre::Real coolDownTime,
						const Ogre::Real size, const Ogre::String material,
						const Ogre::Real speed, const Ogre::Real acceleration,
						const Ogre::ColourValue colour, const Ogre::Real lifeSpan,
						Ogre::Real dmg);
		~ProjectileSkill();

		// Getters.
		Ogre::String getName()			{ return mSkillName; }

		// Interactions.
		void executeLeftSkill();
		void executeRightSkill();
		void executeHeadSkill();
		void executeRightShoulderSkill();
		void executeLeftShoulderSkill();

	};

}

#endif