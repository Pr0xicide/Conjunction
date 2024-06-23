#ifndef capstone_GAME_H_
#define capstone_GAME_H_

namespace capstone {

	// Forward declarations.
	class Level;

	/**
	 * This class creates the game that will be used to during run time.
	 */
    class Game
        : public Ogre::FrameListener         // handle rendering loop events
        , public Ogre::WindowEventListener   // handle window events
        , public OIS::KeyListener            // handle keyboard events
        , public OIS::MouseListener          // handle mouse events
		, public OgreBites::SdkTrayListener  // gui stuff
    {

        // Configuration and log file names.
        Ogre::String					mOgreCfgName;       // renderer config
        Ogre::String					mPluginCfgName;     // plugin config
        Ogre::String					mResourceCfgName;   // resource locations
        Ogre::String					mLogName;           // log file

        // Core Ogre3D objects.
        Ogre::Root*						mRoot;
        Ogre::RenderWindow*				mWindow;
        Ogre::Viewport*					mViewport;

        // Input handling stuff.
        OIS::InputManager*				mInputMgr;
        OIS::Keyboard*					mKeyboard;
        OIS::Mouse*						mMouse;

        // GUI variables.
        OgreBites::SdkTrayManager*		mTrayMgr;
        OgreBites::Label*				mHealthLabel;

        // Flag to set if game should exit.
        bool							mShouldQuit;

        // Pointer to the game world, which contains all game objects
        Level*							mLevel;
		

    public:

        // Constructor and destructor.
        Game();
        ~Game();

        // The method that actually initializes everything, runs the game loop, then shuts everything down.
        void go();

        // Getters.
        OIS::Keyboard* getKeyboard()		{ return mKeyboard; }
        OIS::Mouse* getMouse()				{ return mMouse; }
        Ogre::RenderWindow* getWindow()		{ return mWindow; }
        Ogre::Viewport* getViewport()		{ return mViewport; }
        Ogre::Real getTime() const			{ return 0.001f * mRoot->getTimer()->getMilliseconds(); }

    private:

        // Noncopyable constructor and assignment operator.
        Game(const Game&);
        Game& operator= (const Game&);

        // Private initialization methods.
        bool initializeRenderer();
        void initializeInput();
        void initializeResources();
        void initializeGui();

        // Private cleanup methods.
        void shutdownRenderer();
        void shutdownInput();
		void shutdownGui();

        // Level handling methods.
        void createLevel();
        void destroyLevel(); 

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		void updateHud();
        void windowResized(Ogre::RenderWindow* window);
        void windowClosed(Ogre::RenderWindow* window);
        bool keyPressed(const OIS::KeyEvent& arg);
        bool keyReleased(const OIS::KeyEvent& arg);
        bool mouseMoved(const OIS::MouseEvent& arg);
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

    };
}

#endif
