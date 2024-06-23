#ifndef capstone_PROJECTILE_H_
#define capstone_PROJECTILE_H_

namespace capstone {

	// Class declarations.
	class Part;
	class Skill;
	class SpaceLevel;

	/**
	 * Class definition for all projectiles that will used during the space level.
	 */
	class Projectile {

		// Instance variables.
		SpaceLevel*				mLevel;

		void*					mShooter;

		bool					mDestroy;

		int						damageAmount;

		Ogre::BillboardSet*		mBillboardSet;  // Used for drawing the flare.

		Ogre::ColourValue		mColour;

		Ogre::Real				mSize;
		Ogre::Real				mSpeed;
		Ogre::Real				mAcceleration;
		Ogre::Real				mBirth;			// Time taken at projectile creation.
		Ogre::Real				mLifeSpan;		// Total time the projectile will stay in world.

		Ogre::RibbonTrail*		mTrail;         // Trail.

		Ogre::SceneNode*		mSceneNode;
		
		Ogre::String			mMaterial;
		
		Ogre::Vector3			mVelocity;

	public:

		// Constructors and destructor.
		Projectile();
		Projectile(SpaceLevel* level,
                   void* shooter,
                   const Ogre::Vector3& position,
                   const Ogre::Quaternion& orientation,
				   const Ogre::Real size,
				   const Ogre::String material,
				   const Ogre::Real speed,
				   const Ogre::Real acceleration,
				   const Ogre::ColourValue colour,
				   const Ogre::Real lifeSpan,
				   int dmgAmount);
		~Projectile();

		// Getters.
		void* getShooter() const						{ return mShooter; }
		bool getDestroy() const							{ return mDestroy; }
		int getDamageAmount() const						{ return damageAmount; }
		Ogre::SceneNode* getSceneNode() const			{ return mSceneNode; }
		Ogre::Vector3 getProjectilePosition() const		{ return mSceneNode->getPosition(); }

		// Setters.
		void destroy()									{ mDestroy = true; }	

		// Main update method/function.
		void update(Ogre::Real deltaT);

	};

}

#endif