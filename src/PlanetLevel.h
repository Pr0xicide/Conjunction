#ifndef capstone_PLANETLEVEL_H_
#define capstone_PLANETLEVEL_H_

#include "Level.h"
#include "Buildings.h"
#include "PlanetPlayer.h"
#include "PlanetProp.h"

namespace capstone {

	// Forward declarations.
	class PlanetPlayer;

	class PlanetLevel : public Level {

		// Class variables.
		static const char*					planetFilename;
		unsigned							oIndex;

		std::vector<InteractiveBuilding*>	buildingList;
		std::vector<PlanetProp*>			propList;

		PlanetPlayer*						mPlayer;

		Ogre::Camera*						mCamera;

        Ogre::Light*						mLight;

		Ogre::SceneManager*					sceneMgr;

		Ogre::SceneNode*					roomNode;
		Ogre::SceneNode*					sphereNode;

		Ogre::Real							planetRadius;
		
	public:

		// Constructor and destructor.
		PlanetLevel(Ogre::Real radius);
		~PlanetLevel();

		// Update.
		void update(Ogre::Real deltaT);
		
		// Getters.
		InteractiveBuilding*	getClosestBuilding()	{ return buildingList[oIndex]; }
		PlanetPlayer*			getPlayer()				{ return mPlayer; }

		OIS::KeyListener*		getKeyListener()		{ return mPlayer; }
		OIS::MouseListener*		getMouseListener()		{ return mPlayer; }
		
		Ogre::Camera*			getCamera()				{ return mCamera; }
		Ogre::Real				getRadius()				{ return planetRadius; }
		Ogre::SceneManager*		getSceneManager()		{ return sceneMgr; }
		Ogre::Vector3			getPlayerPosition()		{ return mPlayer->getSceneNode()->getPosition(); }		

	private:

		// Initialization helper methods.
		void init();
		void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
		void createWorld();
		void populateWorld();
		void createPlayer(Ogre::SceneManager* sceneMgr);
		void createTerrain(Ogre::SceneManager* sceneMgr);

		// Update helper methods.
		void closestInteractiveObject();

	};

}

#endif