#ifndef capstone_COMPLETEPART_H_
#define capstone_COMPLETEPART_H_

#include "Part.h"

namespace capstone{

	// Forward declarations.
	class SpaceLevel;
	class Room;
	class SpacePlayer;
	class Skill;

	/**
	 * This class is the blueprint for the parts that the robot will make and equip.
	 */
	class CompletePart : public Part {
		
		// Class object pointers.
		SpaceLevel* level;
		Room* room;
		SpacePlayer* player;

		bool playerClose;
		bool isEquipped;

		int mPartType;					// For now, possible values are: 1, 2, 3.
		
		Ogre::Entity* mEntity;			// If this part is a blueprint, mEntity is a blueprint.
		Ogre::SceneNode* mSceneNode;
		Ogre::String mName;
		Skill*  mSkill;
		Ogre::String mSkillName;
		Ogre::Real mStatIncrease;		// mPartType determines which stat increases.
		Ogre::Real mRadius;

		// Attributes for projectiles to use.
		Ogre::Vector3 mPosition;
		Ogre::Quaternion& mOrientation;
		Ogre::Real mSize;
		Ogre::String mMaterial;
		Ogre::Real mSpeed;
		Ogre::Real mAcceleration;
		Ogre::ColourValue mColour;
		Ogre::Real mLifeSpan;
		unsigned mSkillType;
		
	public:

		// Constructors and destructor.
		CompletePart();																							// Default constructor in case no part is put into a realloc state.
		CompletePart(Ogre::String name, Ogre::Entity* entity, int partType,					
			Ogre::SceneNode* node, Ogre::Real statIncrease, Ogre::String skillName);							// Constructor for a previously created part.
		

		CompletePart(Ogre::String name, Ogre::Entity* entity, int partType,
			Ogre::SceneNode* node, Ogre::Real statIncrease, Ogre::String skillName,
			Ogre::Real size, Ogre::String material,
			Ogre::Real speed, Ogre::Real acceleration,
			Ogre::ColourValue colourValue, Ogre::Real lifeSpan);												// Constructor for parts that shoot projectiles.

		// Copyable.
        CompletePart(const CompletePart& p);
		CompletePart(const CompletePart& p, SpaceLevel* l, SpacePlayer* sp);
        CompletePart& operator= (const CompletePart& p);
		~CompletePart();

		// Getters.
		bool				getPlayerClose()	{ return playerClose; }
		bool				getIsEquipped()		{ return isEquipped; }
		int					getMetal()			{ return 0; }
		int					getDarkMatter()		{ return 0; }
		int					getCarbon()			{ return 0; }
		int					getPartType()		{ return mPartType; }
		double				getStatIncrease()	{ return mStatIncrease; }
		Ogre::Entity*		getEntity()			{ return mEntity; }
		Ogre::Real			getRadius()			{ return mRadius; }
		Ogre::String		getName()			{ return mName; }
		Ogre::SceneNode*	getSceneNode()		{ return mSceneNode; }
		Skill*				getSkill()			{ return mSkill; }
		unsigned			getSkillType()		{ return mSkillType; }
		Ogre::String		getMeshName()		{ return ""; }
		Ogre::Real			getSize()			{ return mSize; }
		Ogre::String		getMaterial()		{ return mMaterial; }
		Ogre::Real			getSpeed()			{ return mSpeed; }
		Ogre::Real			getAccel()			{ return mAcceleration; }
		float				getRed()			{ return mColour.r; }
		float				getGreen()			{ return mColour.g; }
		float				getBlue()			{ return mColour.b; }
		Ogre::Real			getLifeSpan()		{ return mLifeSpan; }

		// Setters
		void setPlayerClose(bool close)			{ playerClose = close; }
		void setIsEquipped(bool equipped)		{ isEquipped = equipped; }
	};

}

#endif