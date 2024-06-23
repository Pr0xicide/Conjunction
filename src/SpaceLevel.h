#ifndef capstone_SPACELEVEL_H_
#define capstone_SPACELEVEL_H_

#include "Level.h"
#include "Resources.h"

namespace capstone {

	// Forward declarations.
	class SpacePlayer;
	class Enemy;
	class Projectile;
	class Explosions;

	class SpaceLevel : public Level {

		enum PowerupTypes
		{
			invalid = -1,		
			darkMatter = 1,			
			metalScrap = 2,		
			carbon = 3,		
		} PowerupTypes;

		// Class variables.
		static Ogre::String				skybox;
		char*							fileName;
		int								waveNum;
		Ogre::Camera*					mCamera;
        Ogre::Light*					mLight;
		Ogre::Light*					mTopLight;
		Ogre::SceneManager*				sceneMgr;
		SpacePlayer*					mPlayer;
		std::vector<Enemy*>				enemies;
		std::vector<Projectile*>		projectiles;
		std::vector<Explosions*>		explosion;
		std::vector<Resource*>			resources;
		Ogre::Real						mTime;				// keep track of time elapsed in the world

	public:

		// Constructor and destructor.
		SpaceLevel(int wave);
		~SpaceLevel();

		// return total elapsed time in seconds
        Ogre::Real getTime() const
        { return mTime; }

		// Update.
		void update(Ogre::Real deltaT);

		// Getters.
		OIS::KeyListener*		getKeyListener();
        OIS::MouseListener*		getMouseListener();
		Ogre::Camera*			getCamera()				{ return mCamera; }
		Ogre::SceneManager*		getSceneManager()		{ return sceneMgr; }
		SpacePlayer*			getPlayer()				{ return mPlayer; }
		std::vector<Enemy*>		getEnemies()			{ return enemies; }
		bool					isLevelComplete()		{ return enemies.size() == 0; }

		// Setters.
		void addObject(Projectile* projectile)			{ projectiles.push_back(projectile); }
		void addObject(Explosions* exp)					{ explosion.push_back(exp);	}

	private:

		// Initialization methods.
		void init();
		void createPlayer(Ogre::SceneManager* sceneMgr);
		void createTerrain(Ogre::SceneManager* sceneMgr);
		void createEnemies();

		// Update methods.
		void updateEnemies(Ogre::Real deltaT);
		void updateProjectiles(Ogre::Real deltaT);
		void updatePowerups(Ogre::Real deltaT);
		void checkCollisions();

	};

}
	
#endif