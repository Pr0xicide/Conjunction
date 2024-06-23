#include "stdafx.h"

#include "ReallocState.h"
#include "GlobalVariables.h"
#include "SpacePlayer.h"
#include "Attributes.h"

namespace capstone {

	/**
	 * Default constructor.
	 */
	Attributes::Attributes()
		: damageDealt(0)
		, levelAccuracy(0)
		, shotsHit(0)
		, totalShots(0)
		, metalGained(0)
		, matterGained(0)
		, carbonGained(0)
	{

		// Instantiate realloc states.
		r1 = new ReallocState();
		r2 = new ReallocState();
		r3 = new ReallocState();

		// Set the current realloc state.
		currentReallocState = r1;

		// Hardcoded values for now.
		metal = 20;
		darkMatter = 10;
		carbon = 100;

	}

	/**
	 * Class Destructor
	 */
	Attributes::~Attributes() {

		// Delete realloc states.
		delete r1;
		delete r2;
		delete r3;

	}

	/**
	 * This function initializes a singleton design pattern of this class to use.
	 *
	 * @return		pointer to this class
	 *
	 */
	Attributes* Attributes::getInstance() {
		static Attributes attributes;
		return &attributes;
	}

	/**
	 * This function gets called when creating a part from a blueprint.
	 *
	 * @param metal			ammount of metal that needs to get decremented
	 * @param darkMatter	ammount of dark matter that needs to get decremented
	 * @param carbon		ammount of carbo that needs to get decremented
	 *
	 */
	void Attributes::decrementResources(int metal, int darkMatter, int carbon) {
		this->metal -= metal;
		this->darkMatter -= darkMatter;
		this->carbon -= carbon;
	}

	/**
	 * This function reset the level properties so that the next level 
	 * stats will calculated again.
	 */
	void Attributes::resetLevelProperties() {

		damageDealt = 0;
		shotsHit = 0;
		totalShots = 0;
		levelAccuracy = 0;
		metalGained = 0;
		matterGained = 0;
		carbonGained = 0;

	}

}