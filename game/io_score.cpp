#include "io_score.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "../core/core_base.hpp"
#include "../core/core_error.hpp"

using namespace std;


namespace io {

	class CompareScores {
	public:
		inline bool operator() (uint a, uint b) {
			return (a > b);
		}
	};


	ScoreFile::ScoreFile()
		: scores()
	{
		string scoreStr;

		ifstream f(SAVE_FILE_REL_PATH);
		while (getline(f, scoreStr)) {
			istringstream score_ss(scoreStr);
			uint score;
			score_ss >> score;

			scores.push_back(score);
		}
		f.close();

		sort(scores.begin(), scores.end(), CompareScores());
	}


	void ScoreFile::save(uint score) {
		getInstance();

		if (addScore(score)) {
			ofstream f(SAVE_FILE_REL_PATH);
			for (auto s: scores)
				f << s << '\n';
			f.close();
		}
	}


	void ScoreFile::reset() {
		scores.clear();
		getInstance();
		ofstream f(SAVE_FILE_REL_PATH, ofstream::out | ofstream::trunc);
		f.close();
	}


	bool ScoreFile::addScore(uint score) {
		getInstance();
		
		if (scores.size() >= 10 && score <= scores[scores.size() - 1])
			return false;

		if (scores.size() < 10)
			scores.push_back(score);
		else
			scores[scores.size() - 1] = score;

		sort(scores.begin(), scores.end(), CompareScores());

		return true;
	}

} // namespace io
