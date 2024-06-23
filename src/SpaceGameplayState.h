#ifndef capstone_SPACEGAMEPLAYSTATE_H_
#define capstone_SPACEGAMEPLAYSTATE_H_

#include "GameplayState.h"

namespace capstone {

	// Foward declarations.
	class SpaceLevel;

	/**
	 * This class provides the functionally for the gameplay in space. 
	 */
	class SpaceGameplayState : public GameplayState {

		// The current wave number.
		int								currentWave;

        // Scene properties.
		SpaceLevel*						level;
        Ogre::SceneManager*				mSceneMgr;

        // GUI variables.
        OgreBites::SdkTrayManager*		mTrayMgr;
		OgreBites::Label*				mHealthLabel;
        Ogre::Overlay*					mCrosshairOverlay;
		
		CEGUI::Window*					win_intermission;
		CEGUI::Window*					lbl_summary;
		CEGUI::Window*					btn_end;
		CEGUI::ProgressBar*				healthBar;

		// Private constructor and destructor (Singleton design pattern).
		SpaceGameplayState();
		~SpaceGameplayState();

        // Noncopyable constructor and assignment operator.
        SpaceGameplayState(const SpaceGameplayState&);
        SpaceGameplayState& operator= (const SpaceGameplayState&);

	public:

		static SpaceGameplayState* getInstance();

        void initialize();
        void finalize();

        void enter();
        void leave();

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

		// Getters.
		int getCurrentWave()					{ return currentWave; }
		std::string getMessage()				{ return "Current Wave: "+ currentWave; }
		CEGUI::ProgressBar* getHealthBar()		{ return healthBar; }

		// Setters.
		void resetGame()						{ currentWave = 1; }

	private:

		void createLevel();
		void destroyLevel();
        void initializeGui();
        void shutdownGui();

		// Event callbacks.
        bool keyPressed(const OIS::KeyEvent& arg);
        bool keyReleased(const OIS::KeyEvent& arg);
        bool mouseMoved(const OIS::MouseEvent& arg);
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
		bool onEndClicked(const CEGUI::EventArgs& e);
		bool onButtonMouseOver(const CEGUI::EventArgs& e);

	};

}

#endif