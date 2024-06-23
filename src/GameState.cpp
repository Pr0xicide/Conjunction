#include "stdafx.h"

#include "GameState.h"
#include "MusicManager.h"

namespace capstone {

	/**
	 * GameState constructor.
	 */
    GameState::GameState() {

    }

	/**
	 * GameState destructor.
	 */
    GameState::~GameState()  {
    
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the GameState.
	 */
    void GameState::initialize() {
    
	}

	/**
	 * This function will destroy the GameState.
	 */
    void GameState::finalize() {

    }


// =====================================================================================
//
// #Switching
//
// =====================================================================================

	/**
	 * This function will enter the game state.
	 */
    void GameState::enter() {

    }

	/**
	 * This function will leave the game state.
	 */
    void GameState::leave() {

    }

	/**
	 * This function will switch the visiblity of the tutorial labels.
	 */
	void GameState::setTutorialVisibility() {

		if (lbl_help->isVisible() && lbl_helpDesc->isVisible()) 
			hideTutorial();

		else {
			MusicManager::getInstance()->playToggleSound();
			showTutorial();
		}

	}


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will render every frame for the game state.
	 * 
	 * @param evt			frame Event that signals when a new frame is rendered
	 *
	 */
    bool GameState::frameRenderingQueued(const Ogre::FrameEvent& evt) {
        return true;
    }


// =====================================================================================
//
// #Events Listeners
//
// =====================================================================================

	/**
	 * This function will handle the event when a keyboard key is struck.
	 */
    bool GameState::keyPressed(const OIS::KeyEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when a keyboard key is released.
	 */
    bool GameState::keyReleased(const OIS::KeyEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool GameState::mouseMoved(const OIS::MouseEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool GameState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool GameState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        return true;
    }

}
