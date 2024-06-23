#ifndef capstone_PLANETLEVEL_H_
#define capstone_PLANETLEVEL_H_

#include "Level.h"
#include "Controllers.h"
#include "Buildings.h"

namespace capstone {

	/**
	 * This class provides the functionally for the Planet levels and extends the level class.
	 */
	class PlanetLevel : public Level {

		// Class variables.
		BaseController*						mController;

		Ogre::SceneManager*					sceneMgr;
		Ogre::SceneNode*					sphereNode;
		
		std::vector<InteractiveBuilding*>	buildingList;

	public:

		// Constructor and destructor.
		PlanetLevel(Game* game);
		~PlanetLevel();

		// Update.
		void update(Ogre::Real deltaT);

		// Getters.
		Ogre::SceneManager*		getSceneMgr()		{ return sceneMgr; }
		OIS::KeyListener*		getKeyListener();
		OIS::MouseListener*		getMouseListener();

	private:

		void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
		void createWorld(Ogre::SceneManager* sceneMgr);
		void createTerrain(Ogre::SceneManager* sceneMgr);
		void setupController();

	};

}

#endif