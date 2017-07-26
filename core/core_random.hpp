#ifndef FLAG_CORE_CORE_RANDOM_HPP
#define FLAG_CORE_CORE_RANDOM_HPP

//------------------------------------------------------------
// Random number generation
//------------------------------------------------------------


#include <random>
#include <limits>
#include <type_traits>

#define CORE_GET_RANDOM_SEED core::sys_rand()


namespace core {

	/** SLOW, possibly non-deterministic random number generator */
	extern std::random_device sys_rand;

	/** FAST deterministic generator of 32 or 64 random bits:
		Seed with sys_rand() for simulated non-determinism.
		Use the functions below to get a proper number out of the generated bits. */
	using Random32 = std::mt19937;
	using Random64 = std::mt19937_64;


	/** Get a random integral number */
	template <typename RNG, typename IntType = int>
	IntType randomInt(RNG& rng,
	                  IntType min = std::numeric_limits<IntType>::min(),
	                  IntType max = std::numeric_limits<IntType>::max())
	{
		static_assert(std::is_integral<IntType>::value, "core::randomInt(...) requires an integral type.");
		std::uniform_int_distribution<IntType> dist(min, max);
		return dist(rng);
	}


	/** Get a random real number */
	template <typename RNG, typename RealType = float>
	RealType randomReal(RNG& rng,
	                    RealType min = std::numeric_limits<RealType>::min(),
	                    RealType max = std::numeric_limits<RealType>::max())
	{
		static_assert(std::is_floating_point<RealType>::value, "core::randomReal(...) requires an real type.");
		std::uniform_real_distribution<RealType> dist(min, max);
		return dist(rng);
	}

} // namespace core


#endif // FLAG_CORE_CORE_RANDOM_HPP
