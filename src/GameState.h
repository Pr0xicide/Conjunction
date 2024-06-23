#ifndef capstone_GAMESTATE_H_
#define capstone_GAMESTATE_H_

namespace capstone {

	/**
	 * This class is the parent class of all game states.
	 */
	class GameState : public OgreBites::SdkTrayListener {

	protected:

		CEGUI::Window*			stateWindow;
		CEGUI::Window*			lbl_help;
		CEGUI::Window*			lbl_helpDesc;

		// Constructor and destructor.
        GameState();
        virtual ~GameState();

		void setTutorialVisibility();
		void hideTutorial()		{ lbl_help->hide(); lbl_helpDesc->hide(); }
		void showTutorial()		{ lbl_help->show(); lbl_helpDesc->show(); }

	public:

        virtual void initialize();
        virtual void finalize();

        virtual void enter();
        virtual void leave();

        virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

        virtual bool keyPressed(const OIS::KeyEvent& arg);
        virtual bool keyReleased(const OIS::KeyEvent& arg);

        virtual bool mouseMoved(const OIS::MouseEvent& arg);
        virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	private:
        // Noncopyable.
        GameState(const GameState&);
        GameState& operator= (const GameState&);

	};

}

#endif