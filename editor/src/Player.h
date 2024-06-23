#ifndef capstone_PLAYER_H_
#define capstone_PLAYER_H_

namespace capstone {

	/**
	 * Parent class for the player since the robot has a space and planet form.
	 */ 
	class Player : public OIS::KeyListener
				 , public OIS::MouseListener {

	protected:

	public:

		// Constructor and destructor.
		Player();
		~Player();

		// Abstract methods/functions.
		virtual void setupBody(Ogre::SceneManager* sceneMgr) = 0;
		virtual void setupCamera(Ogre::Camera* camera) = 0;
		virtual void setupAnimations() = 0;
		virtual void update(Ogre::Real deltaT) = 0;

	private:

        // Noncopyable constructor and assignment operator.
        Player(const Player&);
        Player& operator= (const Player&);
		
        // OIS::KeyListener callbacks
        virtual bool keyPressed(const OIS::KeyEvent& arg) = 0;
        virtual bool keyReleased(const OIS::KeyEvent& arg) = 0;

        // OIS::MouseListener callbacks
        virtual bool mouseMoved(const OIS::MouseEvent& arg) = 0;
        virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) = 0;
        virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) = 0;

	};

}

#endif