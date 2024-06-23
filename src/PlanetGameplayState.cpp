#include "stdafx.h"

#include "gamestates.h"
#include "Levels.h"
#include "PlanetPlayer.h"
#include "Game.h"
#include "Buildings.h"
#include "MusicManager.h"
#include "GlobalVariables.h"

namespace capstone {

	/**
	 * PlanetGameplayState constructor.
	 */
	PlanetGameplayState::PlanetGameplayState() 
	    : GameplayState()
		, mSceneMgr(NULL)
        , mTrayMgr(NULL)
		, level(NULL)
		, playSound(false)
	{

	}

	/**
	 * PlanetGameplayState destructor.
	 */
	PlanetGameplayState::~PlanetGameplayState() {

	}

	/**
	 * This function will create an instance of the planet's gameplay state using the singleton design pattern.
	 * 
	 * @return a pointer to this PlanetGameplayState class.
	 *
	 */
	PlanetGameplayState* PlanetGameplayState::getInstance() {
		static PlanetGameplayState pGameplay;
		return &pGameplay;
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the planet's gameplay state.
	 */
	void PlanetGameplayState::initialize() {
		createLevel();
		initializeGui();
	}

	/**
	 * This function will create the level.
	 */
	void PlanetGameplayState::createLevel() {
		// New level object, 100 representing the radius of the sphere.
		level = new PlanetLevel(100);
	}

	/**
	 * This function will initialize the planet's UI.
	 */
	void PlanetGameplayState::initializeGui() {

		// Instantiate the tray manager.
        mTrayMgr = new OgreBites::SdkTrayManager("PlanetGameplayGui",
                                                 Game::getInstance()->getWindow(),
                                                 Game::getInstance()->getMouse(),
                                                 this);
        mTrayMgr->toggleAdvancedFrameStats();
        mTrayMgr->hideCursor();

		mEnterLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "EnterLabel", "'Q' Enter", 150);

        // Don't show anything yet.
        mTrayMgr->hideAll();

		// Initialize CEGUI windows.
		stateWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "planet_MasterRoot");
		win_planet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "planet_HUD");
		stateWindow->addChildWindow(win_planet);

		// Initialize labels.
		lbl_enter = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_enter");
		lbl_enter->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.9f,0)));;
		lbl_enter->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.06f, 0)));
		lbl_enter->setText("BUILDING!!!111onesonesones");
		lbl_enter->setAlpha(0.85f);
		win_planet->addChildWindow(lbl_enter);

		lbl_help = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "p_lbl_help");
		lbl_help->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.03f,0)));;
		lbl_help->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.06f, 0)));
		lbl_help->setText("Planet Tutorial");
		lbl_help->setAlpha(0.85f);
		win_planet->addChildWindow(lbl_help);

		lbl_helpDesc = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "p_lbl_desc");
		lbl_helpDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.1f,0)));
		lbl_helpDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.3f, 0)));
		lbl_helpDesc->setProperty("VertFormatting", "TopAligned");
		lbl_helpDesc->setAlpha(0.85f);
		win_planet->addChildWindow(lbl_helpDesc);

		std::string tutorialMsg = "Movement \n";
		tutorialMsg += "W - Forward \n";
		tutorialMsg += "S - Back \n";
		tutorialMsg += "A - Strafe Left \n";
		tutorialMsg += "D - Strafe Right \n";
		tutorialMsg += "Spacebar - Jump \n \n";
		tutorialMsg += "Actions \n";
		tutorialMsg += "E - Interact \n";
		tutorialMsg += "H - Toggle Help \n";

		lbl_helpDesc->setText(tutorialMsg);

	}


// =====================================================================================
//
// #Finalization
//
// =====================================================================================

	/**
	 * This function will destroy the planet's gameplay state.
	 */
	void PlanetGameplayState::finalize() {
		shutdownGui();
		destroyLevel();
	}

	/**
	 * This function will destroy the level.
	 */
	void PlanetGameplayState::destroyLevel() {
		delete level;
		level = NULL;
	}

	/**
	 * This function will destroy the GUI used for this game state.
	 */
	void PlanetGameplayState::shutdownGui() {
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
	 * This function will enter the planet's gameplay state.
	 */
    void PlanetGameplayState::enter() {

		// Show HUD.
		mTrayMgr->showTrays();
		mEnterLabel->hide();
		CEGUI::System::getSingleton().setGUISheet(stateWindow);
		CEGUI::MouseCursor::getSingleton().hide();
		win_planet->show();
		lbl_enter->hide();
		hideTutorial();
		Game::getInstance()->getViewport()->setCamera(level->getCamera());

		// Play background music.
		MusicManager::getInstance()->playPlanetMusic();
		
	}

	/**
	 * This function will leave the planet's gameplay state.
	 */
    void PlanetGameplayState::leave() {

		// Hide HUD.
		mTrayMgr->hideTrays();
		hideTutorial();
		lbl_enter->hide();
		CEGUI::MouseCursor::getSingleton().hide();
        Game::getInstance()->getViewport()->setCamera(NULL);

		// Stop background music.
		MusicManager::getInstance()->stopPlanetMusic();

		// Set the previous state so that the pause menu knows what state to go back to.
		Game::getInstance()->setPreviousState(this);
    }


// =====================================================================================
//
// #Update
//
// =====================================================================================

	/**
	 * This function will render every frame for the planet's gameplay state.
	 * 
	 * @param evt			frame event that signals when a new frame is rendered
	 *
	 */
    bool PlanetGameplayState::frameRenderingQueued(const Ogre::FrameEvent& evt) {

		// Update the level. which updates everything else.
		level->update(evt.timeSinceLastFrame);

		// Update HUD.
		lbl_enter->hide();
		updateHud();
		mTrayMgr->frameRenderingQueued(evt);

		// Update FMOD
		MusicManager::getInstance()->update(evt.timeSinceLastFrame);

        return true;
    }

	/**
	 * This function will update the GUI/HUD in game.
	 */
	void PlanetGameplayState::updateHud() {

		// Get the closest building.
		InteractiveBuilding* building = level->getClosestBuilding();

		if (building->isPlayerClose()) {
			
			if (playSound) {
				MusicManager::getInstance()->playInteractBuildingSound();
				playSound = false;
			}
			
			lbl_enter->setText(building->getName());
			lbl_enter->show();
		}

		else {
			playSound = true;
		}

	}


// =====================================================================================
//
// #Event Listeners
//
// =====================================================================================

	/**
	 * This function will handle the event when a keyboard key is struck.
	 */
	bool PlanetGameplayState::keyPressed(const OIS::KeyEvent& arg) {

		// Show tutorial menu.
		if (arg.key == OIS::KC_H) 
			setTutorialVisibility();

		if (!mTrayMgr->isDialogVisible()) {
			// Get the closest building.
			InteractiveBuilding* building = level->getClosestBuilding();

			// Interact with the building if the player is close.
			if (arg.key == OIS::KC_E && building->isPlayerClose()) {
				//MusicManager::getInstance()->playCursorMoveSound();
				mTrayMgr->showYesNoDialog(building->getQuestion(), building->getDesc());
			}
		}
			
		level->getKeyListener()->keyPressed(arg);

		return true;
	}

	/**
	 * This function will handle the event when a keyboard key is released.
	 */
	bool PlanetGameplayState::keyReleased(const OIS::KeyEvent& arg) {
		if (!mTrayMgr->isDialogVisible()) 
			level->getKeyListener()->keyReleased(arg);
		return true; 
	}

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool PlanetGameplayState::mouseMoved(const OIS::MouseEvent& arg) {
		if (mTrayMgr->isDialogVisible())
            mTrayMgr->injectMouseMove(arg);
		else 
			level->getMouseListener()->mouseMoved(arg);
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool PlanetGameplayState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		if (mTrayMgr->isDialogVisible())
            mTrayMgr->injectMouseDown(arg, id);
		else
			level->getMouseListener()->mousePressed(arg, id);
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool PlanetGameplayState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        if (mTrayMgr->isDialogVisible())
            mTrayMgr->injectMouseUp(arg, id);
		else
			level->getMouseListener()->mouseReleased(arg, id);
        return true;
    }

	/**
	 * This function will handle when the yes or no dialog buttons get selected.
	 */
	void PlanetGameplayState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit) {

		MusicManager::getInstance()->playMouseClick();

		// Get the closest building.
		InteractiveBuilding* building = level->getClosestBuilding();
		
		if (yesHit)		 { building->interact(); }
		else			 { // Do nothing. }
	
	}

}

}