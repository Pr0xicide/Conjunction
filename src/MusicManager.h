#ifndef capstone_MUSICMANAGER_H_
#define capstone_MUSICMANAGER_H_

namespace capstone{

	class MusicManager{

		// FMOD variables
		FMOD_RESULT result;
		FMOD::System* system;

		// Music
		FMOD::Sound* mainMenuMusic;
		FMOD::Sound* planetMusic;
		FMOD::Sound* spaceMusic;
		FMOD::Sound* roomMusic;
		FMOD::Sound* pauseMusic;

		// Sounds
		FMOD::Sound* equipSound;
		FMOD::Sound* projectileSound;
		FMOD::Sound* mouseClickSound;
		FMOD::Sound* jumpSound;
		FMOD::Sound* menuBackSound;
		FMOD::Sound* menuStartSound;
		FMOD::Sound* btnHoverSound;
		FMOD::Sound* btnToggleSound;
		FMOD::Sound* interactBuildingSound;
		FMOD::Sound* cursorMoveSound;
		FMOD::Sound* jetstreamSound;
		FMOD::Sound* projectileHitSound;
		FMOD::Sound* rocketSound;
		FMOD::Sound* buildSound;

		// Channels
		FMOD::Channel* mainMenuChannel;
		FMOD::Channel* planetChannel;
		FMOD::Channel* spaceChannel;
		FMOD::Channel* roomChannel;
		FMOD::Channel* pauseChannel;
		FMOD::Channel* rocketChannel;
		FMOD::Channel* projectileHitChannel;

		// Private constructor and destructor (Singleton design pattern).
		MusicManager();
		~MusicManager();

		// Noncopyable constructor and assignment operator.
        MusicManager(const MusicManager&);
        MusicManager& operator= (const MusicManager&);

		void initialize();
		void errorCheck(const std::string& errMsg);
		void createMusic();
		void createSounds();

		inline FMOD_VECTOR OgreToFmod(const Ogre::Vector3& in)
		{
			// FMOD uses +x = right, +Y = up, +z = forward
			FMOD_VECTOR out = { -in.x, in.y, in.z };
			return out;
		}

		public:

			// Getters
			static MusicManager* getInstance();

			// Update FMOD
			void update(Ogre::Real deltaT);

			// Music
			void playMainMenuMusic();
			void stopMainMenuMusic();
			void playPlanetMusic();
			void stopPlanetMusic();
			void playSpaceMusic();
			void stopSpaceMusic();
			void playRoomMusic();
			void stopRoomMusic();
			void playPauseMusic();
			void stopPauseMusic();

			// Sounds
			void playEquipSound();
			void playProjectileSound();
			void playRocketSound();
			void playStartSound();
			void playBackSound();
			void playMouseClick();
			void playMouseOverSound();
			void playJumpSound();
			void playInteractBuildingSound();
			void playToggleSound();
			void playCursorMoveSound();
			void playBuildSound();
			void playProjectileHitSound(Ogre::Vector3 pos, Ogre::Vector3 vel);
			FMOD::Channel* playJetstreamSound(Ogre::Vector3 pos, Ogre::Vector3 vel, FMOD::Channel* channel);
			FMOD::Channel* stopJetstreamSound(FMOD::Channel* channel);

			// Set 3D sound attributes
			FMOD::Channel* setAttributes(Ogre::Vector3 pos, Ogre::Vector3 vel, FMOD::Channel* channel);

			// Set listener attributes
			void setListenerAttributes(Ogre::Vector3 pos, Ogre::Vector3 vel, Ogre::Vector3 dir, Ogre::Vector3 up);
	};

}

#endif