#include "stdafx.h"

#include "Util.h"

namespace capstone {

	Ogre::Degree GetAbsYaw(Ogre::Vector2 dir1, Ogre::Vector2 dir2)
    {
        // could avoid these normalizations if we knew that dir1 and dir2 are unit vectors
        dir1.normalise();
        dir2.normalise();

        // return angle in [0,180] range
       return Ogre::Math::ACos(dir1.dotProduct(dir2));
    }

    Ogre::Degree GetSignedYaw(Ogre::Vector2 dir1, Ogre::Vector2 dir2)
    {
        // get angle (always in [0,180] range)
        Ogre::Degree yawToGoal = GetAbsYaw(dir1, dir2);

        // get right perpendicular vector
        Ogre::Vector2 left = GetLeftPerp(dir1);

        // compute sign
        if (left.dotProduct(dir2) > 0)
            return yawToGoal;
        else
            return -yawToGoal;
    }
}