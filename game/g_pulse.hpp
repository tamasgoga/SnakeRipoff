#ifndef SNAKE_GAME_G_PULSE_HPP
#define SNAKE_GAME_G_PULSE_HPP


//--------------------------------------------------------------
// Pulse with linear decay in milliseconds
//--------------------------------------------------------------

#include "../core/core_time.hpp"


class Pulse {
public:
	using tstep_t = core::SimpleTimer::clock::rep;

	const uint64_t max;
	const uint64_t initDecay;
	const tstep_t timeStep;

	Pulse(uint64_t max, uint64_t initDecay, tstep_t timeStep);

	void reset();
	uint64_t get();

	inline bool hasPulsed() {
		return (value > 0) && (timer.elapsed_ms().count() / timeStep > 0);
	}

	inline bool isActive() {
		return value > 0;
	}

private:
	uint64_t decay;
	uint64_t value;
	core::SimpleTimer timer;
};


#endif // SNAKE_GAME_G_PULSE_HPP
