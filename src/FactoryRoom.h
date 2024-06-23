#ifndef capstone_FACTORYROOM_H_
#define capstone_FACTORYROOM_H_

#include "Room.h"
#include "RoomFlare.h"

namespace capstone {

	// Forward declarations.
	class SpacePlayer;
	class Part;

	class FactoryRoom : public Room {

		// Class variables.
		char*					partsFile;
		char*					blueprintsFile;
		Ogre::Camera*			mCamera;
        Ogre::Light*			mLight;
		Ogre::SceneManager*		sceneMgr;
		SpacePlayer*			mPlayer;

		// Collections.
		std::vector<Part*>		parts;
		std::vector<RoomFlare*>	flareList;

	public:

		// Constructor and destructor.
		FactoryRoom();
		~FactoryRoom();

		// Update.
		void update(Ogre::Real deltaT);

		// Getters.
		OIS::KeyListener*		getKeyListener();
        OIS::MouseListener*		getMouseListener();
		Ogre::Camera*			getCamera()				{ return mCamera; }
		Ogre::SceneManager*		getSceneManager()		{ return sceneMgr; }
		SpacePlayer*			getPlayer()				{ return mPlayer; }
		std::vector<Part*>		getParts()				{ return parts; }

		// Called when the player creates a part from a blueprint.
		void buildPart(Part* part);
		void placeParts(Ogre::SceneManager* sceneMgr);

	private:

		void init();
		void createPlayer(Ogre::SceneManager* sceneMgr);
		void createTerrain(Ogre::SceneManager* sceneMgr);
		void createFlares();

		// Helper methods for buildPart()
		void buildMeleePart(Part* part);
		void buildProjectilePart(Part* part);
		void buildPassivePart(Part* part);
		void deleteBlueprint();
	};
}

#endif