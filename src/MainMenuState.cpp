#include "stdafx.h"

#include "gamestates.h"
#include "Game.h"
#include "MusicManager.h"

namespace capstone {

	/**
	 * MainMenuState constructor.
	 */
    MainMenuState::MainMenuState()
        : MenuState()
		, mSceneMgr(NULL)
        , mTrayMgr(NULL)
        , mStartButton(NULL)
        , mQuitButton(NULL)
    {
		
    }

	/**
	 * MainMenuState destructor.
	 */
	MainMenuState::~MainMenuState() {

	}

	/**
	 * This function will create an instance of the main menu state using the singleton design pattern.
	 *
	 * @return a pointer to this MainMenuState class.
	 */
	MainMenuState* MainMenuState::getInstance() {
		static MainMenuState menu;
		return &menu;
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the main menu state.
	 */
    void MainMenuState::initialize() {

		mSpeed = -1;

		// Initialize the scene manager.
        mSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

		// Initialize the camera.
        mCamera = mSceneMgr->createCamera("MainMenuStateCamera");
		mCamera->setNearClipDistance(1);
        mCamera->setFarClipDistance(5000);
        mCamera->setAutoAspectRatio(true);
        mCamera->setPosition(0, 0, -5);

		// Initialize the tray manager.
        mTrayMgr = new OgreBites::SdkTrayManager("Main Menu",
                                                 Game::getInstance()->getWindow(),
                                                 Game::getInstance()->getMouse(),
                                                 this);

		mTrayMgr->showLogo(OgreBites::TL_TOP);
		
		// Instantiate buttons.
        mStartButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "StartButton", "Start Game", 200);
		mCreditsButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "CreditsButton", "Credits", 200);
        mQuitButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "QuitButton", "Quit", 200);
		
		// Set skybox for the menu.
		mSceneMgr->setSkyBox(true, "SpaceBox", 10, true);

		// Hide everything.
		mTrayMgr->hideAll();

		stateWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "_MasterRoot");
	
	}


// =====================================================================================
//
// #Finalization
//
// =====================================================================================

	/**
	 * This function will destroy the main menu state.
	 */
    void MainMenuState::finalize() {

		// Destroy the scene manager which destroys everything.
		Ogre::Root::getSingleton().destroySceneManager(mSceneMgr);

		// Re-assign the tray manager. 
        delete mTrayMgr;
        mTrayMgr = NULL;

		if (stateWindow != NULL) {
			// this destroys the root window and all its children automatically
			CEGUI::WindowManager::getSingleton().destroyWindow(stateWindow);
			stateWindow = NULL;
		}

    }


// =====================================================================================
//
// #Switching
//
// =====================================================================================

	/**
	 * This function will enter the main menu state.
	 */
    void MainMenuState::enter() {
        Game::getInstance()->getViewport()->setCamera(mCamera);
        mTrayMgr->showTrays();
        mTrayMgr->showAll();
		MusicManager::getInstance()->playMainMenuMusic();
    }

	/**
	 * This function will leave the main menu state.
	 */
    void MainMenuState::leave() {
        Game::getInstance()->getViewport()->setCamera(NULL);
        mTrayMgr->hideAll();

		MusicManager::getInstance()->stopMainMenuMusic();
    }


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will render every frame for the main menu state.
	 * 
	 * @param evt			frame event that signals when a new frame is rendered
	 *
	 */
    bool MainMenuState::frameRenderingQueued(const Ogre::FrameEvent& evt) {

		// Set captions for the buttons.
        mStartButton->setCaption("Start Game");
        mQuitButton->setCaption("Quit");
		mCreditsButton->setCaption("Credits");

		// Update HUD.
		mTrayMgr->frameRenderingQueued(evt);

		// Rotate camera.
		mCamera->yaw(mSpeed * evt.timeSinceLastFrame);

		// Update FMOD
		MusicManager::getInstance()->update(evt.timeSinceLastFrame);

        return true;
    }


// =====================================================================================
//
// #Event Listeners
//
// =====================================================================================

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool MainMenuState::mouseMoved(const OIS::MouseEvent& arg) {
		mTrayMgr->injectMouseMove(arg);
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool MainMenuState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		mTrayMgr->injectMouseDown(arg, id);
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool MainMenuState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		mTrayMgr->injectMouseUp(arg, id);
        return true;
    }

	/**
	 * This function will handle when a button is clicked.
	 * 
	 * @param button			pointer to the button that was clicked
	 *
	 */
	void MainMenuState::buttonHit(OgreBites::Button* button) {

		// Determine which button was clicked.
        if (button == mStartButton) {
			MusicManager::getInstance()->playStartSound();
			std::cout << "Go to the Planet Gameplay State" << std::endl;
			Game::getInstance()->switchState(PlanetGameplayState::getInstance());
        } 
		
		else if (button == mQuitButton) {
			Game::getInstance()->quit();
        }

		else if (button == mCreditsButton) {
			MusicManager::getInstance()->playMouseClick();
			Game::getInstance()->switchState(CreditsState::getInstance());
		}

	}

}
