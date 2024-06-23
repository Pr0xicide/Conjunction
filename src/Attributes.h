#ifndef capstone_REALLOCSTATES_H_
#define capstone_REALLOCSTATES_H_

namespace capstone {

	class ReallocState;
	class SpacePlayer;

	/**
	 * This class 
	 */
	class Attributes {

		// Pointers to Realloc States.
		ReallocState*		r1;
		ReallocState*		r2;
		ReallocState*		r3;
		ReallocState*		currentReallocState;

		// Resources the player is carrying.
		int					metal;
		int					darkMatter;
		int					carbon;

		// Additional player statistics.
		int					metalGained, matterGained, carbonGained;
		int					damageDealt;
		int					shotsHit;
		int					totalShots;
		double				levelAccuracy;

		// Private constructor and destructor (Singleton design pattern).
		Attributes();
		~Attributes();

		// Noncopyable constructor and assignment operator.
        Attributes(const Attributes&);
        Attributes& operator= (const Attributes&);

	public:

		// Getters
		static Attributes* getInstance();
		ReallocState* getR1()								{ return r1; }
		ReallocState* getR2()								{ return r2; }
		ReallocState* getR3()								{ return r3; }
		ReallocState* getCurrentReallocState()				{ return currentReallocState; }
		int getMetal()										{ return metal; }
		int getDarkMatter()									{ return darkMatter; }
		int getCarbon()										{ return carbon; }
		int getHitCount()									{ return shotsHit; }
		int getShotCount()									{ return totalShots; }
		int getMatterGained()								{ return matterGained; }
		int getMetalGained()								{ return metalGained; }
		int getCarbonGained()								{ return carbonGained; }

		// Setters
		void setR1(ReallocState* r1)						{ this->r1 = r1; }
		void setR2(ReallocState* r2)						{ this->r2 = r2; }
		void setR3(ReallocState* r3)						{ this->r3 = r3; }
		void setCurrentReallocState(ReallocState* current)	{ currentReallocState = current; }
		void setMetal(int metal)							{ this->metal = metal; }
		void setDarkMatter(int darkMatter)					{ this->darkMatter = darkMatter; }
		void setCarbon(int carbon)							{ this->carbon = carbon; }
		void decrementResources(int metal, int darkMatter, int carbon);
		void incrementMetal(int metal)						{ this->metal += metal; }
		void incrementDarkMatter(int darkMatter)			{ this->darkMatter += darkMatter; }
		void incrementCarbon(int carbon)					{ this->carbon += carbon; }
		void incrementHitCount()							{ shotsHit++; }
		void incrementShotCount()							{ totalShots++; }
		void incrementMatterGained(int amt)					{ matterGained += amt; }
		void incrementMetalGained(int amt)					{ metalGained += amt; }
		void incrementCarbonGained(int amt)					{ carbonGained += amt; }
		void resetLevelProperties();
		// metalGained, matterGain, carbonGain;
	};
}

#endif