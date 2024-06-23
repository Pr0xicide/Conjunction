#ifndef capstone_REALLOCSTATES_H_
#define capstone_REALLOCSTATES_H_

namespace capstone{

	class ReallocState;
	class SpacePlayer;

	class ReallocStates{

		ReallocState* r1;
		ReallocState* r2;
		ReallocState* r3;

	public:
		ReallocStates();
		~ReallocStates();

		// Getters
		static ReallocStates* getInstance();
		ReallocState* getR1()					{ return r1; }
		ReallocState* getR2()					{ return r2; }
		ReallocState* getR3()					{ return r3; }

		// Setters
		void setR1(ReallocState* r1)			{ this->r1 = r1; }
		void setR2(ReallocState* r2)			{ this->r2 = r2; }
		void setR3(ReallocState* r3)			{ this->r3 = r3; }
	};
}

#endif