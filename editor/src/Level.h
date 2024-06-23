#ifndef capstone_LEVEL_H_
#define capstone_LEVEL_H_

namespace capstone {

    // Forward declarations.
    class Game;

	/**
	 * This class is the base class of all levels.
	 */
    class Level {

	protected:

        // Pointers for the forward declared classses.
        Game* mGame;

        // The level is responsible for managing the Ogre scene using a scene manager.
		float					mRadius;
        Ogre::SceneManager*		mSceneMgr;
        Ogre::Camera*			mCamera;
        Ogre::Light*			mLight;

    public:

        // Constructor and destructor.
        Level(Game* game);
        ~Level();

        // Getters.
		float getPlanetRadius()						{ return mRadius; }
        Game* getGame()								{ return mGame; }
        Ogre::SceneManager* getSceneManager()		{ return mSceneMgr; }
        Ogre::Camera* getActiveCamera()				{ return mCamera; }

		// Public abstract methods/functions.
        virtual OIS::KeyListener* getKeyListener() = 0;
        virtual OIS::MouseListener* getMouseListener() = 0;
		virtual void update(Ogre::Real deltaT) = 0;
		virtual void setupController() = 0;

		// Initialization methods.
		void initLevel();

    private:

        // Noncopyable constructor and assignment operator.
        Level(const Level&);
        Level& operator= (const Level&);

		// Private abstract methods/functions.
		virtual void createTerrain(Ogre::SceneManager* sceneMgr) = 0;
    };
}

#endif
