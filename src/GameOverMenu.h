#ifndef capstone_GAMEOVERSTATE_H_
#define capstone_GAMEOVERSTATE_H_

#include "MenuState.h"

namespace capstone {

	/**
	 * This class provides the functionally for the the player dies.
	 */
	class GameOverState : public MenuState {

        // Scene properties.
        Ogre::SceneManager*				mSceneMgr;
        Ogre::Camera*					mCamera;

        // GUI variables.
        OgreBites::SdkTrayManager*		mTrayMgr;
        OgreBites::Button*				mPlayAgainButton;
		OgreBites::Button*				mExitButton;

		// Private constructor and destructor (Singleton design pattern).
		GameOverState();
		~GameOverState();

        // Noncopyable constructor and assignment operator.
        GameOverState(const GameOverState&);
        GameOverState& operator= (const GameOverState&);

	public:

		static GameOverState* getInstance();

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
