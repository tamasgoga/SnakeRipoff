#include "g_pulse.hpp"


Pulse::Pulse(uint64_t max, uint64_t initDecay, tstep_t timeStep)
	: max(max)
	, initDecay(initDecay)
	, timeStep(timeStep)
	, decay(initDecay)
	, value(0)
	, timer(true)
{;}


void Pulse::reset() {
	value = max;
	decay = initDecay;
	timer.reset();
}


uint64_t Pulse::get() {
	uint64_t decrease = decay * (timer.elapsed_ms().count() / timeStep);

	if (decrease > 0) {
		decay += (decay >> 2);
		value = value >= decrease ? value - decrease : 0;
		timer.reset();
	}

	return value;
}
