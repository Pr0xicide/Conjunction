#ifndef capstone_DARKMATTER_H_
#define capstone_DARKMATTER_H_

#include "Resource.h"

namespace capstone {

	/**
	 * Class definition for the dark matter particles that the player can collect.
	 */
	class DarkMatter : public Resource {

	public:

		// Constructor and destructor.
		DarkMatter(SpaceLevel* level, Ogre::Vector3 pos);
		~DarkMatter();
		
		void update(Ogre::Real deltaT);

	private:

		void setup();

	};

}

#endif