#ifndef capstone_GAMEPLAYSTATE_H_
#define capstone_GAMEPLAYSTATE_H_

#include "GameState.h"

namespace capstone {
	
	/**
	 * This class is the sub parent class for the menus.
	 */
	class GameplayState : public GameState {

	protected:
		// Constructor and destructor.
        GameplayState();
        virtual ~GameplayState();

	public:

        virtual void initialize();
		virtual void createLevel() = 0;
		virtual void destroyLevel() = 0;
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
        GameplayState(const GameplayState&);
        GameplayState& operator= (const GameplayState&);

	};

}

#endif