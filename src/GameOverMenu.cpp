#include "stdafx.h"

#include "gamestates.h"
#include "Game.h"
#include "MusicManager.h"

namespace capstone {

	/**
	 * GameOverState constructor.
	 */
    GameOverState::GameOverState()
        : MenuState()
		, mSceneMgr(NULL)
        , mTrayMgr(NULL)
        , mPlayAgainButton(NULL)
        , mExitButton(NULL)
    {
		
    }

	/**
	 * GameOverState destructor.
	 */
	GameOverState::~GameOverState() {

	}

	/**
	 * This function will create an instance of the game over state using the singleton design pattern.
	 *
	 * @return a pointer to this GameOverState class.
	 *
	 */
	GameOverState* GameOverState::getInstance() {
		static GameOverState menu;
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
    void GameOverState::initialize() {

		// Initialize the scene manager.
        mSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

		// Initialize the camera.
        mCamera = mSceneMgr->createCamera("GAME OVER CAMERA");
		mCamera->setNearClipDistance(1);
        mCamera->setFarClipDistance(5000);
        mCamera->setAutoAspectRatio(true);
        mCamera->setPosition(0, 0, -5);

		// Initialize the tray manager.
        mTrayMgr = new OgreBites::SdkTrayManager("GAME OVER",
                                                 Game::getInstance()->getWindow(),
                                                 Game::getInstance()->getMouse(),
                                                 this);
		
		// Instantiate buttons.
        mPlayAgainButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "PlayAgainButton", "Play Again", 200);
		mExitButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "ExitButton", "Exit Game", 200);
		
		// Set skybox for the menu.
		mSceneMgr->setSkyBox(true, "SpaceBox", 10, true);

		// Hide everything.
		mTrayMgr->hideAll();
	
	}


// =====================================================================================
//
// #Finalization
//
// =====================================================================================

	/**
	 * This function will destroy the main menu state.
	 */
    void GameOverState::finalize() {

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
    void GameOverState::enter() {
        Game::getInstance()->getViewport()->setCamera(mCamera);
        mTrayMgr->showTrays();
        mTrayMgr->showAll();
		MusicManager::getInstance()->playMainMenuMusic();
    }

	/**
	 * This function will leave the main menu state.
	 */
    void GameOverState::leave() {
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
    bool GameOverState::frameRenderingQueued(const Ogre::FrameEvent& evt) {

		// Update HUD.
		mPlayAgainButton->setCaption("Play Again");
		mExitButton->setCaption("Exit Game");
		mTrayMgr->frameRenderingQueued(evt);

		// Update FMOD.
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
    bool GameOverState::mouseMoved(const OIS::MouseEvent& arg) {
		mTrayMgr->injectMouseMove(arg);
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool GameOverState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		mTrayMgr->injectMouseDown(arg, id);
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool GameOverState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		mTrayMgr->injectMouseUp(arg, id);
        return true;
    }

	/**
	 * This function will handle when a button is clicked.
	 * 
	 * @param button			pointer to the button that was clicked
	 *
	 */
	void GameOverState::buttonHit(OgreBites::Button* button) {

		// Determine which button was clicked.
		if (button == mPlayAgainButton) {
			Game::getInstance()->switchState(MainMenuState::getInstance());
		}

		else if (button == mExitButton) {
			Game::getInstance()->quit();
		}

	}

}
