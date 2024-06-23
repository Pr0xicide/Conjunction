#ifndef capstone_ROOMGAMEPLAYSTATE_H_
#define capstone_ROOMGAMEPLAYSTATE_H_

#include "GameplayState.h"

namespace capstone{

	// Forward declarations.
	class Room;
	class EquipRoom;
	class FactoryRoom;

	/**
	 * This class provides the functionally for the gameplay in the room. 
	 */
	class RoomGameplayState : public GameplayState {

        // Scene properties.
		bool							playSound;
		int								id;
		Room*							room;
		EquipRoom*						equipRoom;
		FactoryRoom*					factoryRoom;
        Ogre::SceneManager*				mSceneMgr;

        // GUI variables.
        OgreBites::SdkTrayManager*		mTrayMgr;
		OgreBites::TextBox*				tbx_partStats;
		OgreBites::TextBox*				tbx_resources;
		CEGUI::Window*					win_roomHUD;
		CEGUI::Window*					lbl_matter;
		CEGUI::Window*					lbl_metal;
		CEGUI::Window*					lbl_carbon;
		CEGUI::Window*					lbl_partName;
		CEGUI::Window*					lbl_partDetails;
		CEGUI::Window*					lbl_partRequirements;

		// Private constructor and destructor (Singleton design pattern).
		RoomGameplayState();
		~RoomGameplayState();

        // Noncopyable constructor and assignment operator.
        RoomGameplayState(const RoomGameplayState&);
        RoomGameplayState& operator= (const RoomGameplayState&);

	public:

		static RoomGameplayState* getInstance();

        void initialize();
        void finalize();

        void enter();
        void leave();

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

		// Getters.
		int getRoomId()						{ return id; }

		// Setters.
		void setRoomId(int _id)				{ id = _id; }

	private:

		std::string determinePartType(int type);

		void createLevel();
		void destroyLevel();
        void initializeGui();
        void shutdownGui();
		void updateHud();

		void showPartLabels()				{ lbl_partName->show(); lbl_partDetails->show(); }
		void hidePartLabels()				{ lbl_partName->hide(); lbl_partDetails->hide(); }

		// Event callbacks.
        bool keyPressed(const OIS::KeyEvent& arg);
        bool keyReleased(const OIS::KeyEvent& arg);
        bool mouseMoved(const OIS::MouseEvent& arg);
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	};

}

#endif