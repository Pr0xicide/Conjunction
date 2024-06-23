#ifndef capstone_BaseController_H_
#define capstone_BaseController_H_

namespace capstone {

	class Level;

	/**
	 * This root controller for all controllers for this level editor.
	 */
	class BaseController : public OIS::KeyListener
						 , public OIS::MouseListener {

	protected:

		Level*					mLevel;
		Ogre::Camera*			mCamera;
		Ogre::SceneManager*		mSceneMgr;
		OIS::Keyboard*			mKeyboard;

	public:

		// Constructor and destructor.
		BaseController(Level* l, Ogre::Camera* c, Ogre::SceneManager* sm);
		virtual ~BaseController();

		virtual void update(Ogre::Real deltaT) = 0;

		// Getters.
		Ogre::SceneManager* getSceneMgr() { return mSceneMgr; }

        // OIS::KeyListener callbacks.
        virtual bool keyPressed(const OIS::KeyEvent& arg) = 0;
        virtual bool keyReleased(const OIS::KeyEvent& arg) = 0;

        // OIS::MouseListener callbacks.
        virtual bool mouseMoved(const OIS::MouseEvent& arg) = 0;
        virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) = 0;
        virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) = 0;

	};

}

#endif