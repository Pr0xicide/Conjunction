#include "stdafx.h"

#include "gamestates.h"
#include "Rooms.h"
#include "Game.h"
#include "SpacePlayer.h"
#include "Part.h"
#include "GlobalVariables.h"
#include "Attributes.h"
#include "MusicManager.h"
#include "Skill.h"

namespace capstone{

	/**
	 * RoomGameplayState constructor.
	 */
	RoomGameplayState::RoomGameplayState() 
	    : GameplayState()
		, mSceneMgr(NULL)
        , mTrayMgr(NULL)
		, room(NULL)
		, equipRoom(NULL)
		, playSound(false)
	{
		srand(1);
	}

	/**
	 * RoomGameplayState destructor.
	 */
	RoomGameplayState::~RoomGameplayState() {

	}

	/**
	 * This function will create an instance of the room's gameplay state using the singleton design pattern.
	 * 
	 * @return a pointer to this RoomGameplayState class.
	 *
	 */
	RoomGameplayState* RoomGameplayState::getInstance() {
		static RoomGameplayState rGameplay;
		return &rGameplay;
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the room's gameplay state.
	 */
	void RoomGameplayState::initialize() {
		createLevel();
		initializeGui();
	}

	/**
	 * This function will create the level.
	 */
	void RoomGameplayState::createLevel() {
		// New level object.
		equipRoom = new EquipRoom();
		factoryRoom = new FactoryRoom();
	}

	/**
	 * This function will initialize the room's gameplay state.
	 */
	void RoomGameplayState::initializeGui() {

		// Instantiate the tray manager.
        mTrayMgr = new OgreBites::SdkTrayManager("RoomGameplayGui",
                                                 Game::getInstance()->getWindow(),
                                                 Game::getInstance()->getMouse(),
                                                 this);

        mTrayMgr->toggleAdvancedFrameStats();
        mTrayMgr->hideCursor();
		
		// Get the width and height of the game application window.
		int appWidth = Game::getInstance()->getViewport()->getActualWidth();
		int appHeight = Game::getInstance()->getViewport()->getActualHeight();

		// Create the textbox.
		tbx_partStats =  mTrayMgr->createTextBox(OgreBites::TL_TOPLEFT, "Stats", "Part Stats", appWidth * 0.25, appHeight * 0.25); 
		tbx_resources = mTrayMgr->createTextBox(OgreBites::TL_TOPRIGHT, "Resources", "Resources", appWidth * 0.25, appHeight * 0.25);

        // Don't show anything yet.
        mTrayMgr->hideAll();

		// Initialize CEGUI windows.
		stateWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "room_MasterRoot");
		win_roomHUD = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "RoomHUD");
		stateWindow->addChildWindow(win_roomHUD);

		// Create labels.
		lbl_matter = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_matter");
		lbl_matter->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.9f,0)));;
		lbl_matter->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0, 25)));
		lbl_matter->setAlpha(0.75f);
		win_roomHUD->addChildWindow(lbl_matter);

		lbl_metal = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_metal");
		lbl_metal->setPosition(CEGUI::UVector2(CEGUI::UDim(0.33f, 0), CEGUI::UDim(0.9f,0)));;
		lbl_metal->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0, 25)));
		lbl_metal->setAlpha(0.75f);
		win_roomHUD->addChildWindow(lbl_metal);

		lbl_carbon = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_carbon");
		lbl_carbon->setPosition(CEGUI::UVector2(CEGUI::UDim(0.63f, 0), CEGUI::UDim(0.9f,0)));;
		lbl_carbon->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0, 25)));
		lbl_carbon->setAlpha(0.75f);
		win_roomHUD->addChildWindow(lbl_carbon);

		lbl_partName = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_partName");
		lbl_partName->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.55f,0)));;
		lbl_partName->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.06f, 0)));
		lbl_partName->setText("PART NAME!");
		lbl_partName->setAlpha(0.85f);
		win_roomHUD->addChildWindow(lbl_partName);

		lbl_partDetails = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_partDetails");
		lbl_partDetails->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.63f,0)));;
		lbl_partDetails->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.15f, 0)));
		lbl_partDetails->setText("");
		lbl_partDetails->setProperty("VertFormatting", "TopAligned");
		lbl_partDetails->setAlpha(0.85f);
		win_roomHUD->addChildWindow(lbl_partDetails);

		lbl_partRequirements = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "lbl_partRequirements");
		lbl_partRequirements->setPosition(CEGUI::UVector2(CEGUI::UDim(0.29f, 0), CEGUI::UDim(0.63f,0)));;
		lbl_partRequirements->setSize(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.15f, 0)));
		lbl_partRequirements->setText("");
		lbl_partRequirements->setProperty("VertFormatting", "TopAligned");
		lbl_partRequirements->setAlpha(0.85f);
		win_roomHUD->addChildWindow(lbl_partRequirements);

		// Initialize labels.
		lbl_help = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "r_lbl_help");
		lbl_help->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.03f,0)));;
		lbl_help->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.06f, 0)));
		lbl_help->setText("Room Tutorial");
		lbl_help->setAlpha(0.85f);
		win_roomHUD->addChildWindow(lbl_help);

		lbl_helpDesc = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "r_lbl_desc");
		lbl_helpDesc->setPosition(CEGUI::UVector2(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.1f,0)));;
		lbl_helpDesc->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.30f, 0)));
		lbl_helpDesc->setText("Movement ");
		lbl_helpDesc->setProperty("VertFormatting", "TopAligned");
		lbl_helpDesc->setAlpha(0.85f);
		win_roomHUD->addChildWindow(lbl_helpDesc);

		std::string tutorialMsg = "Movement \n";
		tutorialMsg += "W - Forward \n";
		tutorialMsg += "S - Back \n";
		tutorialMsg += "A - Rotate Left \n";
		tutorialMsg += "D - Rotate Right \n \n";
		tutorialMsg += "Actions \n";
		tutorialMsg += "E - Leave \n";
		tutorialMsg += "H - Toggle Help \n";
		tutorialMsg += "I - Equip Part (Armory) \n";
		tutorialMsg += "M - Make Part (Factory) \n";

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
	void RoomGameplayState::destroyLevel() {
		delete equipRoom;	
		delete factoryRoom;
		room = NULL;
		equipRoom = NULL;
		factoryRoom = NULL;
	}

	/**
	 * This function will destroy the room's gameplay state.
	 */
	void RoomGameplayState::finalize() {
		shutdownGui();
		destroyLevel();
	}

	/**
	 * This function will destroy the GUI used for this game state.
	 */
	void RoomGameplayState::shutdownGui() {
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
	 * This function will enter the room's gameplay state.
	 */
    void RoomGameplayState::enter() {

		// Show HUD.
		mTrayMgr->hideAll();
		CEGUI::System::getSingleton().setGUISheet(stateWindow);
		CEGUI::MouseCursor::getSingleton().hide();
		stateWindow->show();
		hideTutorial();
		hidePartLabels();
		lbl_partRequirements->hide();

		switch (id) {

			// We are entering the equipment room.
			case 1:
				if(!equipRoom->getPlayer()->isPausing()){
					equipRoom->deleteAllParts();
					equipRoom->placeParts(equipRoom->getSceneManager());
				}
				room = equipRoom;
				lbl_matter->hide();
				lbl_metal->hide();
				lbl_carbon->hide();
				break;

			// We are entering the factory room.
			case 2:
				room = factoryRoom;
				lbl_matter->show();
				lbl_metal->show();
				lbl_carbon->show();
				break;

			default:
				std::cout << "There's a problem..." << std::endl;
				break;
		}

		room->getPlayer()->setPausing(false);

		Game::getInstance()->getViewport()->setCamera(room->getCamera());
		room->getPlayer()->setCurrentReallocState();	// Makes sure reallocStates carry over.
		MusicManager::getInstance()->playRoomMusic();

    }

	/**
	 * This function will leave the space's gameplay state.
	 */
    void RoomGameplayState::leave() {

		// Hide HUD.
		mTrayMgr->hideTrays();
		hideTutorial();
		stateWindow->hide();
        Game::getInstance()->getViewport()->setCamera(NULL);

		// Free up all entities
		std::vector<Part*> p = room->getParts();
		for(unsigned i=0;i<p.size();i++){
			p[i]->getSceneNode()->detachAllObjects();
		}
		room->getPlayer()->getHeadNode()->detachAllObjects();
		room->getPlayer()->getLeftArmNode()->detachAllObjects();
		room->getPlayer()->getRightArmNode()->detachAllObjects();
		room->getPlayer()->getLeftShoulderNode()->detachAllObjects();
		room->getPlayer()->getRightShoulderNode()->detachAllObjects();

		// Stop background music.
		MusicManager::getInstance()->stopRoomMusic();

		// Set the previous state so that the pause menu knows what state to go back to.
		Game::getInstance()->setPreviousState(this);
    }


// =====================================================================================
//
// #Update
//
// =====================================================================================

	/**
	 * This function will render every frame for the space's gameplay state.
	 * 
	 * @param evt			frame event that signals when a new frame is rendered
	 *
	 */
    bool RoomGameplayState::frameRenderingQueued(const Ogre::FrameEvent& evt) {

		// Update the level. which updates everything else.
		room->update(evt.timeSinceLastFrame);

		// Update HUD.
		updateHud();
		mTrayMgr->frameRenderingQueued(evt);
		CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

		// Update FMOD
		MusicManager::getInstance()->update(evt.timeSinceLastFrame);

        return true;
    }

	/**
	 * This function will update the GUI/HUD.
	 */
	void RoomGameplayState::updateHud() {

		// Player resources
		int metal = Attributes::getInstance()->getMetal();
		int darkMatter = Attributes::getInstance()->getDarkMatter();
		int carbon = Attributes::getInstance()->getCarbon();

		// Display the player's resources to the labels.
		lbl_matter->setText("Dark Matter: "+ intToString(darkMatter));
		lbl_metal->setText("Metal Scrap: "+ intToString(metal));
		lbl_carbon->setText("Carbon: "+ intToString(carbon));

		// Hide labels.
		hidePartLabels();
		lbl_partRequirements->hide();
		
		// Grab the closest part the the player.
		Part* part = room->getClosestPart();

		// If the player is close to the nearest part to the part.
		if (part->getPlayerClose()) {

			// Play sound once, and not repeatly.
			if (playSound) {
				MusicManager::getInstance()->playInteractBuildingSound();
				playSound = false;
			}

			showPartLabels();

			// Get variables to display the part information.
			std::string name = part->getName();
			int stat = part->getStatIncrease();
			std::string type = determinePartType(part->getPartType());
			std::string skill = part->getSkill()->getName();
			int reqMetal = part->getMetal();
			int reqDarkMatter = part->getDarkMatter();
			int reqCarbon = part->getCarbon();

			// String to hold all the part's information.
			Ogre::String message = type +"\n";
			message += "Damage: "+ Ogre::StringConverter::toString(stat) +"\n";

			// If this part is a blueprint, display the resources necessary to create this part.
			if (id == 2) {
				lbl_partRequirements->show();
				std::string requirementMsg = "Requires "+ Ogre::StringConverter::toString(reqDarkMatter)+ " dark matter \n";
				requirementMsg += "Requires "+ Ogre::StringConverter::toString(reqMetal)+ " metal scrap \n";
				requirementMsg += "Requires "+ Ogre::StringConverter::toString(reqCarbon)+ " carbon \n";
				lbl_partRequirements->setText(requirementMsg);
			}

			// Set the textbox's message.
			lbl_partName->setText(part->getName());
			tbx_partStats->setText(message);
			lbl_partDetails->setText(message);

		}

		else {
			playSound = true;
		}

	}

	/**
	 * This function will determine the part's type.
	 *
	 * @param type			number representing the ID of the part type
	 *
	 * @return a string that describe the part type
	 *
	 */
	std::string RoomGameplayState::determinePartType(int type) {

		// Determine part type.
		switch(type) {

			case HEAD_PART:
				return "Head Part";
				break;

			case LEFT_ARM_PART:
				return "Left Arm Part";
				break;

			case RIGHT_ARM_PART:
				return "Right Arm Part";
				break;

			case LEFT_SHOULDER_PART:
				return "Right Shoulder Part";
				break;

			case RIGHT_SHOULDER_PART:
				return "Left Shoulder Part";
				break;
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
	bool RoomGameplayState::keyPressed(const OIS::KeyEvent& arg) {

		// Show tutorial menu.
		if (arg.key == OIS::KC_H)
			setTutorialVisibility();

		room->getKeyListener()->keyPressed(arg);

		return true;
	}

	/**
	 * This function will handle the event when a keyboard key is released.
	 */
	bool RoomGameplayState::keyReleased(const OIS::KeyEvent& arg) {

		room->getKeyListener()->keyReleased(arg);

		return true; 
	}

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool RoomGameplayState::mouseMoved(const OIS::MouseEvent& arg) {

		room->getMouseListener()->mouseMoved(arg);

        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool RoomGameplayState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {

		room->getMouseListener()->mousePressed(arg, id);

        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool RoomGameplayState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {

		room->getMouseListener()->mouseReleased(arg, id);

        return true;
    }

}
