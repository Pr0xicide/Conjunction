#ifndef capstone_PLANETPROP_H_
#define capstone_PLANETPROP_H_

namespace capstone {

	struct PlanetObjectStruct {
		std::string			meshName;
		std::string			materialName;
		Ogre::Real			scale;
		Ogre::Degree		pitch;
		Ogre::Degree		yaw;
		Ogre::Degree		roll;
	};

	// Class declarations.
	class PlanetLevel;

	/**
	 * Class definition for all the props placed on the planet.
	 */
	class PlanetProp {

		// Instance variables.
		PlanetLevel*				level;
		PlanetObjectStruct*			objStruct;

		Ogre::Entity*				mEntity;

		Ogre::SceneManager*			mSceneMgr;

		Ogre::SceneNode*			mRootNode;
		Ogre::SceneNode*			mSceneNode;

	public:

		// Constructor and destructor.
		PlanetProp(PlanetLevel* level, PlanetObjectStruct* objStruct);
		virtual ~PlanetProp();

	private:

		// Helper initialization methods.
		void setupObject();

	};

}

#endif