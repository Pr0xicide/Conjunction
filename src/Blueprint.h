#ifndef capstone_BLUEPRINT_H_
#define capstone_BLUEPRINT_H_

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
	class Blueprint : public Part {
		
		// Class object pointers.
		SpaceLevel* level;
		Room* room;
		SpacePlayer* player;

		bool playerClose;
		bool isEquipped;

		int mPartType;					// For now, possible values are: 1, 2, 3.
		int metal;
		int darkMatter;
		int carbon;
		
		Ogre::String mMeshName;
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
		Blueprint();																	// Default constructor in case no part is put into a realloc state.
																				
		Blueprint(Ogre::String name, Ogre::String meshName, Ogre::Entity* entity, int partType,
			Ogre::SceneNode* node, Ogre::Real statIncrease,
			Ogre::String skillName, 
			int metal, int darkMatter, int carbon);										// Constructor for a blueprint of a part.

		Blueprint(Ogre::String name, Ogre::String meshName, Ogre::Entity* entity, int partType,
			Ogre::SceneNode* node, Ogre::Real statIncrease, Ogre::String skillName,
			int metal, int darkMatter, int carbon,
			Ogre::Real size, Ogre::String material,
			Ogre::Real speed, Ogre::Real acceleration,
			Ogre::ColourValue colourValue, Ogre::Real lifeSpan);						// Constructor for parts that shoot projectiles.

		// Copyable.
        /*Blueprint(const Blueprint& p);
		Blueprint(const Blueprint& p, SpaceLevel* l, SpacePlayer* sp);
        Blueprint& operator= (const Blueprint& p);*/
		~Blueprint();

		// Getters.
		bool				getPlayerClose()	{ return playerClose; }
		bool				getIsEquipped()		{ return isEquipped; }
		int					getMetal()			{ return metal; }
		int					getDarkMatter()		{ return darkMatter; }
		int					getCarbon()			{ return carbon; }
		int					getPartType()		{ return mPartType; }
		double				getStatIncrease()	{ return mStatIncrease; }
		Ogre::Entity*		getEntity()			{ return mEntity; }
		Ogre::Real			getRadius()			{ return mRadius; }
		Ogre::String		getName()			{ return mName; }
		Ogre::SceneNode*	getSceneNode()		{ return mSceneNode; }
		Skill*				getSkill()			{ return mSkill; }
		unsigned			getSkillType()		{ return mSkillType; }
		Ogre::String		getMeshName()		{ return mMeshName; }
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
		void setToExist();						// Called when a part gets created from a blueprint.
	};

}

#endif