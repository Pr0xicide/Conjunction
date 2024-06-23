#ifndef capstone_METALSCRAP_H_
#define capstone_METALSCRAP_H_

#include "Resource.h"

namespace capstone {

	/**
	 * Class definition for the metal scrap that the player can collect.
	 */
	class MetalScrap : public Resource {

	public:

		// Constructor and destructor.
		MetalScrap(SpaceLevel* level, Ogre::Vector3 pos);
		~MetalScrap();
		
		void update(Ogre::Real deltaT);

	private:

		void setup();

	};

}

#endif