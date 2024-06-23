#include "stdafx.h"

#include "ReallocState.h"
#include "GlobalVariables.h"
#include "SpaceLevel.h"
#include "Player.h"
#include "SpacePlayer.h"
#include "Part.h"
#include "CompletePart.h"

namespace capstone {

	/**
	 * Default constructor
	 */
	ReallocState::ReallocState()
		: mHead(NULL)
		, mLeftArm(NULL)
		, mRightArm(NULL)
		, isSelected(false)
	{
		mHead = new CompletePart();
		mLeftArm = new CompletePart();
		mRightArm = new CompletePart();
		mLeftShoulder = new CompletePart();
		mRightShoulder = new CompletePart();
	}
	
	/**
	 * Overloaded Constructor to be used with save files.
	 *
	 * @param head
	 * @param leftArm
	 * @param rightArm
	 *
	 */
	ReallocState::ReallocState(CompletePart* head, CompletePart* leftArm, CompletePart* rightArm,
			CompletePart* leftShoulder, CompletePart* rightShoulder)	//parts must come in this order
		: mHead(head)
		, mLeftArm(leftArm)
		, mRightArm(rightArm)
		, mLeftShoulder(leftShoulder)
		, mRightShoulder(rightShoulder)
		, isSelected(false)
	{	

	}

	/**
	 * Class Destructor.
	 */
	ReallocState::~ReallocState() {
	}
	
	/**
	 * This function will define the part that will go onto the head bone.
	 */
	void ReallocState::setHeadPart(CompletePart* head){

		if(mHead != head) {
			head->getSceneNode()->detachAllObjects();
			//mHead->setIsEquipped(false);
			mHead = head; 
		}

	}

	/**
	 * This function will define the part that will go onto the left arm bone.
	 */
	void ReallocState::setLeftArmPart(CompletePart* leftArm){

		if(mLeftArm != leftArm){
			leftArm->getSceneNode()->detachAllObjects();
			//mLeftArm->setIsEquipped(false);
			mLeftArm = leftArm; 
		}
	}
	
	/**
	 * This function will define the part that will go onto the right arm bone.
	 */
	void ReallocState::setRightArmPart(CompletePart* rightArm){

		if(mRightArm != rightArm){
			rightArm->getSceneNode()->detachAllObjects();
			//mRightArm->setIsEquipped(false);
			mRightArm = rightArm; 
		}
	}

	void ReallocState::setLeftShoulderPart(CompletePart* leftShoulder){

		if(mLeftShoulder != leftShoulder){
			leftShoulder->getSceneNode()->detachAllObjects();
			//mLeftShoulder->setIsEquipped(false);
			mLeftShoulder = leftShoulder; 
		}
	}

	void ReallocState::setRightShoulderPart(CompletePart* rightShoulder){

		if(mRightShoulder != rightShoulder){
			rightShoulder->getSceneNode()->detachAllObjects();
			//mRightShoulder->setIsEquipped(false);
			mRightShoulder = rightShoulder;
		}
	}

	void ReallocState::setEquippedPartsFlags(bool b){
		mHead->setIsEquipped(b);
		mLeftArm->setIsEquipped(b);
		mRightArm->setIsEquipped(b);
		mLeftShoulder->setIsEquipped(b);
		mRightShoulder->setIsEquipped(b);
	}

}