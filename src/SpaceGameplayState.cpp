#include "stdafx.h"

#include "gamestates.h"
#include "SpaceLevel.h"
#include "Game.h"
#include "GlobalVariables.h"
#include "SpacePlayer.h"
#include "MusicManager.h"
#include "Attributes.h"
#include "Enemy.h"

namespace capstone {

	/**
	 * SpaceGameplayState constructor.
	 */
	SpaceGameplayState::SpaceGameplayState() 
		: GameplayState()
		, mSceneMgr(NULL)
        , mTrayMgr(NULL)
		, level(NULL)
		, mCrosshairOverlay(NULL)
	{

	}

	/**
	 * SpaceGameplayState destructor.
	 */
	SpaceGameplayState::~SpaceGameplayState() {

	}

	/**
	 * This function will create an instance of the space's gameplay state using the singleton design pattern.
	 * 
	 * @return a pointer to this SpaceGameplayState class.
	 *
	 */
	SpaceGameplayState* SpaceGameplayState::getInstance() {
		static SpaceGameplayState sGameplay;
		return &sGameplay;
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the planet's gameplay state.
	 */
	void SpaceGameplayState::initialize() {
		// Set level to the beginning.
		currentWave = 1;

		initializeGui();
	}

	/**
	 * This function will create the level.
	 */
	void SpaceGameplayState::createLevel() {
		// New level object.
		level = new SpaceLevel(currentWave);
	}

	/**
	 * This function will initialize the space's level UI.
	 */
	void SpaceGameplayState::initializeGui() {

		// Instantiate the tray manager.
        mTrayMgr = new OgreBites::SdkTrayManager("SpaceGameplayGui",
                                                 Game::getInstance()->getWindow(),
                                                 Game::getInstance()->getMouse(),
                                                 this);

		//Lable for the Health GUI to go Top left of the screen
		mHealthLabel = mTrayMgr->createLabel(OgreBites::TL_TOPLEFT, "HealthLabel", "Health: ---", 125);

		// Game's crosshairs.
		mCrosshairOverlay = Ogre::OverlayManager::getSingleton().getByName("CrosshairOverlay");
		mCrosshairOverlay->hide();

        // Don't show anything yet.
        mTrayMgr->toggleAdvancedFrameStats();
        mTrayMgr->hideCursor();
        mTrayMgr->hideAll();

		// Setup the UI.
		stateWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "s_MasterRoot");
		win_intermission = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "win_intermission");
		stateWindow->addChildWindow(win_intermission);

		// Label for when the level is done.
		lbl_summary = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_summary");
		lbl_summary->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.15f,0)));;
		lbl_summary->setSize(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.5f, 0)));
		lbl_summary->setProperty("VertFormatting", "TopAligned");
		lbl_summary->setText("Level Summary");
		lbl_summary->setAlpha(0.75f);
		win_intermission->addChildWindow(lbl_summary);
		
		// Button to end the level.
		btn_end = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_end");
		btn_end->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.7f, 0)));
		btn_end->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.1f, 0)));
		btn_end->setText("End Level");
		btn_end->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&SpaceGameplayState::onButtonMouseOver, this));
		btn_end->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SpaceGameplayState::onEndClicked, this));
		win_intermission->addChildWindow(btn_end);

		// Setup the health bar.
		healthBar = (CEGUI::ProgressBar*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/ProgressBar", "HealthBar");
		healthBar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.03f,0)));
		healthBar->setSize(CEGUI::UVector2(CEGUI::UDim(0.24f, 0), CEGUI::UDim(0.05f, 0)));
		healthBar->setProgress(1);
		healthBar->setStepSize(0.01f);
		stateWindow->addChildWindow(healthBar);

		lbl_help = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "s_lbl_help");
		lbl_help->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.1f,0)));;
		lbl_help->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.06f, 0)));
		lbl_help->setText("Combat Tutorial");
		lbl_help->setAlpha(0.85f);
		stateWindow->addChildWindow(lbl_help);

		lbl_helpDesc = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "s_lbl_desc");
		lbl_helpDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.17f,0)));
		lbl_helpDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.35f, 0)));
		lbl_helpDesc->setProperty("VertFormatting", "TopAligned");
		lbl_helpDesc->setAlpha(0.85f);
		stateWindow->addChildWindow(lbl_helpDesc);

		std::string tutorialMsg = "Movement \n";
		tutorialMsg += "W - Forward \n";
		tutorialMsg += "S - Back \n";
		tutorialMsg += "A - Strafe Left \n";
		tutorialMsg += "D - Strafe Right \n";
		tutorialMsg += "Shift - Elevate Down \n";
		tutorialMsg += "Spacebar - Elevate Up \n \n";
		tutorialMsg += "Actions \n";
		tutorialMsg += "1 - Realloc State 1 \n";
		tutorialMsg += "2 - Realloc State 2 \n";
		tutorialMsg += "2 - Realloc State 3";

		lbl_helpDesc->setText(tutorialMsg);

	}


// =====================================================================================
//
// #Finalization
//
// =====================================================================================

	/**
	 * This function will destroy the level.
	 */
	void SpaceGameplayState::destroyLevel() {
		delete level;
		level = NULL;
	}
	
	/**
	 * This function will destroy the space's gameplay state.
	 */
	void SpaceGameplayState::finalize() {
		shutdownGui();
		destroyLevel();
	}

	/**
	 * This function will initialize the GUI used for this game state.
	 */
	void SpaceGameplayState::shutdownGui() {
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
// #Swtiching
//
// =====================================================================================

	/**
	 * This function will enter the space's gameplay state.
	 */
    void SpaceGameplayState::enter() {

		createLevel();

		// Show HUD.
		//mTrayMgr->showTrays(); // Hiding the health label.
		Game::getInstance()->getViewport()->setCamera(level->getCamera());
		mCrosshairOverlay->show();
		CEGUI::System::getSingleton().setGUISheet(stateWindow);
		stateWindow->show();
		win_intermission->hide();
		hideTutorial();
		healthBar->setProgress(1);
		CEGUI::MouseCursor::getSingleton().hide();

		// Make sure reallocStates carry over.
		level->getPlayer()->setCurrentReallocState();
		Attributes::getInstance()->resetLevelProperties();

		std::vector<Enemy*> enemies = level->getEnemies();
		for(unsigned i=0;i<enemies.size();i++){
			enemies[i]->setupSounds();
		}

		// Play background music.
		MusicManager::getInstance()->playSpaceMusic();
    }

	/**
	 * This function will leave the space's gameplay state.
	 */
    void SpaceGameplayState::leave() {
		
		// Hide HUD.
		mTrayMgr->hideTrays();
		mCrosshairOverlay->hide();
		stateWindow->hide();
		Game::getInstance()->getViewport()->setCamera(NULL);
		CEGUI::MouseCursor::getSingleton().hide();

		// Free up all entites.
		level->getPlayer()->getHeadNode()->detachAllObjects();
		level->getPlayer()->getLeftArmNode()->detachAllObjects();
		level->getPlayer()->getRightArmNode()->detachAllObjects();
		level->getPlayer()->getLeftShoulderNode()->detachAllObjects();
		level->getPlayer()->getRightShoulderNode()->detachAllObjects();

		// Set the next wave, if all enemies are killed.
		if (level->isLevelComplete()) {
			currentWave++;
		}

		std::vector<Enemy*> enemies = level->getEnemies();

		for(unsigned i=0;i<enemies.size();i++){
			enemies[i]->stopJetSound();
		}

		// Destroy level.
		destroyLevel();

		// Stop background music.
		MusicManager::getInstance()->stopSpaceMusic();

		// Set the previous state so that the pause menu knows what state to go back to.
		Game::getInstance()->setPreviousState(this);
    }


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will render every frame for the space's gameplay state.
	 * 
	 * @param evt			frame event that signals when a new frame is rendered
	 *
	 */
    bool SpaceGameplayState::frameRenderingQueued(const Ogre::FrameEvent& evt) {

		// Update the level. which updates everything else.
		level->update(evt.timeSinceLastFrame);

		// Show the mouse cursor when the level is done.
		if (level->isLevelComplete()) {
			CEGUI::MouseCursor::getSingleton().show();
			win_intermission->show();

			std::string msg = "Level Complete \n \n";
			msg += intToString(Attributes::getInstance()->getShotCount()) +" shots fired \n";
			msg += intToString(Attributes::getInstance()->getHitCount()) +" shots hit \n \n";
			msg += "Resources Collected: \n \n";
			msg += intToString(Attributes::getInstance()->getMatterGained()) +" dark matter added \n";
			msg += intToString(Attributes::getInstance()->getMetalGained()) +" metal scrap added: \n";
			msg += intToString(Attributes::getInstance()->getCarbonGained()) +" carbon added: \n";

			lbl_summary->setText(msg);
		} else if (level->getPlayer()->getHealth() <= 0.0f){
			resetGame();
			Game::getInstance()->switchState(GameOverState::getInstance());
		}
		
		// Update HUD.
		mTrayMgr->frameRenderingQueued(evt);
		CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

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
	 * This function will handle the event when a keyboard key is struck.
	 */
	bool SpaceGameplayState::keyPressed(const OIS::KeyEvent& arg) {
			
		if (!level->isLevelComplete()) {
			// Show tutorial menu.
			if (arg.key == OIS::KC_H) 
				setTutorialVisibility();
			level->getKeyListener()->keyPressed(arg);
		}

		return true;
	}

	/**
	 * This function will handle the event when a keyboard key is released.
	 */
	bool SpaceGameplayState::keyReleased(const OIS::KeyEvent& arg) {

		if (!level->isLevelComplete()) {
			level->getKeyListener()->keyReleased(arg);
		}

		return true; 
	}

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool SpaceGameplayState::mouseMoved(const OIS::MouseEvent& arg) {

		float dx = (float)arg.state.X.rel;
		float dy = (float)arg.state.Y.rel;

		if (!level->isLevelComplete())
			level->getMouseListener()->mouseMoved(arg);
		else 
			CEGUI::System::getSingleton().injectMouseMove(dx, dy);

        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool SpaceGameplayState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {

		if (!level->isLevelComplete())
			level->getMouseListener()->mousePressed(arg, id);
		else
			CEGUI::System::getSingleton().injectMouseButtonDown(ConvertButton(id));

        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool SpaceGameplayState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {

		if (!level->isLevelComplete())
			level->getMouseListener()->mouseReleased(arg, id);
		else 
			CEGUI::System::getSingleton().injectMouseButtonUp(ConvertButton(id));

        return true;
    }

	/**
	 * This function will handle the event for when the mouse is over the area of the button.
	 */
	bool SpaceGameplayState::onButtonMouseOver(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseOverSound();
		return true;
	}

	/**
	 * This function will handle when the end level button gets clicked.
	 */
	bool SpaceGameplayState::onEndClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playBackSound();
		Game::getInstance()->switchState(PlanetGameplayState::getInstance());
		return true;
	}

}