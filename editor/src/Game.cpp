#include "stdafx.h"

#include "Game.h"
#include "Level.h"
#include "PlanetLevel.h"

#if _WIN32
#include "WindowsConsole.h"
#endif

namespace capstone {

	/**
	 * Game constructor.
	 */
    Game::Game()
        : mOgreCfgName("../ogre.cfg")            // render system config
#if _DEBUG
        , mPluginCfgName("../plugins_d.cfg")     // plugins to use in Debug configuration
#else
        , mPluginCfgName("../plugins.cfg")       // plugin to use in Release configuration
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
        , mLevel(NULL)
		, mTrayMgr(NULL)
    {

    }

	/**
	 * Game destructor.
	 */
    Game::~Game()
    {
        destroyLevel();
        shutdownInput();
        shutdownRenderer();
		shutdownGui();
    }


//=====================================================================================
//
// #Initialization
//
//=====================================================================================

	/**
	 * This function will initialize the game in order to start.
	 */
    void Game::go()
    {
		// Return nothing if the renderer isn't initialized properly.
        if (!initializeRenderer())
            return;

		// Otherwise, initialize input and resource assets.
        initializeInput();
        initializeResources();
		initializeGui();

		// Create the level.
        createLevel();

        // Enter the rendering loop.
        mRoot->startRendering();

		// Stop.
        destroyLevel();
        shutdownInput();
        shutdownRenderer();

    }

	/**
	 * This function will initialize the game renderer.
	 */
    bool Game::initializeRenderer()
    {
        // Create root object.
        mRoot = new Ogre::Root(mPluginCfgName, mOgreCfgName, mLogName);

        // Create a window.
        if (mRoot->showConfigDialog()) {
            // If returned true, user clicked OK, so initialize.
            mWindow = mRoot->initialise(true, "Capstone");
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
        mViewport->setBackgroundColour(Ogre::ColourValue(0.4f, 0.6f, 0.9f));

        return true;
    }

	/**
	 * This function will initialize the game input.
	 */
    void Game::initializeInput()
    {
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
    void Game::initializeResources()
    {
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
	 * This function will initialize the GUI.
	 */
	void Game::initializeGui() {
		
		// Initialize SdkTrayManager.
        mTrayMgr = new OgreBites::SdkTrayManager("Capstone", mWindow, mMouse, this);
        mTrayMgr->showFrameStats(OgreBites::TL_TOPRIGHT);
        mTrayMgr->toggleAdvancedFrameStats();
        mTrayMgr->hideCursor();

	}

	/**
	 * This function will create the level.
	 */
    void Game::createLevel() {

        // If a level already exists, destroy it.
        if (mLevel) 
            destroyLevel();
		
		// New level.
		mLevel = new PlanetLevel(this);

    }


//=====================================================================================
//
// #Cleanup
//
//=====================================================================================

	/**
	 * This function will stop the renderer.
	 */
    void Game::shutdownRenderer()
    {
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
    void Game::shutdownInput()
    {
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

	/**
	 * This function will shutdown the GUI.
	 */
	void Game::shutdownGui() {
        delete mTrayMgr;
        mTrayMgr = NULL;
	}

	/**
	 * This function will destory the level.
	 */
    void Game::destroyLevel() {
        delete mLevel;
        mLevel = NULL;
    }


//=====================================================================================
//
// #Update/Render
//
//=====================================================================================

	/**
	 * This function will render every frame.
	 * 
	 * @param evt				event object handling each frame being rendered
	 *
	 */
    bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt) {
        // Check if we should quit.
        if (mShouldQuit)
            return false;

        // Grab latest input device states.
        mKeyboard->capture();
        mMouse->capture();

		// Update game HUD.
		updateHud();

        // Update the world.
        if (mLevel) {
            
            // Update the world for this frame.
            mLevel->update(evt.timeSinceLastFrame);

            // Make sure we're using the world's active camera.
            mViewport->setCamera(mLevel->getActiveCamera());

        }

		
		mTrayMgr->frameRenderingQueued(evt);

        return true;
    }

	/**
	 * This function will update the HUD.
	 */
	void Game::updateHud() {

	}


//=====================================================================================
//
// #I/O Event Listeners
//
//=====================================================================================

	/**
	 * This function will resize the game window if nessacery.
	 */
    void Game::windowResized(Ogre::RenderWindow* window)
    {
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
	 */
    void Game::windowClosed(Ogre::RenderWindow* window)
    {
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
    bool Game::keyPressed(const OIS::KeyEvent& arg)
    {
        // Tell the current World about this event.
        if (mLevel && mLevel->getKeyListener())
            mLevel->getKeyListener()->keyPressed(arg);

        switch (arg.key) {

		// Setting this flag will terminate the rendering loop.
        case OIS::KC_ESCAPE:
            mShouldQuit = true;
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
    bool Game::keyReleased(const OIS::KeyEvent& arg)
    {
        // Tell the current World about this event.
        if (mLevel && mLevel->getKeyListener())
            mLevel->getKeyListener()->keyReleased(arg);

        return true;
    }

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool Game::mouseMoved(const OIS::MouseEvent& arg)
    {
		// Enable movement for the mouse if the dialog is present.
        if (mTrayMgr->isDialogVisible())
            mTrayMgr->injectMouseMove(arg);

        // Tell the current World about this event.
        if (mLevel && mLevel->getMouseListener())
            mLevel->getMouseListener()->mouseMoved(arg);

        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool Game::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
    {
		// Enable mouse events when the popup is present.
        if (mTrayMgr->isDialogVisible())
            mTrayMgr->injectMouseDown(arg, id);

        // Tell the current World about this event.
        if (mLevel && mLevel->getMouseListener())
            mLevel->getMouseListener()->mousePressed(arg, id);

        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool Game::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
    {
		// Enable mouse events when the popup is present.
        if (mTrayMgr->isDialogVisible())
            mTrayMgr->injectMouseUp(arg, id);

        // Tell the current World about this event.
        if (mLevel && mLevel->getMouseListener())
            mLevel->getMouseListener()->mouseReleased(arg, id);

        return true;
    }

	/** 
	 * This function will prompt the user will a dialog box.
	 */
    void Game::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit) {

		if (question == "Leave?") {
            if (yesHit) {
                createLevel();
			}
            else 
                mShouldQuit = true;
        }

    }

} 
