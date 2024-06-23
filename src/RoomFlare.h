#ifndef capstone_ROOMFLARE_H_
#define capstone_ROOMFLARE_H_

namespace capstone {
	
	// Forward delcaration.
	class Room;

	/**
	 * Class definition for the flares that will shine up any of the rooms.
	 */
	class RoomFlare {

		// Instance variables.
		float					sfX;
		float					sfY;
		float					sfZ;
		float					sfTimeScale;
		float					a;
		float					b;
		float					accumulator;

		int						animate;

		Room*			room;

		Ogre::BillboardSet*		mBillboardSet;  // Used for drawing the flare.

		Ogre::ColourValue		mColour;

		Ogre::Light*			mLight;

		Ogre::Real				mSize;
		Ogre::Real				mSpeed;

		Ogre::RibbonTrail*		mTrail;

		Ogre::SceneNode*		mSceneNode;

		Ogre::Vector3			position;

	public:

		// Constructor and destructor.
		RoomFlare(Room* roomLevel, Ogre::Vector3 position);
		virtual ~RoomFlare();

		// Update/Render.
		void update(Ogre::Real deltaT);

	private:

		void setupFlare();

	};

}

#endif