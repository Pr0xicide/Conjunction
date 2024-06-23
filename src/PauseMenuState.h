#ifndef capstone_PAUSEMENUSTATE_H_
#define capstone_PAUSEMENUSTATE_H_

#include "MenuState.h"
#include "ReallocState.h"

namespace capstone {

	class PauseMenuState : public MenuState {

		float						transparency;

        // Scene properties.
        Ogre::SceneManager*			mSceneMgr;
        Ogre::Camera*				mCamera;

        // GUI variables.
		CEGUI::Window*				win_menu;
		CEGUI::Window*				win_controls;
		CEGUI::Window*				win_realloc;

		CEGUI::Window*				btn_controls;
		CEGUI::Window*				btn_exit;
		CEGUI::Window*				btn_resume;
		CEGUI::Window*				btn_realloc;
		CEGUI::Window*				btn_back;
		CEGUI::Window*				btn_cback;
		CEGUI::Window*				btn_r1;
		CEGUI::Window*				btn_r2;
		CEGUI::Window*				btn_r3;
		CEGUI::Window*				btn_planet;
		CEGUI::Window*				btn_space;
		CEGUI::Window*				btn_room;

		CEGUI::Window*				lbl_wave;
		CEGUI::Window*				lbl_stats;
		CEGUI::Window*				lbl_resources;
		CEGUI::Window*				lbl_headSlot;
		CEGUI::Window*				lbl_leftShoulderSlot;
		CEGUI::Window*				lbl_rightShoulderSlot;
		CEGUI::Window*				lbl_leftArmSlot;
		CEGUI::Window*				lbl_rightArmSlot;
		CEGUI::Window*				lbl_controlsList;

		//// Used for determining which state we came from.
		int previousState;

		// Private constructor and destructor (Singleton design pattern).
		PauseMenuState();
		~PauseMenuState();

        // Noncopyable constructor and assignment operator.
        PauseMenuState(const PauseMenuState&);
        PauseMenuState& operator= (const PauseMenuState&);

	public:

		static PauseMenuState* getInstance();

        void initialize();
        void finalize();

        void enter();
        void leave();

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

		// Setters.
		void setPreviousState(int state) { previousState = state; }
		
	private:

		void displayResources();
		void displayReallocState(ReallocState* state);
		void displayEquipped();
		void initMenu();
		void initControlsMenu();
		void initReallocMenu();

		// Event callbacks.
        bool mouseMoved(const OIS::MouseEvent& arg);
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
		bool onResumeClicked(const CEGUI::EventArgs& e);
		bool onControlsClicked(const CEGUI::EventArgs& e);
		bool onExitClicked(const CEGUI::EventArgs& e);
		bool onReallocClicked(const CEGUI::EventArgs& e);
		bool onBackClicked(const CEGUI::EventArgs& e);
		bool onControlsBackClicked(const CEGUI::EventArgs& e);
		bool onR1Clicked(const CEGUI::EventArgs& e);
		bool onR2Clicked(const CEGUI::EventArgs& e);
		bool onR3Clicked(const CEGUI::EventArgs& e);
		bool onButtonMouseOver(const CEGUI::EventArgs& e);
		bool onPlanetClicked(const CEGUI::EventArgs& e);
		bool onSpaceClicked(const CEGUI::EventArgs& e);
		bool onRoomClicked(const CEGUI::EventArgs& e);

	};

}

#endif