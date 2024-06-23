#ifndef capstone_LEVEL_H_
#define capstone_LEVEL_H_

namespace capstone {

	/**
	 * This class is the base class of all levels.
	 */
    class Level {

	protected:
		
		bool shouldFinish;

    public:
		Level();
		~Level();

		// Abstract methods that all child classes need.
		virtual void init()											= 0;
		
		virtual void update(Ogre::Real deltaT)						= 0;
		virtual OIS::KeyListener* getKeyListener()					= 0;
		virtual OIS::MouseListener* getMouseListener()				= 0;
		virtual void createPlayer(Ogre::SceneManager* sceneMgr)		= 0;
		virtual void createTerrain(Ogre::SceneManager* sceneMgr)	= 0;

    private:
        // Noncopyable.
        Level(const Level&);
        Level& operator= (const Level&);

    };
}

#endif
