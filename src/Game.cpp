#include "stdafx.h"

#include "Game.h"
#include "Levels.h"
#include "gamestates.h"

#if _WIN32
#include "WindowsConsole.h"
#endif

namespace capstone {

	/**
	 * Game constructor.
	 */
    Game::Game()
        : mOgreCfgName("ogre.cfg")            // render system config
#if _DEBUG
        , mPluginCfgName("plugins_d.cfg")     // plugins to use in Debug configuration
#else
        , mPluginCfgName("plugins.cfg")       // plugin to use in Release configuration
#endif
        , mResourceCfgName("resources.cfg")   // resource locations
        , mLogName("ogre.log")                // log file name
        , mRoot(NULL)
        , mWindow(NULL)
        , mViewport(NULL)
        , mInputMgr(NULL)
        , mKeyboard(NULL)
        , mMouse(NULL)
        , mShouldQuit(false)
		, mCurrentState(NULL)
    {

    }

	/**
	 * Game destructor.
	 */
    Game::~Game() {
        shutdownInput();
        shutdownRenderer();
		if (mGuiRenderer)
			CEGUI::OgreRenderer::destroySystem();
    }


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function creates an instance of the game class.
	 * 
	 * @return pointer to this game class.
	 *
	 */
    Game* Game::getInstance() {
        static Game game;   // The one and only Game.
        return &game;       // Return the address.
    }

	/**
	 * This function will initialize the game in order to start.
	 */
    void Game::go() {

		// Return nothing if the renderer isn't initialized properly.
        if (!initializeRenderer())
            return;

		// Otherwise, initialize input and resource assets.
		initializeInput();
        initializeResources();
		initializeCEGUI();

		// Initialize all states.
		MainMenuState::getInstance()->initialize();
		PlanetGameplayState::getInstance()->initialize();
		SpaceGameplayState::getInstance()->initialize();
		RoomGameplayState::getInstance()->initialize();
		PauseMenuState::getInstance()->initialize();
		CreditsState::getInstance()->initialize();
		GameOverState::getInstance()->initialize();
		//AC
        // Start in MainMenuState.
        switchState(MainMenuState::getInstance());

        // Enter the rendering loop.
        mRoot->startRendering();

		// Finalize all states.
		MainMenuState::getInstance()->finalize();
		PlanetGameplayState::getInstance()->finalize();
		SpaceGameplayState::getInstance()->finalize();
		PauseMenuState::getInstance()->finalize();
		RoomGameplayState::getInstance()->finalize();
		CreditsState::getInstance()->finalize();
		GameOverState::getInstance()->finalize();

		// Stop rendering.
        shutdownInput();
        shutdownRenderer();

		if (mGuiRenderer)
			CEGUI::OgreRenderer::destroySystem();

    }

	/**
	 * This function will initialize the game renderer.
	 */
    bool Game::initializeRenderer() {

        // Create root object.
        mRoot = new Ogre::Root(mPluginCfgName, mOgreCfgName, mLogName);

        // Create a window.
        if (mRoot->showConfigDialog()) {
            // If returned true, user clicked OK, so initialize.
            mWindow = mRoot->initialise(true, "Conjunction");
        } 
		
		else {
            // User clicked Cancel
            return false;
        }

        // Register self as event handler for window events.
        Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

        // Register self as frame listener
        mRoot->addFrameListener(this);

        // Create a viewport covering the entire window.
        mViewport = mWindow->addViewport(NULL);
        mViewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

        return true;
    }

	/**
	 * This function will initialize the game input.
	 */
    void Game::initializeInput() {

        // Create input manager for our window.
        size_t windowHnd = 0;
        mWindow->getCustomAttribute("WINDOW", &windowHnd);
        mInputMgr = OIS::InputManager::createInputSystem(windowHnd);

        // Create input objects.
        mKeyboard = dynamic_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
        mMouse = dynamic_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));

        // Adjust mouse clipping area.
        const OIS::MouseState& ms = mMouse->getMouseState();
        ms.width = mWindow->getWidth();
        ms.height = mWindow->getHeight();

        // Register self as callback handler for keyboard and mouse events.
        mKeyboard->setEventCallback(this);
        mMouse->setEventCallback(this);
    }

	/**
	 * This function will initialize the game's resources.
	 */
    void Game::initializeResources() {

        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

        // Load resource config file.
        Ogre::ConfigFile cf;
        cf.load(mResourceCfgName);

        // Find all resource groups and locations in config file.
        Ogre::ConfigFile::SectionIterator sec = cf.getSectionIterator();

        while (sec.hasMoreElements()) {
            Ogre::String secName = sec.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = sec.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i) {
                Ogre::String typeName = i->first;
                Ogre::String archName = i->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }

        // Initialize resource manager.
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

	/**
	 * This function will initialize the UI framework: CEGUI.
	 */
	void Game::initializeCEGUI() {
		// Create GUI renderer.
		mGuiRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
		CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
		CEGUI::System::getSingleton().setDefaultFont("Verdana");
		CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	}


// =====================================================================================
//
// #State Manager
//
// =====================================================================================

	/**
	 * This function switchs the current game state to another game state.
	 * 
	 * @param state			the new state to switch to
	 *
	 */
	void Game::switchState(GameState* state) {

		// Leave current state.
        if (mCurrentState != NULL)
            mCurrentState->leave();

        // Enter new state.
        mCurrentState = state;
        mCurrentState->enter();

	}


// =====================================================================================
//
// #Shutdown
//
// =====================================================================================

	/**
	 * This function will stop the renderer.
	 */
    void Game::shutdownRenderer() {
		// Shutdown Ogre.
        if (mRoot) {
            Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
            delete mRoot;
            mRoot = NULL;
        }
    }

	/**
	 * This function will stop the input handlers.
	 */
    void Game::shutdownInput() {
        // Destroy OIS handlers.
        if (mInputMgr) {
            if (mKeyboard) {
                mInputMgr->destroyInputObject(mKeyboard);
                mKeyboard = NULL;
            }

            if (mMouse) {
                mInputMgr->destroyInputObject(mMouse);
                mMouse = NULL;
            }

            mInputMgr->destroyInputSystem(mInputMgr);
            mInputMgr = NULL;
        }
    }


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will render every frame.
	 *
	 * @param evt			Frame Event that signals when a new frame is rendered
	 *
	 */
    bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt) {

        // Check if we should quit.
        if (mShouldQuit)
            return false;

        // Grab latest input device states.
        mKeyboard->capture();
        mMouse->capture();

        // Update the current game state.
        if (mCurrentState != NULL) 
			mCurrentState->frameRenderingQueued(evt);

        return true;

    }


// =====================================================================================
//
// #Event Listeners
//
// =====================================================================================

	/**
	 * This function will resize the game window if nessacery.
	 * 
	 * @param window		game window to render on
	 *
	 */
    void Game::windowResized(Ogre::RenderWindow* window) {

        // React for the main window that is created.
        if (window == mWindow) {

            // Adjust mouse clipping area.
            const OIS::MouseState& ms = mMouse->getMouseState();
            ms.width = window->getWidth();
            ms.height = window->getHeight();

        }

    }

	/**
	 * This function will close the game window.
	 * 
	 * @param window		game window to close
	 */
    void Game::windowClosed(Ogre::RenderWindow* window) {

        // React for the main window that is created.
        if (window == mWindow) {
            // Detach OIS before window shutdown.
            shutdownInput();
            mShouldQuit = true;
        }

    }

	/**
	 * This function will handle the event when a keyboard key is struck.
	 */
    bool Game::keyPressed(const OIS::KeyEvent& arg) {
        
        mCurrentState->keyPressed(arg);

        switch (arg.key) {

		// Setting this flag will terminate the rendering loop.
        case OIS::KC_ESCAPE:

			if (mCurrentState == PauseMenuState::getInstance()) {
				Game::getInstance()->switchState(Game::getInstance()->mPreviousState);
			}
			
			else if (mCurrentState == GameOverState::getInstance()) {
				std::cout << "can not pause game" << std::endl;	//AC
			} 
			
			else {

				if(mCurrentState == PlanetGameplayState::getInstance())
					mPreviousState = PlanetGameplayState::getInstance();

				else if(mCurrentState == SpaceGameplayState::getInstance())
					mPreviousState = SpaceGameplayState::getInstance();

				else if(mCurrentState == RoomGameplayState::getInstance())
					mPreviousState = RoomGameplayState::getInstance();
			
				if(mCurrentState != MainMenuState::getInstance())
					Game::getInstance()->switchState(PauseMenuState::getInstance());
				
			}

            break;

#if _WIN32
        // Toggle console.
        case OIS::KC_GRAVE:
            WindowsConsole::getInstance()->toggle();
            break;
#endif

        // Take a screenshot.
        case OIS::KC_F12:
            mWindow->writeContentsToTimestampedFile("screenshot", ".png");
            break;
        }

        return true;

    }

	/**
	 * This function will handle the event when a keyboard key is released.
	 */
    bool Game::keyReleased(const OIS::KeyEvent& arg) {
        mCurrentState->keyReleased(arg);
        return true;
    }

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool Game::mouseMoved(const OIS::MouseEvent& arg) {
        mCurrentState->mouseMoved(arg);
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool Game::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        mCurrentState->mousePressed(arg, id);
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool Game::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        mCurrentState->mouseReleased(arg, id);
        return true;
    }

}
