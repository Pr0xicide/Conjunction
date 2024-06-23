#ifndef capstone_CREDITSSTATE_H_
#define capstone_CREDITSSTATE_H_

#include "MenuState.h"

namespace capstone {

	/**
	 * This class provides the functionally for the credit list.
	 */
	class CreditsState : public MenuState {

		static Ogre::Real				speed;
		std::string						msg;

        // Scene properties.
        Ogre::SceneManager*				mSceneMgr;
        Ogre::Camera*					mCamera;

        // GUI variables.
		Ogre::Real						scrollSpeed, percent;
        OgreBites::SdkTrayManager*		mTrayMgr;
		OgreBites::TextBox*				mTextbox;

		// Private constructor and destructor (Singleton design pattern).
		CreditsState();
		~CreditsState();

        // Noncopyable constructor and assignment operator.
        CreditsState(const CreditsState&);
        CreditsState& operator= (const CreditsState&);

	public:

		static CreditsState* getInstance();

        void initialize();
        void finalize();

        void enter();
        void leave();

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	private:

		void setupGUI();

		// Event callbacks.
        bool keyPressed(const OIS::KeyEvent& arg);
		bool mouseMoved(const OIS::MouseEvent& arg);

	};

}

#endif