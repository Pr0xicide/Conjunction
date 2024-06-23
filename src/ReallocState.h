#ifndef capstone_REALLOCSTATE_H_
#define capstone_REALLOCSTATE_H_

namespace capstone{

	//forward declarations
	class Part;
	class CompletePart;
	class SpacePlayer;

	/**
	 * This class is the blueprint for the realloc states for the robot for switching parts.
	 */
	class ReallocState {

		bool isSelected;

		// Pointer to each node.
		CompletePart* mHead;
		CompletePart* mLeftArm;
		CompletePart* mRightArm;
		CompletePart* mLeftShoulder;
		CompletePart* mRightShoulder;

		// Pointer to the player.
		SpacePlayer* player;

	public:

		// Constructors and destructor.
		ReallocState();													// Default constructor.
		ReallocState(CompletePart* head, CompletePart* leftArm, CompletePart* rightArm,
			CompletePart* leftShoulder, CompletePart* rightShoulder);	// Parts must come in this order.
		// Copyable.
        ReallocState(const ReallocState&);
        ReallocState& operator= (const ReallocState&);
		~ReallocState();

		// Getters.
		bool				getIsSelected()			{ return isSelected; }
		CompletePart*		getHeadPart()			{ return mHead; }
		CompletePart*		getLeftArmPart()		{ return mLeftArm; }
		CompletePart*		getRightArmPart()		{ return mRightArm; }
		CompletePart*		getLeftShoulderPart()	{ return mLeftShoulder; }
		CompletePart*		getRightShoulderPart()	{ return mRightShoulder; }

		// Setters.
		void		setIsSelected(bool isSelected)		{ this->isSelected = isSelected; }
		void		setHeadPart(CompletePart* head);
		void		setLeftArmPart(CompletePart* leftArm);
		void		setRightArmPart(CompletePart* rightArm);
		void		setLeftShoulderPart(CompletePart* leftShoulder);
		void		setRightShoulderPart(CompletePart* rightShoulder);
		void		setEquippedPartsFlags(bool);
	};
}

#endif