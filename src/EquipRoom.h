#ifndef capstone_EQUIPROOM_H_
#define capstone_EQUIPROOM_H_

#include "Room.h"
#include "RoomFlare.h"

namespace capstone {

	// Forward declarations.
	class SpacePlayer;
	class Part;
	class CompletePart;

	class EquipRoom : public Room {

		// Class variables.
		char*					fileName;
		Ogre::Camera*			mCamera;
        Ogre::Light*			mLight;
		Ogre::SceneManager*		sceneMgr;
		SpacePlayer*			mPlayer;

		// Collection list.
		std::vector<Part*>		parts;
		std::vector<RoomFlare*>	flareList;

	public:

		// Constructor and destructor.
		EquipRoom();
		~EquipRoom();

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
		void deleteAllParts();

	private:

		void init();
		void createPlayer(Ogre::SceneManager* sceneMgr);
		void createTerrain(Ogre::SceneManager* sceneMgr);
		void createFlares();

	};
}

#endif