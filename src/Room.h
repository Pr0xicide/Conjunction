#ifndef capstone_ROOM_H_
#define capstone_ROOM_H_

#include "Level.h"

namespace capstone {

	class SpacePlayer;
	class Part;

	class Room : public Level {

	protected:

		Part*							closestPart;

	public:

		// Constructor and destructor.
		Room();
		~Room();

		// Update.
		virtual void update(Ogre::Real deltaT)						= 0;

		// Getters.
		virtual Ogre::Camera*			getCamera()					= 0;
		virtual SpacePlayer*			getPlayer()					= 0;
		virtual std::vector<Part*>		getParts()					= 0;
		virtual Ogre::SceneManager*		getSceneManager()			= 0;
		virtual OIS::KeyListener*		getKeyListener()			= 0;
        virtual OIS::MouseListener*		getMouseListener()			= 0;
		Part*							getClosestPart()			{ return closestPart; }

		// Setters.
		virtual void					buildPart(Part* part)		= 0;

	protected:

		// Pure virtual methods.
		virtual void init()											= 0;
		virtual void createPlayer(Ogre::SceneManager* sceneMgr)		= 0;
		virtual void createTerrain(Ogre::SceneManager* sceneMgr)	= 0;
		virtual void placeParts(Ogre::SceneManager* sceneMgr)		= 0;

		
	};
}

#endif