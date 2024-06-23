#include "stdafx.h"

#include "GameState.h"
#include "GameplayState.h"
#include "Game.h"

namespace capstone {

	/**
	 * GameplayState constructor.
	 */
    GameplayState::GameplayState() 
		: GameState()
	{

    }

	/**
	 * GameplayState destructor.
	 */
    GameplayState::~GameplayState()  {
    
	}

// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the GameplayState.
	 */
    void GameplayState::initialize() {
		createLevel();
	}

	/**
	 * This function will destroy the GameplayState.
	 */
    void GameplayState::finalize() {

    }

// =====================================================================================
//
// #Switching
//
// =====================================================================================

	/**
	 * This function will enter the gameplay state.
	 */
    void GameplayState::enter() {

    }

	/**
	 * This function will leave the gameplay state.
	 */
    void GameplayState::leave() {

    }

// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will render every frame for the gameplay state.
	 * 
	 * @param evt			Frame Event that signals when a new frame is rendered
	 *
	 */
    bool GameplayState::frameRenderingQueued(const Ogre::FrameEvent& evt) {
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
    bool GameplayState::keyPressed(const OIS::KeyEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when a keyboard key is released.
	 */
    bool GameplayState::keyReleased(const OIS::KeyEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool GameplayState::mouseMoved(const OIS::MouseEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool GameplayState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool GameplayState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        return true;
    }

}
