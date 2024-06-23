#include "stdafx.h"

#include "gamestates.h"
#include "Game.h"
#include "Attributes.h"
#include "Part.h"
#include "CompletePart.h"
#include "MusicManager.h"
#include "GlobalVariables.h"
#include "Skill.h"


namespace capstone {

	/**
	 * PauseMenuState constructor.
	 */
    PauseMenuState::PauseMenuState()
		: MenuState()
		, mSceneMgr(NULL)
		, previousState(0)
		, transparency(0.8f)
    {

    }

	/**
	 * PauseMenuState destructor.
	 */
	PauseMenuState::~PauseMenuState() {

	}

	/**
	 * This function will create an instance of the pause menu state using the singleton design pattern.
	 * 
	 * @return a pointer to this PauseMenuState class.
	 *
	 */
	PauseMenuState* PauseMenuState::getInstance() {
		static PauseMenuState pMenu;
		return &pMenu;
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the pause menu state.
	 */
    void PauseMenuState::initialize() {

		// Initialize the scene manager.
        mSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

		// Get the width and height of the game application window.
		int appWidth = Game::getInstance()->getViewport()->getActualWidth();
		int appHeight = Game::getInstance()->getViewport()->getActualHeight();

		// Initialize the camera.
        mCamera = mSceneMgr->createCamera("PauseMenuStateCamera");
		mCamera->setNearClipDistance(1);
        mCamera->setFarClipDistance(5000);
        mCamera->setAutoAspectRatio(true);
        mCamera->setPosition(0, 0, -5);
		mCamera->pitch(Ogre::Degree(90));

		// Set skybox for the menu.
		mSceneMgr->setSkyBox(true, "SpaceBox", 10, true);

		// Setup the UI.
		initMenu();
		initReallocMenu();
		initControlsMenu();

	}

	/**
	 * This function sets up the positioning and size of the menu UI.
	 */
	void PauseMenuState::initMenu() {

		// Variables for the UI.
		float xPos = -0.05f;

		// Setup the UI.
		stateWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "p_MasterRoot");

		// Window panel to hold the main buttons.
		win_menu = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "PauseMenu");
		stateWindow->addChildWindow(win_menu);
		
		// Resume button.
		btn_resume = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_resume");
		btn_resume->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.05f, 0)));
		btn_resume->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_resume->setAlpha(transparency);
		btn_resume->setText("Resume Game");
		btn_resume->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_resume->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onResumeClicked, this));
		win_menu->addChildWindow(btn_resume);

		// Realloc state button.
		btn_realloc = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_realloc");
		btn_realloc->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.20f, 0)));
		btn_realloc->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_realloc->setAlpha(transparency);
		btn_realloc->setText("Realloc States");
		btn_realloc->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_realloc->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onReallocClicked, this));
		win_menu->addChildWindow(btn_realloc);

		// Controls button.
		btn_controls = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_controls");
		btn_controls->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.35f, 0)));
		btn_controls->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_controls->setAlpha(transparency);
		btn_controls->setText("Controls");
		btn_controls->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_controls->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onControlsClicked, this));
		win_menu->addChildWindow(btn_controls);

		// Exit Game button.
		btn_exit = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_exit");
		btn_exit->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.5f, 0)));
		btn_exit->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_exit->setAlpha(transparency);
		btn_exit->setText("Exit Game");
		btn_exit->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_exit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onExitClicked, this));
		win_menu->addChildWindow(btn_exit);

		// Label to display the current wave the player is on.
		lbl_wave = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_wave");
		lbl_wave->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.15f, 0)));
		lbl_wave->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.1f, 0)));
		lbl_wave->setAlpha(transparency);
		lbl_wave->setText("Current Wave: ");
		win_menu->addChildWindow(lbl_wave);

		// Label to display the header for the resources.
		lbl_stats = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_heading");
		lbl_stats->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.3f, 0)));
		lbl_stats->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.1f, 0)));
		lbl_stats->setAlpha(transparency);
		lbl_stats->setText("Resources");
		win_menu->addChildWindow(lbl_stats);

		// Label to display the player's resources.
		lbl_resources = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_resources");
		lbl_resources->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.45f, 0)));
		lbl_resources->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.16f,0)));
		lbl_resources->setText("Dark Matter: --- \n \nMetal: --- \n \nCarbon: ---");
		lbl_resources->setProperty("VertFormatting", "TopAligned");
		lbl_resources->setAlpha(transparency);
		win_menu->addChildWindow(lbl_resources);

	}

	/**
	 * This function sets up the positioning and size of the realloc state menu.
	 */
	void PauseMenuState::initReallocMenu() {

		// Variables for the UI.
		float xPos = -0.05f;
		float labelHeight = 0.1f;

		// Window panel to show the realloc states.
		win_realloc = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "ReallocMenu");
		stateWindow->addChildWindow(win_realloc);

		// Back button.
		btn_back = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_back");
		btn_back->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.05f, 0)));
		btn_back->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_back->setAlpha(transparency);
		btn_back->setText("Back");
		btn_back->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_back->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onBackClicked, this));
		win_realloc->addChildWindow(btn_back);

		// Realloc #1 button.
		btn_r1 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_r1");
		btn_r1->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.2f, 0)));
		btn_r1->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f,0)));
		btn_r1->setAlpha(transparency);
		btn_r1->setText("Realloc #1");
		btn_r1->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_r1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onR1Clicked, this));
		win_realloc->addChildWindow(btn_r1);

		// Realloc #2 button.
		btn_r2 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_r2");
		btn_r2->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.35f, 0)));
		btn_r2->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_r2->setAlpha(transparency);
		btn_r2->setText("Realloc #2");
		btn_r2->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_r2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onR2Clicked, this));
		win_realloc->addChildWindow(btn_r2);

		// Realloc #3 button.
		btn_r3 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_r3");
		btn_r3->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.5f, 0)));
		btn_r3->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_r3->setAlpha(transparency);
		btn_r3->setText("Realloc #3");
		btn_r3->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_r3->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onR3Clicked, this));
		win_realloc->addChildWindow(btn_r3);

		// Label for the item in the head socket.
		lbl_headSlot = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_head");
		lbl_headSlot->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.15f, 0)));;
		lbl_headSlot->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(labelHeight, 0)));
		lbl_headSlot->setAlpha(transparency);
		lbl_headSlot->setText("Head \nName: Ogre Head \nDamage: 100");
		lbl_headSlot->setProperty("VertFormatting", "TopAligned");
		win_realloc->addChildWindow(lbl_headSlot);

		// Label for the item in the left shoulder socket.
		lbl_leftShoulderSlot = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_ls");
		lbl_leftShoulderSlot->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.30f,0)));;
		lbl_leftShoulderSlot->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(labelHeight, 0)));
		lbl_leftShoulderSlot->setAlpha(transparency);
		lbl_leftShoulderSlot->setText("Left Shoulder \nName: Left Shoulder Cannon\n Damage: 100");
		lbl_leftShoulderSlot->setProperty("VertFormatting", "TopAligned");
		win_realloc->addChildWindow(lbl_leftShoulderSlot);

		// Label for the item in the right shoulder socket.
		lbl_rightShoulderSlot = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_rs");
		lbl_rightShoulderSlot->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.45f, 0)));;
		lbl_rightShoulderSlot->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(labelHeight, 0)));
		lbl_rightShoulderSlot->setAlpha(transparency);
		lbl_rightShoulderSlot->setText("Right Shoulder \nName: Right Shoulder Cannon\n Damage: 100");
		lbl_rightShoulderSlot->setProperty("VertFormatting", "TopAligned");
		win_realloc->addChildWindow(lbl_rightShoulderSlot);

		// Label for the item in the left arm socket.
		lbl_leftArmSlot = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_la");
		lbl_leftArmSlot->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.60f, 0)));;
		lbl_leftArmSlot->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(labelHeight, 0)));
		lbl_leftArmSlot->setAlpha(transparency);
		lbl_leftArmSlot->setText("Left Arm \nName: Left Arm Cannon\n Damage: 100");
		lbl_leftArmSlot->setProperty("VertFormatting", "TopAligned");
		win_realloc->addChildWindow(lbl_leftArmSlot);

		// Label for the item in the right arm socket.
		lbl_rightArmSlot = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_ra");
		lbl_rightArmSlot->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.75f, 0)));;
		lbl_rightArmSlot->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(labelHeight, 0)));
		lbl_rightArmSlot->setAlpha(transparency);
		lbl_rightArmSlot->setText("Right Arm \nName: Right Arm Cannon\n Damage: 100");
		lbl_rightArmSlot->setProperty("VertFormatting", "TopAligned");
		win_realloc->addChildWindow(lbl_rightArmSlot);

	}

	/**
	 * This function sets up the positioning and size of the controls menu.
	 */
	void PauseMenuState::initControlsMenu() {

		// Variables for the UI.
		float xPos = -0.05f;

		// Window panel to hold the control UI elements.
		win_controls = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "ControlsMenu");
		stateWindow->addChildWindow(win_controls);

		// Create buttons for the controls screen.
		btn_cback = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_controls_back");
		btn_cback->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.05f, 0)));
		btn_cback->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_cback->setAlpha(transparency);
		btn_cback->setText("Back");
		btn_cback->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_cback->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onControlsBackClicked, this));
		win_controls->addChildWindow(btn_cback);

		btn_planet = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_planet");
		btn_planet->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.2f, 0)));
		btn_planet->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_planet->setAlpha(transparency);
		btn_planet->setText("Planet");
		btn_planet->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_planet->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onPlanetClicked, this));
		win_controls->addChildWindow(btn_planet);

		btn_space = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_space");
		btn_space->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.35f, 0)));
		btn_space->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_space->setAlpha(transparency);
		btn_space->setText("Space");
		btn_space->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_space->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onSpaceClicked, this));
		win_controls->addChildWindow(btn_space);

		btn_room = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "btn_room");
		btn_room->setPosition(CEGUI::UVector2(CEGUI::UDim(xPos, 0), CEGUI::UDim(0.5f, 0)));
		btn_room->setSize(CEGUI::UVector2(CEGUI::UDim(0.31f, 0), CEGUI::UDim(0.1f, 0)));
		btn_room->setAlpha(transparency);
		btn_room->setText("Armory/Factory");
		btn_room->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::Event::Subscriber(&PauseMenuState::onButtonMouseOver, this));
		btn_room->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseMenuState::onRoomClicked, this));
		win_controls->addChildWindow(btn_room);

		// Labels for list the controls.
		lbl_controlsList = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_controlsList");
		lbl_controlsList->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.1f, 0)));;
		lbl_controlsList->setSize(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.8f, 0)));
		lbl_controlsList->setAlpha(transparency);
		lbl_controlsList->setProperty("VertFormatting", "TopAligned");
		win_controls->addChildWindow(lbl_controlsList);

	}


// =====================================================================================
//
// #Finalization
//
// =====================================================================================

	/**
	 * This function will destroy the pause menu state.
	 */
    void PauseMenuState::finalize() {
		// Destroy the scene manager which destroys everything.
		Ogre::Root::getSingleton().destroySceneManager(mSceneMgr);

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
    void PauseMenuState::enter() {
        Game::getInstance()->getViewport()->setCamera(mCamera);
		CEGUI::System::getSingleton().setGUISheet(stateWindow);
		stateWindow->show();
		win_realloc->hide();
		win_controls->hide();
		win_menu->show();
		displayResources();
		CEGUI::MouseCursor::getSingleton().show();
		MusicManager::getInstance()->playPauseMusic();
    }

	/**
	 * This function will leave the main menu state.
	 */
    void PauseMenuState::leave() {
        Game::getInstance()->getViewport()->setCamera(NULL);
		CEGUI::MouseCursor::getSingleton().hide();
		stateWindow->hide();
		MusicManager::getInstance()->stopPauseMusic();
    }


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will render every frame for the pause menu state.
	 * 
	 * @param evt		frame Event that signals when a new frame is rendered
	 *
	 */
    bool PauseMenuState::frameRenderingQueued(const Ogre::FrameEvent& evt) {
		CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
		MusicManager::getInstance()->update(evt.timeSinceLastFrame);
        return true;
    }


// =====================================================================================
//
// #Event Handlers
//
// =====================================================================================

	/**
	 * This function will handle for when the mouse moves.
	 */
    bool PauseMenuState::mouseMoved(const OIS::MouseEvent& arg) {
		float dx = (float)arg.state.X.rel;
		float dy = (float)arg.state.Y.rel;
		CEGUI::System::getSingleton().injectMouseMove(dx, dy);
        return true;
    }

	/**
	 * This function will handle the event for when the mouse is clicked.
	 */
    bool PauseMenuState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		CEGUI::System::getSingleton().injectMouseButtonDown(ConvertButton(id));
        return true;
    }

	/**
	 * This function will handle the event for when the mouse is clicked is over.
	 */
    bool PauseMenuState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		CEGUI::System::getSingleton().injectMouseButtonUp(ConvertButton(id));
        return true;
    }

	/**
	 * This function will handle the event for when the mouse is over the area of the button.
	 */
	bool PauseMenuState::onButtonMouseOver(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseOverSound();
		return true;
	}

	/**
	 * This function will handle when the resume button gets clicked.
	 */
	bool PauseMenuState::onResumeClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playBackSound();
		Game::getInstance()->switchState(Game::getInstance()->getPreviousState());
		return true;
	}

	/**
	 * This function will handle when the controls button gets clicked.
	 */
	bool PauseMenuState::onControlsClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseClick();
		win_menu->hide();
		win_controls->show();
		//displayEquipped();
		return true;
	}

	/**
	 * This function will handle when the exit button gets clicked.
	 */
	bool PauseMenuState::onExitClicked(const CEGUI::EventArgs& e) {
		Game::getInstance()->quit();
		return true;
	}

	/**
	 * This function will handle when the realloc button gets clicked.
	 */
	bool PauseMenuState::onReallocClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseClick();
		win_menu->hide();
		win_realloc->show();
		displayEquipped();
		return true;
	}

	/**
	 * These function will handle when the back button gets clicked.
	 */
	bool PauseMenuState::onBackClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playBackSound();
		win_menu->show();
		win_realloc->hide();
		return true;
	}
	bool PauseMenuState::onControlsBackClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playBackSound();
		win_menu->show();
		win_controls->hide();
		return true;
	}

	/**
	 * This function will handle when the Realloc #1 button gets clicked.
	 */
	bool PauseMenuState::onR1Clicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseClick();
		displayReallocState(Attributes::getInstance()->getR1());
		return true;
	}

	/**
	 * This function will handle when the Realloc #2 button gets clicked.
	 */
	bool PauseMenuState::onR2Clicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseClick();
		displayReallocState(Attributes::getInstance()->getR2());
		return true;
	}

	/**
	 * This function will handle when the Realloc #3 button gets clicked.
	 */
	bool PauseMenuState::onR3Clicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseClick();
		displayReallocState(Attributes::getInstance()->getR3());
		return true;
	}

	/**
	 * This function will handle when the Planet button gets clicked.
	 */
	bool PauseMenuState::onPlanetClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseClick();

		std::string tutorialMsg = "Planet Controls \n \n";
		tutorialMsg += "Movement \n";
		tutorialMsg += "W - Forward \n";
		tutorialMsg += "S - Back \n";
		tutorialMsg += "A - Strafe Left \n";
		tutorialMsg += "D - Strafe Right \n \n";
		tutorialMsg += "Actions \n";
		tutorialMsg += "Spacebar - Jump \n";
		tutorialMsg += "E - Interact \n";
		tutorialMsg += "H - Toggle Help \n";
		lbl_controlsList->setText(tutorialMsg);

		return true;
	}

	/**
	 * This function will handle when the Space button gets clicked.
	 */
	bool PauseMenuState::onSpaceClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseClick();

		std::string tutorialMsg = "Space Controls\n \n";
		tutorialMsg += "Movement \n";
		tutorialMsg += "W - Forward \n";
		tutorialMsg += "S - Back \n";
		tutorialMsg += "A - Strafe Left \n";
		tutorialMsg += "D - Strafe Right \n";
		tutorialMsg += "Right Ctrl - Elevate Down \n";
		tutorialMsg += "Spacebar - Elevate Up \n \n";
		tutorialMsg += "Actions \n";
		tutorialMsg += "1 - Realloc State 1 \n";
		tutorialMsg += "2 - Realloc State 2 \n";
		tutorialMsg += "3 - Realloc State 3";

		lbl_controlsList->setText(tutorialMsg);


		return true;
	}

	/**
	 * This function will handle when the Room button gets clicked.
	 */
	bool PauseMenuState::onRoomClicked(const CEGUI::EventArgs& e) {
		MusicManager::getInstance()->playMouseClick();

		std::string tutorialMsg = "Armory/Factory Controls \n \n";
		tutorialMsg += "Movement \n";
		tutorialMsg += "W - Forward \n";
		tutorialMsg += "S - Back \n";
		tutorialMsg += "A - Rotate Left \n";
		tutorialMsg += "D - Rotate Right \n \n";
		tutorialMsg += "Actions \n";
		tutorialMsg += "E - Leave \n";
		tutorialMsg += "H - Toggle Help \n";
		tutorialMsg += "I - Equip Part (Armory) \n";
		tutorialMsg += "M - Make Part (Factory) \n";
		lbl_controlsList->setText(tutorialMsg);

		return true;
	}


// =====================================================================================
//
// #GUI Setters.
//
// =====================================================================================

	/**
	 * This function gets called when the enter() function gets called.
	 */
	void PauseMenuState::displayResources() {

		// Display side information.
		lbl_wave->setText("Current Wave: "+ intToString(SpaceGameplayState::getInstance()->getCurrentWave()));

		// Player resources.
		int metal = Attributes::getInstance()->getMetal();
		int darkMatter = Attributes::getInstance()->getDarkMatter();
		int carbon = Attributes::getInstance()->getCarbon();

		// Print out all resources in the static text label.
		std::string text = "Dark Matter: "+ intToString(darkMatter) + " \n \n";
		text += "Metal Scrap: "+ intToString(metal) + " \n \n";
		text += "Carbon: "+ intToString(carbon) + " \n \n";

		// Set text.
		lbl_resources->setText(text);

	}

	/*
	 * Displays all the parts in the first allocation.
	 *
	 * @param state				pointer to which realloc state to display
	 *
	 */
	void PauseMenuState::displayReallocState(ReallocState* state) {

		// Display head part information.
		std::string h_desc = "Head \n";
		h_desc += state->getHeadPart()->getName() +" \n";
		h_desc += "Damage: "+ doubleToString(state->getHeadPart()->getStatIncrease()) +" \n";
		lbl_headSlot->setText(h_desc);

		// Display left shoulder part information.
		std::string ls_desc = "Left Shoulder \n";
		ls_desc += state->getLeftShoulderPart()->getName() +" \n";
		ls_desc += "Damage: "+ doubleToString(state->getLeftShoulderPart()->getStatIncrease()) +" \n";
		lbl_leftShoulderSlot->setText(ls_desc);

		// Display right shoulder part information.
		std::string rs_desc = "Right Shoulder \n";
		rs_desc += state->getRightShoulderPart()->getName() +" \n";
		rs_desc += "Damage: "+ doubleToString(state->getRightShoulderPart()->getStatIncrease()) +" \n";
		lbl_rightShoulderSlot->setText(rs_desc);

		// Display left arm part information.
		std::string la_desc = "Left Arm \n";
		la_desc += state->getLeftArmPart()->getName() +" \n";
		la_desc += "Damage: "+ doubleToString(state->getLeftArmPart()->getStatIncrease()) +" \n";
		lbl_leftArmSlot->setText(la_desc);

		// Display right arm part information.
		std::string ra_desc = "Right Arm \n";
		ra_desc += state->getRightArmPart()->getName() +" \n";
		ra_desc += "Damage: "+ doubleToString(state->getRightArmPart()->getStatIncrease()) +" \n";
		lbl_rightArmSlot->setText(ra_desc);

	}

	/*
	 * This function initially displays the alloc that is currently equipped.
	 */
	void PauseMenuState::displayEquipped() {
		if(Attributes::getInstance()->getCurrentReallocState() == Attributes::getInstance()->getR1()) {
			displayReallocState(Attributes::getInstance()->getR1());
		}

		else if(Attributes::getInstance()->getCurrentReallocState() == Attributes::getInstance()->getR2()) {
			displayReallocState(Attributes::getInstance()->getR2());
		}

		else if(Attributes::getInstance()->getCurrentReallocState() == Attributes::getInstance()->getR3()) {
			displayReallocState(Attributes::getInstance()->getR3());
		}
	}

}
