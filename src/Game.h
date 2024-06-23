#ifndef capstone_GAME_H_
#define capstone_GAME_H_

namespace capstone {

	// Forward declarations.
	class GameState;

	/**
	 * This class creates the game that will be used to during run time.
	 */
    class Game
        : public Ogre::FrameListener         // handle rendering loop events
        , public Ogre::WindowEventListener   // handle window events
        , public OIS::KeyListener            // handle keyboard events
        , public OIS::MouseListener          // handle mouse events
    {

        // Configuration and log file names.
        Ogre::String			mOgreCfgName;       // renderer config
        Ogre::String			mPluginCfgName;     // plugin config
        Ogre::String			mResourceCfgName;   // resource locations
        Ogre::String			mLogName;           // log file

        // Core Ogre3D objects.
        Ogre::Root*				mRoot;
        Ogre::RenderWindow*		mWindow;
        Ogre::Viewport*			mViewport;

		// CEGUI.
		CEGUI::OgreRenderer*	mGuiRenderer;

        // Input handling stuff.
        OIS::InputManager*		mInputMgr;
        OIS::Keyboard*			mKeyboard;
        OIS::Mouse*				mMouse;

        // Flag to set if game should exit.
        bool mShouldQuit;

        // Pointer to current GameState.
        GameState* mCurrentState;
		GameState* mPreviousState;

    public:
        // Constructor and destructor.
        Game();
        ~Game();

        // The method that actually initializes everything, runs the game loop, then shuts everything down.
        void go();

		// Game states.
		void switchState(GameState* state);
		void quit()							{ mShouldQuit = true; }

        // Getters.
		static Game* getInstance();
        OIS::Keyboard* getKeyboard()		{ return mKeyboard; }
        OIS::Mouse* getMouse()				{ return mMouse; }
        Ogre::RenderWindow* getWindow()		{ return mWindow; }
        Ogre::Viewport* getViewport()		{ return mViewport; }
        Ogre::Real getTime() const			{ return 0.001f * mRoot->getTimer()->getMilliseconds(); } // return total elapsed time in seconds
		GameState* getPreviousState()		{ return mPreviousState; }
		GameState* getCurrentState()		{ return mCurrentState;	 }

		// Setters.
		void setPreviousState(GameState* prevState)		{ mPreviousState = prevState; }
		

    private:

        // Noncopyable constructor and assignment operator.
        Game(const Game&);
        Game& operator= (const Game&);

        // Private initialization methods.
        bool initializeRenderer();
        void initializeInput();
        void initializeResources();
		void initializeCEGUI();

        // Private cleanup methods.
        void shutdownRenderer();
        void shutdownInput();

        /**
         * Listener interfaces - implementations of base class virtual methods
         */

        // Ogre::FrameListener callbacks.
        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

        // Ogre::WindowEventListener callbacks.
        void windowResized(Ogre::RenderWindow* window);
        void windowClosed(Ogre::RenderWindow* window);

        // OIS::KeyListener callbacks.
        bool keyPressed(const OIS::KeyEvent& arg);
        bool keyReleased(const OIS::KeyEvent& arg);

        // OIS::MouseListener callbacks.
        bool mouseMoved(const OIS::MouseEvent& arg);
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

    };
}

#endif
