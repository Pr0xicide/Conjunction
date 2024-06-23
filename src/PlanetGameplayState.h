#ifndef capstone_PLANETGAMEPLAYSTATE_H_
#define capstone_PLANETGAMEPLAYSTATE_H_

#include "GameplayState.h"

namespace capstone {

	// Foward declarations.
	class PlanetLevel;

	/**
	 * This class provides the functionally for the gameplay on planets. 
	 */
	class PlanetGameplayState : public GameplayState {

        // Scene properties.
		bool							playSound;

		PlanetLevel*					level;
        Ogre::SceneManager*				mSceneMgr;

        // GUI variables.
        OgreBites::SdkTrayManager*		mTrayMgr;
		OgreBites::Label*				mEnterLabel;
		CEGUI::Window*					win_planet;
		CEGUI::Window*					lbl_enter;

		// Private constructor and destructor (Singleton design pattern).
		PlanetGameplayState();
		~PlanetGameplayState();

        // Noncopyable constructor and assignment operator.
        PlanetGameplayState(const PlanetGameplayState&);
        PlanetGameplayState& operator= (const PlanetGameplayState&);

	public:

		static PlanetGameplayState* getInstance();

        void initialize();
        void finalize();

        void enter();
        void leave();

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

		// Getters.
		bool isDialogBoxOpen()			{ return mTrayMgr->isDialogVisible(); }

	private:

		void createLevel();
        void initializeGui();
		void updateHud();
		void destroyLevel();
		void shutdownGui();

		// Keyboard & Mouse callbacks.
        bool keyPressed(const OIS::KeyEvent& arg);
        bool keyReleased(const OIS::KeyEvent& arg);
        bool mouseMoved(const OIS::MouseEvent& arg);
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
		void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

	};

}

#endif