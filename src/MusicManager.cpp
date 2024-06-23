#include "stdafx.h"

#include "MusicManager.h"

namespace capstone{

	MusicManager::MusicManager(){
		initialize();
		createMusic();
		createSounds();
	}

	MusicManager::~MusicManager(){

		if(mainMenuMusic){
			result = mainMenuMusic->release();
			errorCheck("Failed to release main menu music");
		}
		if(planetMusic){
			result = planetMusic->release();
			errorCheck("Failed to release planet music");
		}
		if(spaceMusic){
			result = spaceMusic->release();
			errorCheck("Failed to release space music");
		}
		if(roomMusic){
			result = roomMusic->release();
			errorCheck("Failed to release room music");
		}
		if(pauseMusic){
			result = pauseMusic->release();
			errorCheck("Failed to release pause menu music");
		}
		if(jetstreamSound){
			result = jetstreamSound->release();
			errorCheck("Failed to release jetstream sound");
		}

		result = system->release();
		errorCheck("Failed to release the sound system");
	}

	MusicManager* MusicManager::getInstance(){
		static MusicManager musicManager;
		return &musicManager;
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	void MusicManager::initialize(){

		result = FMOD::System_Create(&system);	// Create the main system object.
		errorCheck("Failed to create the sound system");

		result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
		errorCheck("Failed to initialize FMOD");

		result = system->set3DSettings(0.05, 1, 1);
		errorCheck("Failed to set 3D sound settings");
	}

	void MusicManager::setListenerAttributes(Ogre::Vector3 pos, Ogre::Vector3 vel, Ogre::Vector3 dir, Ogre::Vector3 up){

		result = system->set3DListenerAttributes(0,
											&OgreToFmod(pos), &OgreToFmod(vel),
											&OgreToFmod(dir), &OgreToFmod(up));
		errorCheck("Failed to set 3D listener attributes");

	}

	FMOD::Channel* MusicManager::setAttributes(Ogre::Vector3 pos, Ogre::Vector3 vel, FMOD::Channel* channel){

		result = channel->set3DAttributes(&OgreToFmod(pos), &OgreToFmod(vel));
		errorCheck("Failed to set sound 3D attributes for jetstream");

		return channel;
	}

	void MusicManager::createMusic(){
		result = system->createStream("media/bgm/05 Utsuronaru Yuukyou ~Game Over~.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &mainMenuMusic);
		errorCheck("Failed to create main menu music");
		result = system->createStream("media/bgm/15 Synchrodrive.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &planetMusic);
		errorCheck("Failed to create planet music");
		result = system->createStream("media/bgm/17 Augusta Tower -Agressive Mix-.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &spaceMusic);
		errorCheck("Failed to create space music");
		result = system->createStream("media/bgm/19 Academy no Theme.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &roomMusic);
		errorCheck("Failed to create room music");
		result = system->createStream("media/bgm/2-26 Space.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &pauseMusic);
		errorCheck("Failed to create pause menu music");
	}

	void MusicManager::createSounds(){
		result = system->createSound("media/bgm/65402__dk__shotgun-cocking.mp3", FMOD_LOOP_OFF | FMOD_2D, 0, &equipSound);
		errorCheck("Failed to create equipping sound");

		result = system->createSound("media/bgm/80921__justinbw__buttonchime02up.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &buildSound);
		errorCheck("Failed to create build sound");

		result = system->createSound("media/bgm/28917__junggle__btn107.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &projectileSound);
		errorCheck("Failed to create projectile sound");

		result = system->createSound("media/bgm/47251__nthompson__rocket.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &rocketSound);
		errorCheck("Failed to create rocket sound");

		result = system->createSound("media/bgm/mouse_click1.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &mouseClickSound);
		errorCheck("Failed to create mouse click sound");

		result = system->createSound("media/bgm/menu_start.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &menuStartSound);
		errorCheck("Failed to create start game sound");

		result = system->createSound("media/bgm/menu_back.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &menuBackSound);
		errorCheck("Failed to create go back sound");

		result = system->createSound("media/bgm/jump.mp3", FMOD_LOOP_OFF | FMOD_2D, 0, &jumpSound);
		errorCheck("Failed to create jump sound");

		result = system->createSound("media/bgm/interact_building.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &interactBuildingSound);
		errorCheck("Failed to create buidling interaction sound");

		result = system->createSound("media/bgm/mouse_hover.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &btnHoverSound);
		errorCheck("Failed to create button hover sound");

		result = system->createSound("media/bgm/menu_tutorial.mp3", FMOD_LOOP_OFF | FMOD_2D, 0, &btnToggleSound);
		errorCheck("Failed to create toggle sound");

		result = system->createSound("media/bgm/cursor_move.mp3", FMOD_LOOP_OFF | FMOD_2D, 0, &cursorMoveSound);
		errorCheck("Failed to create cursor move sound");

		result = system->createSound("media/bgm/158894__primeval-polypod__rocket-launch.wav", FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL, 0, &jetstreamSound);
		errorCheck("Failed to create jetstream sound");
		result = jetstreamSound->set3DMinMaxDistance(0, 600);
		errorCheck("Failed to set 3D min and max distance for jetstream sound");

		result = system->createSound("media/bgm/103632__benboncan__large-anvil-steel-hammer-4.wav", FMOD_3D | FMOD_3D_LINEARROLLOFF, 0, &projectileHitSound);
		errorCheck("Failed to create projectile sound");
		result = projectileHitSound->set3DMinMaxDistance(0, 500);
		errorCheck("Failed to set 3D min and max distance for projectile sound");

		// cursor_move

	}


// =====================================================================================
//
// #Update
//
// =====================================================================================

	void MusicManager::update(Ogre::Real deltaT)
	{
		result = system->update();
		errorCheck("Failed to update sound system");
	}


// =====================================================================================
//
// # Misc Sounds
//
// =====================================================================================
	
	void MusicManager::playEquipSound(){
		result = system->playSound(FMOD_CHANNEL_FREE, equipSound, false, 0);
		errorCheck("Failed to play equipping sound");
	}

	void MusicManager::playBuildSound(){
		result = system->playSound(FMOD_CHANNEL_FREE, buildSound, false, 0);
		errorCheck("Failed to play build sound");
	}

	void MusicManager::playProjectileSound(){
		result = system->playSound(FMOD_CHANNEL_FREE, projectileSound, false, 0);
		errorCheck("Failed to play projectile sound");
	}

	void MusicManager::playRocketSound(){
		result = system->playSound(FMOD_CHANNEL_FREE, rocketSound, true, &rocketChannel);
		errorCheck("Failed to play rocket sound");

		result = rocketChannel->setVolume(0.60f);
		errorCheck("Failed to set the rocket sound volume");

		result = rocketChannel->setPaused(false);
		errorCheck("Failed to unpause the rocket sound");
	}

	void MusicManager::playMouseClick() {
		result = system->playSound(FMOD_CHANNEL_FREE, mouseClickSound, false, 0);
		errorCheck("Failed to play mouse click sound");
	}

	void MusicManager::playJumpSound() {
		result = system->playSound(FMOD_CHANNEL_FREE, jumpSound, false, 0);
		errorCheck("Failed to play jump sound");
	}

	void MusicManager::playStartSound() {
		result = system->playSound(FMOD_CHANNEL_FREE, menuStartSound, false, 0);
		errorCheck("Failed to play start game sound");
	}

	void MusicManager::playBackSound() {
		result = system->playSound(FMOD_CHANNEL_FREE, menuBackSound, false, 0);
		errorCheck("Failed to play go back sound");
	}

	void MusicManager::playInteractBuildingSound() {
		result = system->playSound(FMOD_CHANNEL_FREE, interactBuildingSound, false, 0);
		errorCheck("Failed to play building interaction sound");
	}

	void MusicManager::playMouseOverSound() {
		result = system->playSound(FMOD_CHANNEL_FREE, btnHoverSound, false, 0);
		errorCheck("Failed to play mouse button hover sound");
	}

	void MusicManager::playToggleSound() {
		result = system->playSound(FMOD_CHANNEL_FREE, btnToggleSound, false, 0);
		errorCheck("Failed to play toggle sound");
	}

	void MusicManager::playCursorMoveSound() {
		result = system->playSound(FMOD_CHANNEL_FREE, cursorMoveSound, false, 0);
		errorCheck("Failed to play cursor move sound");
	}

	void MusicManager::playProjectileHitSound(Ogre::Vector3 pos, Ogre::Vector3 vel){

		result = system->playSound(FMOD_CHANNEL_FREE, projectileHitSound, true, &projectileHitChannel);
		errorCheck("Failed to play projectile hit sound");

		projectileHitChannel = setAttributes(pos, vel, projectileHitChannel);

		result = projectileHitChannel->setPaused(false);
		errorCheck("Failed to unpause projectile hit sound");
	}

	FMOD::Channel* MusicManager::playJetstreamSound(Ogre::Vector3 pos, Ogre::Vector3 vel, FMOD::Channel* channel){

		result = system->playSound(FMOD_CHANNEL_FREE, jetstreamSound, true, &channel);
		errorCheck("Failed to play jetstream sound");

		channel = setAttributes(pos, vel, channel);

		result = channel->setVolume(0.50f);
		errorCheck("Failed to set jetstream volume");
		
		result = channel->setPaused(false);
		errorCheck("Failed to unpause jetstream sound");

		return channel;
	}

	FMOD::Channel* MusicManager::stopJetstreamSound(FMOD::Channel* channel){

		result = channel->stop();
		errorCheck("Failed to stop jetstream sound");

		return channel;

	}


// =====================================================================================
//
// #Main Menu Music
//
// =====================================================================================

	void MusicManager::playMainMenuMusic(){
		result = system->playSound(FMOD_CHANNEL_FREE, mainMenuMusic, false, &mainMenuChannel);
		errorCheck("Failed to play main menu music");
	}
	
	void MusicManager::stopMainMenuMusic(){
		result = mainMenuChannel->stop();
		errorCheck("Failed to stop main menu music");
	}


// =====================================================================================
//
// #Planet Background Music
//
// =====================================================================================

	void MusicManager::playPlanetMusic() {
		result = system->playSound(FMOD_CHANNEL_FREE, planetMusic, false, &planetChannel);
		errorCheck("Failed to play planet music");
		result = planetChannel->setVolume(0.25f);
		errorCheck("Failed to set the planet music volume");
	}

	void MusicManager::stopPlanetMusic() {
		result = planetChannel->stop();
		errorCheck("Failed to stop planet music");
	}


// =====================================================================================
//
// #Space Background Music.
//
// =====================================================================================

	void MusicManager::playSpaceMusic() {
		result = system->playSound(FMOD_CHANNEL_FREE, spaceMusic, false, &spaceChannel);
		errorCheck("Failed to play space music");
		result = spaceChannel->setVolume(0.35f);
		errorCheck("Failed to set space level volume");
	}
	
	void MusicManager::stopSpaceMusic() {
		result = spaceChannel->stop();
		errorCheck("Failed to stop space music");
	}


// =====================================================================================
//
// #Room Background Music
//
// =====================================================================================

	void MusicManager::playRoomMusic(){
		result = system->playSound(FMOD_CHANNEL_FREE, roomMusic, false, &roomChannel);
		result = roomChannel->setVolume(0.35f);
		errorCheck("Failed to play room music");
	}
	
	void MusicManager::stopRoomMusic(){
		result = roomChannel->stop();
		errorCheck("Failed to stop room music");
	}


// =====================================================================================
//
// #Pause Menu Background Music
//
// =====================================================================================

	void MusicManager::playPauseMusic() {
		result = system->playSound(FMOD_CHANNEL_FREE, pauseMusic, false, &pauseChannel);
		errorCheck("Failed to play pause menu music");
		result = pauseChannel->setVolume(0.25f);
		errorCheck("Failed to set the pause menu music");
	}
	
	void MusicManager::stopPauseMusic(){
		result = pauseChannel->stop();
		errorCheck("Failed to stop pause menu music");
	}
	

// =====================================================================================
//
// #Exceptation Handling
//
// =====================================================================================

	void MusicManager::errorCheck(const std::string& errMsg){

		if(result != FMOD_OK){

			std::stringstream ss;

			ss << errMsg << "FMOD error! " << result << ": " << FMOD_ErrorString(result);

			throw std::exception(ss.str().c_str());
		}

	}

}
