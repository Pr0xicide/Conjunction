#ifndef capstone_RESOURCE_H_
#define capstone_RESOURCE_H_

namespace capstone {

	// Forward declaration.
	class SpaceLevel;

	/**
	 * Class definition the resources that the enemies will drop.
	 */
	class Resource {

	protected:

		// Class variables.
		bool					isDisposable;
		int						resourceID;
		int						amount;
		SpaceLevel*				level;

		Ogre::BillboardSet*		mBillboardSet;
		Ogre::Entity*			mEntity;
		Ogre::Real				speed;
		Ogre::SceneNode*		mSceneNode;
		Ogre::RibbonTrail*		mTrail;
		Ogre::Vector3			position;
		Ogre::Vector3			velocity;

		virtual void setup();

	public:

		// Constructor & destructor.
		Resource(SpaceLevel* level, Ogre::Vector3 pos);
		virtual ~Resource();

		// Update/Render.
		virtual void update(Ogre::Real deltaT);

		// Getters.
		bool isResourceDisposable()			{ return isDisposable; }
		int getAmount()						{ return amount; }
		int getResourceId()					{ return resourceID; }
		Ogre::Vector3 getPosition()			{ return mSceneNode->getPosition(); }

		// Setters.
		void setDisposable()				{ isDisposable = true; }

	};

}

#endif
