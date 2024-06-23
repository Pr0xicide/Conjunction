#ifndef capstone_GLOBALS_VARS_H_
#define capstone_GLOBALS_VARS_H_

namespace capstone {

	// Gravity acceleration vector.
    const Ogre::Vector3 gGravity(0, -9.8f, 0);        

	// Values used to determine part types
	const unsigned HEAD_PART = 1;
	const unsigned LEFT_ARM_PART = 2;
	const unsigned RIGHT_ARM_PART = 3;
	const unsigned LEFT_SHOULDER_PART = 4;
	const unsigned RIGHT_SHOULDER_PART = 5;

	// Values used to determine skill types
	const unsigned MELEE_SKILL = 0;
	const unsigned PROJECTILE_SKILL = 1;
	const unsigned PASSIVE_SKILL = 2;

	extern Ogre::Real gFreelookMouseSpeed;
    extern Ogre::Real gMouseSensitivity;

	// default orientation vectors
    const Ogre::Vector3 gForward(Ogre::Vector3(1, 0, 0));

	// default orientation vectors
    const Ogre::Vector3 gForwardZ(Ogre::Vector3(0, 0, 1));

	int charToInt(const char* c);
	float charToFloat(const char* c);
	std::string intToString(int num);
	std::string doubleToString(double num);

	CEGUI::MouseButton ConvertButton(OIS::MouseButtonID buttonId);

}

#endif
