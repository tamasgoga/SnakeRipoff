#ifndef FLAG_CORE_CORE_TIME_HPP
#define FLAG_CORE_CORE_TIME_HPP

//------------------------------------------------------------
// Timing related classes & functions
//------------------------------------------------------------

#include <chrono>
#include <cstdint>


namespace core {

	/** Definitions for commonly used durations units */
	namespace time {
		using nano	= std::chrono::duration<int_least64_t, std::nano>;
		using micro = std::chrono::duration<int_least64_t, std::micro>;
		using milli = std::chrono::duration<int_least64_t, std::milli>;
		using sec	= std::chrono::duration<int_least64_t>;
		using min	= std::chrono::duration<int_least32_t, std::ratio<60>>;
		using hour	= std::chrono::duration<int_least32_t, std::ratio<3600>>;
	}


	/** A very basic timer class */
	class SimpleTimer {
	public:
		using clock = std::chrono::high_resolution_clock;
		using duration = clock::duration;

	private:
		clock::time_point start;

	public:
		/** Construction */
		explicit SimpleTimer(bool start_now = false) noexcept;
		SimpleTimer(const SimpleTimer&) noexcept;
		SimpleTimer& operator=(const SimpleTimer&) noexcept;

		/** Reset the clock. */
		inline void reset() noexcept {start = clock::now();}

		/** Get the elapsed time in various formats. */
		inline duration 	elapsed()		const noexcept {return clock::now() - start;}
		inline time::nano	elapsed_ns()	const {return std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - start);}
		inline time::micro	elapsed_us()	const {return std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start);}
		inline time::milli	elapsed_ms()	const {return std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start);}
		inline time::sec	elapsed_s()		const {return std::chrono::duration_cast<std::chrono::seconds>(clock::now() - start);}
		inline time::min	elapsed_min()	const {return std::chrono::duration_cast<std::chrono::minutes>(clock::now() - start);}
		inline time::hour	elapsed_h()		const {return std::chrono::duration_cast<std::chrono::hours>(clock::now() - start);}

		/** Getters */
		inline clock::time_point getStart() const noexcept {return start;}
	};


	/** Measures the frame-rate */
	class FramerateCounter {
	private:
		SimpleTimer timer;
		int frames;
		float fps;

	public:
		time::milli::rep interval;

		/** Construction */
		FramerateCounter(bool start_now = false, time::milli::rep updateInterval = 250) noexcept;
		FramerateCounter(const FramerateCounter&) noexcept;
		FramerateCounter& operator=(const FramerateCounter& other) noexcept;

		/** Handling */
		inline void start() noexcept {timer.reset();}
		float update();

		/** Getters */
		inline float get() const noexcept {return fps;}
	};

} // namespace core


#endif // FLAG_CORE_CORE_TIME_HPP
