#ifndef capstone_INTERACTIVEBUILDING_H_
#define capstone_INTERACTIVEBUILDING_H_

#include "Game.h"
#include "gamestates.h"

namespace capstone {

	// Forward declarations
	class PlanetLevel;

	/**
	 * This class provides the base functionally of the buildings that the player can enter into.
	 */
	class InteractiveBuilding {

	protected:

		// Class variables.
		static const char*			xmlFileName;

		bool						playerClose;
		int							id;
		Ogre::String				name;
		std::string					desc;

		PlanetLevel*				level;
		
		Ogre::Entity*				mEntity;
		Ogre::SceneNode*			mSceneNode;
		Ogre::Real					mRadius;

		void readXML();

	public:

		InteractiveBuilding(PlanetLevel* level);
		virtual ~InteractiveBuilding();

		// Events.
		virtual void interact() = 0;

		// Getters.
		bool					isPlayerClose()		{ return playerClose; }
		int						getID()				{ return id; }
		std::string				getDesc()			{ return desc; }
		Ogre::Real				getRadius()			{ return mRadius; }
		Ogre::String			getName()			{ return name; }
		virtual Ogre::String	getQuestion()		{ return "Enter "+ name +"?"; }
		Ogre::SceneNode*		getSceneNode()		{ return mSceneNode; }

		// Setters.
		void setPlayerClose(bool close)				{ playerClose = close; }

	private:
		
		virtual void initProperties() = 0;

	};

}

#endif