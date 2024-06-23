#include "stdafx.h"

#include "GlobalVariables.h"

namespace capstone {

	// Instantiate variables.
	Ogre::Real gFreelookMouseSpeed(180 / 1000.0f);
    Ogre::Real gMouseSensitivity(1);

	/**
	 * This function converts char pointers to integers values.
	 *
	 * @param c				char pointer to be converted
	 *
	 * @return an integer value from the character pointer
	 *
	 */
	int charToInt(const char* c) {
		std::string string = c;
		return atoi(string.c_str());
	}

	/**
	 * This function converts char pointers to float values.
	 *
	 * @param c				char pointer to be converted
	 *
	 * @return a float value from the character pointer
	 *
	 */
	float charToFloat(const char* c) {
		std::string string = c;
		return atof(string.c_str());
	}

	/**
	 * This function converts int variables to strings.
	 *
	 * @param num			number to convert
	 *
	 * @return a string with the number stored
	 *
	 */
	std::string intToString(int num) {
		std::ostringstream numCoverter;
		numCoverter << num;
		return numCoverter.str();
	}

	/**
	 * This function converts double variables to strings.
	 *
	 * @param num			number to convert
	 *
	 * @return a string with the number stored
	 *
	 */
	std::string doubleToString(double num) {
		std::ostringstream numCoverter;
		numCoverter << num;
		return numCoverter.str();
	}

	/**
	 * This function is used to match the mouse button ID with the CEGUI ones.
	 */
	CEGUI::MouseButton ConvertButton(OIS::MouseButtonID buttonId) {
		switch (buttonId) {
			case OIS::MB_Left:    return CEGUI::LeftButton;
			case OIS::MB_Right:   return CEGUI::RightButton;
			case OIS::MB_Middle:  return CEGUI::MiddleButton;
			default:              return CEGUI::LeftButton;
		}
	}

}
