#ifndef capstone_PART_H_
#define capstone_PART_H_

namespace capstone{

	// Forward declarations.
	class SpaceLevel;
	class Room;
	class SpacePlayer;
	class Skill;

	/**
	 * This class is the blueprint for the parts that the robot will make and equip.
	 */
	class Part {
		
	public:

		// Constructors and destructor.
		Part();	
		~Part();

		// Getters.
		virtual bool				getPlayerClose()		= 0;
		virtual bool				getIsEquipped()			= 0;		
		virtual int					getMetal()				= 0;		
		virtual int					getDarkMatter()			= 0;
		virtual int					getCarbon()				= 0;		
		virtual int					getPartType()			= 0;	
		virtual double				getStatIncrease()		= 0;
		virtual Ogre::Entity*		getEntity()				= 0;
		virtual Ogre::Real			getRadius()				= 0;
		virtual Ogre::String		getName()				= 0;
		virtual Ogre::SceneNode*	getSceneNode()			= 0;
		virtual Skill*				getSkill()				= 0;
		virtual unsigned			getSkillType()			= 0;
		virtual Ogre::String		getMeshName()			= 0;
		virtual Ogre::Real			getSize()				= 0;
		virtual Ogre::String		getMaterial()			= 0;
		virtual Ogre::Real			getSpeed()				= 0;
		virtual Ogre::Real			getAccel()				= 0;
		virtual float				getRed()				= 0;
		virtual float				getGreen()				= 0;
		virtual float				getBlue()				= 0;
		virtual Ogre::Real			getLifeSpan()			= 0;

		// Setters
		virtual void setPlayerClose(bool close)				= 0;
		virtual void setIsEquipped(bool equipped)			= 0;						
	};

}

#endif