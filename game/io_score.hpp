#ifndef SNAKE_GAME_IO_SCORE_HPP
#define SNAKE_GAME_IO_SCORE_HPP


#include <vector>
#include "../core/core_def.hpp"


namespace io {

	/** Singleton that holds the high-scores.
	    Holds 10 scores at most. */
	class ScoreFile {
		public:
			using size_type = std::vector<uint>::size_type;


			// Get an instance
			static ScoreFile& getInstance() {
				static ScoreFile instance;
				return instance;
			}


			// Save one score
			void save(uint score);


			uint& operator[](size_type i) {
				return scores[i < 10 ? i : 9];
			}


			const uint& operator[](size_type i) const {
				return scores[i < 10 ? i : 9];
			}


			size_type size() const {
				return scores.size();
			}


		private:
			// C-tor, reads the score file if it exists
			ScoreFile();


			// Add a score to the vector
			bool addScore(uint score);


			std::vector<uint> scores;
			const char* SAVE_FILE_REL_PATH = "res/snake.save";

		// Singleton related stuff
		public:
			ScoreFile(const ScoreFile&)      = delete;
			void operator=(const ScoreFile&) = delete;
	};

} // namespace io


#endif // SNAKE_GAME_IO_SCORE_HPP
