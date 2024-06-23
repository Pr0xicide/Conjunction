#ifndef capstone_GameObject_H_
#define capstone_GameObject_H_

namespace capstone {

	// Forward declaration.
	class EditorController;

	class GameObject {

		// Class variables.
		EditorController*		mController;

		bool					isDisposable;
		std::string				mMeshName;
		std::string				mMaterialName;

		Ogre::Entity*			mEntity;
		Ogre::SceneNode*		mRootNode;
		Ogre::SceneNode*		mObjectNode;
		Ogre::SceneNode*		mSceneNode;
		Ogre::SceneManager*		mSceneMgr;

	public:

		GameObject(EditorController* c, std::string mMeshName, std::string mMaterial, Ogre::SceneNode* s, Ogre::SceneManager* m);
		GameObject(EditorController* c, std::string mMeshName, std::string mMaterial, float scale, Ogre::Degree p, Ogre::Degree y, Ogre::Degree r, Ogre::SceneManager* m);
		virtual ~GameObject();

		virtual void update(Ogre::Real deltaT);

		// Getters.
		bool isObjectDisposable()					{ return isDisposable; }

		virtual const char* getMeshName()			{ return mMeshName.c_str(); }
		virtual const char* getMaterialName()		{ return mMaterialName.c_str(); }

		virtual float getObjectScale()				{ return mObjectNode->getScale().x; }
		virtual float getXRotation()				{ return mRootNode->_getDerivedOrientation().getPitch().valueDegrees(); }
		virtual float getYRotation()				{ return mRootNode->_getDerivedOrientation().getYaw().valueDegrees(); }
		virtual float getZRotation()				{ return mRootNode->_getDerivedOrientation().getRoll().valueDegrees(); }

		// Setters.
		void setDisposableFlag()					{ isDisposable = true; }

	private:

		void setupObject();
		void loadObject(float scale, Ogre::Degree p, Ogre::Degree y, Ogre::Degree r);

	};
	
}

#endif