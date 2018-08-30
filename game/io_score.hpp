#ifndef SNAKE_GAME_IO_SCORE_HPP
#define SNAKE_GAME_IO_SCORE_HPP


#include <vector>
#include "../core/core_def.hpp"


namespace io {

	/** Singleton that holds the high-scores */
	class ScoreFile {
		public:
			// Get an instance
			static ScoreFile& getInstance() {
				static ScoreFile instance;
				return instance;
			}


			/** Holds one score */
			struct score {
				long long timestamp;
				uint score;
			};


			// Save one score
			void save(uint score);


		private:
			ScoreFile();

			std::vector<score> scores;

			static const char* SAVE_FILE_REL_PATH;

		// Singleton related stuff
		public:
			ScoreFile(const ScoreFile&)      = delete;
			void operator=(const ScoreFile&) = delete;
	};

} // namespace io


#endif // SNAKE_GAME_IO_SCORE_HPP
