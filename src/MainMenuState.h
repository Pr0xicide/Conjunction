#ifndef capstone_MAINMENUSTATE_H_
#define capstone_MAINMENUSTATE_H_

#include "MenuState.h"

namespace capstone {

	/**
	 * This class provides the functionally for the main menu when the game begins.
	 */
	class MainMenuState : public MenuState {

        // Scene properties.
        Ogre::SceneManager*				mSceneMgr;
        Ogre::Camera*					mCamera;

        // GUI variables.
        OgreBites::SdkTrayManager*		mTrayMgr;
        OgreBites::Button*				mStartButton;
		OgreBites::Button*				mCreditsButton;
        OgreBites::Button*				mQuitButton;
		OgreBites::TextBox*				mTextbox;

		Ogre::Degree					mSpeed;

		// Private constructor and destructor (Singleton design pattern).
		MainMenuState();
		~MainMenuState();

        // Noncopyable constructor and assignment operator.
        MainMenuState(const MainMenuState&);
        MainMenuState& operator= (const MainMenuState&);

	public:

		static MainMenuState* getInstance();

        void initialize();
        void finalize();

        void enter();
        void leave();

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	private:

		// Event callbacks.
        bool mouseMoved(const OIS::MouseEvent& arg);
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        void buttonHit(OgreBites::Button* button);
		
	};

}

#endif