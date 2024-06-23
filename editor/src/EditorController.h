#ifndef capstone_EditorController_H_
#define capstone_EditorController_H_

#include "BaseController.h"
#include "GameObject.h"

namespace capstone {

	/**
	 * EditorController class will provide the user to navigate the level creating objects.
	 */
	class EditorController : public BaseController {

		// Class variables.
		static bool							UAT;
		char*								fileName;
		char*								saveFileName;
		unsigned							listSize;
		unsigned							currentIndex;

		std::vector<std::string>			mMeshNames;
		std::vector<std::string>			mMaterials;
		std::vector<Ogre::Entity*>			mModelList;
		std::vector<Ogre::SceneNode*>		mNodes;
		std::vector<GameObject*>			mGameObjects;
		
		OIS::Keyboard*						mKeyboard;
		
		Ogre::SceneNode*					mParentNode;
		Ogre::SceneNode*					mSceneNode;
		Ogre::SceneNode*					mCameraPivotNode;
		Ogre::SceneNode*					mCameraNode;

		Ogre::Degree						walkSpeed;
		Ogre::Degree						yawSpeed;
		Ogre::Real							wSpeed;
		Ogre::Real							ySpeed;
		Ogre::Real							scaleAmount;
			
	public:

		// Constructor and destructor.
		EditorController(Level* l, Ogre::Camera* c, Ogre::SceneManager* sm);
		~EditorController();

		virtual void update(Ogre::Real deltaT);

		// Getters.
		Level* getLevel()								{ return mLevel; }
		Ogre::SceneNode* getRootSceneNode()				{ return mParentNode; }

		// OIS::KeyListener callbacks.
		bool keyPressed(const OIS::KeyEvent& arg);
		bool keyReleased(const OIS::KeyEvent& arg);

		// OIS::MouseListener callbacks.
		bool mouseMoved(const OIS::MouseEvent& arg);
		bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	private:
		
		void setupMeshes();
		void setupEditor();
		void load();
		void placeObject();

		void updateModel(Ogre::Real deltaT);
		void updateObjects(Ogre::Real deltaT);

		void saveSettings();
		void switchModel(bool cycleForward);
		void scaleModel(bool increase);

	};

}

#endif