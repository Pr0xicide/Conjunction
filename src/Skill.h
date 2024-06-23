#ifndef capstone_SKILL_H_
#define capstone_SKILL_H_

namespace capstone{

	// Foward declarations.
	class SpaceLevel;
	class Part;

	/**
	 * Parent class for all skills that can used by the player.
	 */
	class Skill {

		// Instance variables.
		Ogre::String		skillName;
		Ogre::Real			coolDownTime;

	public:

		// Constructors and destructor.
		Skill();
		Skill(Ogre::String name);
		virtual ~Skill();

		// Getters.
		virtual Ogre::String getName() = 0;

		// Pure abstract methods that all child classes need.
		virtual void executeLeftSkill() = 0;
		virtual void executeRightSkill() = 0;
		virtual void executeHeadSkill() = 0;
		virtual void executeRightShoulderSkill() = 0;
		virtual void executeLeftShoulderSkill() = 0;

	};
}
#endif