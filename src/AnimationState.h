#ifndef capstone_ANIMATIONSTATE_H_
#define capstone_ANIMATIONSTATE_H_

namespace capstone {

	/**
	 * Base class definition for all animation states.
	 */
	class AnimationState {

	protected:

		// Instance variables.
		Ogre::AnimationState*			state;
		Ogre::Entity*					entity;
		Ogre::String					name;

	public:

		// Constructor & destructor.
		AnimationState(Ogre::Entity* entity, Ogre::String name);
		virtual ~AnimationState();

		// Abstract methods needed for all child classes.
		virtual void update(Ogre::Real deltaT) = 0;

		void enter();
		void exit();
		
		// Getters.
		bool isAnimationDone()			{ return state->hasEnded(); }

		// Setters.
		void resetAnimation()			{ state->setTimePosition(0); }

	};

}

#endif