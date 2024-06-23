#include "stdafx.h"

#include "GameState.h"
#include "MenuState.h"
#include "Game.h"

namespace capstone {
	
	/**
	 * MenuState constructor.
	 */
    MenuState::MenuState() 
		: GameState()
	{

    }

	/**
	 * MenuState destructor.
	 */
    MenuState::~MenuState()  {
    
	}

// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the MenuState.
	 */
    void MenuState::initialize() {
    
	}

	/**
	 * This function will destroy the MenuState.
	 */
    void MenuState::finalize() {

    }

// =====================================================================================
//
// #Switching
//
// =====================================================================================

	/**
	 * This function will enter the game menu state.
	 */
    void MenuState::enter() {

    }

	/**
	 * This function will leave the game menu state.
	 */
    void MenuState::leave() {

    }

// =====================================================================================
//
// #Update
//
// =====================================================================================

	/**
	 * This function will render every frame for the game menu state.
	 * 
	 * @param evt - Frame Event that signals when a new frame is rendered
	 *
	 */
    bool MenuState::frameRenderingQueued(const Ogre::FrameEvent& evt) {
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
    bool MenuState::keyPressed(const OIS::KeyEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when a keyboard key is released.
	 */
    bool MenuState::keyReleased(const OIS::KeyEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse has been moved.
	 */
    bool MenuState::mouseMoved(const OIS::MouseEvent& arg) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been clicked.
	 */
    bool MenuState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        return true;
    }

	/**
	 * This function will handle the event when the mouse button has been released.
	 */
    bool MenuState::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
        return true;
    }

}
