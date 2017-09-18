#ifndef FLAG_CORE_CORE_DEF_HPP
#define FLAG_CORE_CORE_DEF_HPP

//------------------------------------------------------------
// Definitions & abbreviations
//------------------------------------------------------------


#define CORE_PROTOTYPE          // undefined functions
#define CORE_INIT               // functions that should be called in core::init()


#ifndef CORE_NO_TYPE_ABBREVIATIONS

/** Type abbreviations; these can be disabled by defining the macro above. */
using uint = unsigned int;
using ulong = unsigned long int;

#endif // CORE_NO_TYPE_ABBREVIATIONS


namespace core {

	/** Represents the error code returned by Core & SDL calls.
	    The error message can be reached reached by calling errorMessage calls.
	    Interchangeable with int.

	    Meaning:
	      - 0: success;
	      - negative: SDL errors;
	      - positive: Core errors. */
	using error = int;

}

#endif // FLAG_CORE_CORE_DEF_HPP
