#include "stdafx.h"

#include "gamestates.h"
#include "Game.h"
#include "MusicManager.h"

namespace capstone {
	
	// Static variables.
	Ogre::Real CreditsState::speed = 0.03f;

	/**
	 * CreditsState constructor.
	 */
    CreditsState::CreditsState()
        : MenuState()
		, mSceneMgr(NULL)
        , mTrayMgr(NULL)
		, scrollSpeed(speed)
    {
		
    }

	/**
	 * CreditsState destructor.
	 */
	CreditsState::~CreditsState() {

	}

	/**
	 * This function will create an instance of the main menu state using the singleton design pattern.
	 *
	 * @return a pointer to this CreditsState class.
	 *
	 */
	CreditsState* CreditsState::getInstance() {
		static CreditsState menu;
		return &menu;
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the credits list state.
	 */
    void CreditsState::initialize() {

		// Initialize the scene manager.
        mSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

		// Initialize the camera.
        mCamera = mSceneMgr->createCamera("CreditsStateCamera");
		mCamera->setNearClipDistance(1);
        mCamera->setFarClipDistance(5000);
        mCamera->setAutoAspectRatio(true);
        mCamera->setPosition(0, 0, 0);

		msg = "Conjunction \nAn Epidemic Production \n \n";
		msg += "Producer \nJayvin Duong \n \n";
		msg += "Q/A Lead \nKyle Van Grinsven \n \n";
		msg += "Lead Game Designer \nManav Rajrishi \n \n";
		msg += "Lead Programmer \nManav Rajrishi \n \n";
		msg += "Art Director \nJayvin Duong \n \n";
		msg += "Game Designers: \nAllan Corda \nJayvin Duong \nKyle Van Grinsven \n \n";
		msg += "Programmers: \nAllan Corda \nJayvin Duong \nKyle Van Grinsven \n \n";
		msg += "Tools Programmers: \nJayvin Duong \nKyle Van Grinsven \n \n";
		msg += "Concept Artists: \nJayvin Duong \nAerith N. \n \n";
		msg += "Texture Artists: \nJayvin Duong \nNu-Lee Han \nAerith N. \nCesar Vonc \n \n";
		msg += "3D Modelers: \nOmar Chaudry \nJayvin Duong \nDaniel Haughton \nDave Hribar \nAndrey Kurin \nIvan Malek \n";
		msg += "Aerith N. \nChudnov Y. Nickolas\nFabio Passaro \nRhys Salcombe \nChris Sapiano \nAlexander Shareef \nMiyuki T. \n \n";
		msg += "Sound: \nKyle Van Grinsven \nMike Koenig \n \n";

		setupGUI();
	
	}

	/**
	 * This function will initialize the credits list UI.
	 */
	void CreditsState::setupGUI() {
		// Initialize the tray manager.
        mTrayMgr = new OgreBites::SdkTrayManager("CreditsList",
                                                 Game::getInstance()->getWindow(),
                                                 Game::getInstance()->getMouse(),
                                                 this);

		mTextbox = mTrayMgr->createTextBox(OgreBites::TL_CENTER, "tbx_credits", "Credits", 400, 300);
		mTextbox->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin vitae auctor justo.");

		// Set skybox for the menu.
		mSceneMgr->setSkyBox(true, "SpaceBox", 10, true);

		// Hide everything.
		mTrayMgr->hideAll();
		
		// Create a CEGUI window.
		stateWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "Credits_MasterRoot");
	}


// =====================================================================================
//
// #Finalization
//
// =====================================================================================

	/**
	 * This function will destroy the credits list state.
	 */
    void CreditsState::finalize() {

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
	 * This function will enter the credits list state.
	 */
    void CreditsState::enter() {
		scrollSpeed = speed;
		percent = 0;
        Game::getInstance()->getViewport()->setCamera(mCamera);
        mTrayMgr->showTrays();
        mTrayMgr->showAll();
		MusicManager::getInstance()->playMainMenuMusic();
    }

	/**
	 * This function will leave the credits list state.
	 */
    void CreditsState::leave() {
		scrollSpeed = 0;
		percent = 0;
		mTextbox->setScrollPercentage(0);
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
	 * This function will render every frame for the credits list state.
	 * 
	 * @param evt			frame event that signals when a new frame is rendered
	 *
	 */
    bool CreditsState::frameRenderingQueued(const Ogre::FrameEvent& evt) {

		percent += (scrollSpeed * evt.timeSinceLastFrame) * 0.75f;

		if (mTextbox->getScrollPercentage() >= 1) {
			MusicManager::getInstance()->playBackSound();
			Game::getInstance()->switchState(MainMenuState::getInstance());
		}

		else {
			// Update HUD.
			mTextbox->setText(msg);
			mTextbox->setScrollPercentage(percent);
			mTrayMgr->frameRenderingQueued(evt);

			// Update FMOD
			MusicManager::getInstance()->update(evt.timeSinceLastFrame);
		}

        return true;
    }


// =====================================================================================
//
// #Event Listeners
//
// =====================================================================================

	/**
	 * This function will handle the event when a keyboard key is struck.
	 */
	bool CreditsState::keyPressed(const OIS::KeyEvent& arg) {

		// Exit this game state.
		if (arg.key == OIS::KC_ESCAPE) {
			MusicManager::getInstance()->playBackSound();
			Game::getInstance()->switchState(MainMenuState::getInstance());
		}
		
		return true;
	}

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool CreditsState::mouseMoved(const OIS::MouseEvent& arg) {
		mTrayMgr->injectMouseMove(arg);
        return true;
    }

}
