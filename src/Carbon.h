#ifndef capstone_CARBON_H_
#define capstone_CARBON_H_

#include "Resource.h"

namespace capstone {

	/**
	 * Class definition for the carbon particles that the player can collect.
	 */
	class Carbon : public Resource {

	public:

		// Constructor and destructor.
		Carbon(SpaceLevel* level, Ogre::Vector3 pos);
		~Carbon();
		
		void update(Ogre::Real deltaT);

	private:

		void setup();

	};

}

#endif
