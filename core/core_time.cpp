#include "core_time.hpp"


namespace core {

	//------------------------------------------------------------
	// class: SimpleTimer
	//------------------------------------------------------------

	SimpleTimer::SimpleTimer(bool start_now) noexcept {
		if (start_now)
			start = clock::now();
	}

	SimpleTimer::SimpleTimer(const SimpleTimer& other) noexcept
		: start(other.start)
	{;}

	SimpleTimer& SimpleTimer::operator=(const SimpleTimer& other) noexcept {
		start = other.start;
		return *this;
	}

	//------------------------------------------------------------
	// class: FramerateCounter
	//------------------------------------------------------------

	FramerateCounter::FramerateCounter(bool start_now, time::milli::rep updateInterval) noexcept
		: timer(start_now)
		, frames(0)
		, fps(0.f)
		, interval(updateInterval)
	{;}

	FramerateCounter::FramerateCounter(const FramerateCounter& other) noexcept
		: timer(other.timer)
	{;}

	FramerateCounter& FramerateCounter::operator=(const FramerateCounter& other) noexcept {
		timer = other.timer;
		return *this;
	}

	float FramerateCounter::update() {
		++frames;
		auto elapsed = timer.elapsed_ms().count();

		if (elapsed > interval && frames > 10) {
			float felaps = 0.001f * elapsed;
			fps = static_cast<float>(frames) / felaps;
			timer.reset();
			frames = 0;
		}

		return fps;
	}

} // namespace core
