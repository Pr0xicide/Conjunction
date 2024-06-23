#ifndef capstone_EXPLOSIONS_H_
#define capstone_EXPLOSIONS_H_

namespace capstone {

    class SpaceLevel;

    class Explosions {

        // Static id number needed to generate a unique name for each instance
        // of the fireball material.  Incremented every time a new explosion is generated.
        static int msMatId;

        SpaceLevel*					mSpaceLevel;

		Ogre::BillboardSet*			mBillboardSet;
		Ogre::Real					mDisposalTime;
		Ogre::Real					mFireballSize;
		Ogre::SceneNode*			mSceneNode;
        Ogre::String				mMatName;

    private:

        // Noncopyable.
        Explosions(const Explosions&);
        Explosions& operator= (const Explosions&);

    public:

		// Constructor & destructor.
        Explosions(SpaceLevel* mSpaceLevel,
                          Ogre::Real fireballSize,
                          const Ogre::Vector3& position);
        ~Explosions();

		// Getters.
        bool isDisposable() const;

    };
}

#endif