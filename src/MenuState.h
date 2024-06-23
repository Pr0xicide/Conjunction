#ifndef capstone_MENUSTATE_H_
#define capstone_MENUSTATE_H_

namespace capstone {
	
	/**
	 * This class is the sub-parent class for the menus.
	 */
	class MenuState : public GameState {

	protected:
		// Constructor and destructor.
        MenuState();
        virtual ~MenuState();

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
        // Noncopyable constructor and assignment operator.
        MenuState(const MenuState&);
        MenuState& operator= (const MenuState&);

	};

}

#endif