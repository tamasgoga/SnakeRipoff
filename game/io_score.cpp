#include "io_score.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "../core/core_base.hpp"
#include "../core/core_error.hpp"

using namespace std;
using namespace std::chrono;


namespace io {

	class CompareScores {
	public:
		inline bool operator() (const ScoreFile::score& a, const ScoreFile::score& b) {
			return (a.score > b.score);
		}
	};


	ScoreFile::ScoreFile()
		: scores()
	{
		string timestamp, score;

		ifstream f(SAVE_FILE_REL_PATH);
		while (getline(f, timestamp, ',')) {
			getline(f, score);

			struct score s;
			istringstream timestamp_ss(timestamp), score_ss(score);
			timestamp_ss >> s.timestamp;
			score_ss >> s.score;

			scores.push_back(s);
		}
		f.close();

		sort(scores.begin(), scores.end(), CompareScores());
	}


	void ScoreFile::save(uint score) {
		getInstance();

		auto now = static_cast<long long>(duration_cast<chrono::milliseconds>(high_resolution_clock::now().time_since_epoch()).count());

		if (addScore({now, score})) {
			ofstream f(SAVE_FILE_REL_PATH);
			for (auto& s: scores)
				f << s.timestamp << ',' << s.score << '\n';
			f.close();
		}
	}


	bool ScoreFile::addScore(score s) {
		getInstance();
		
		if (scores.size() >= 10 && s.score <= scores[scores.size() - 1].score)
			return false;

		if (scores.size() < 10)
			scores.push_back(s);
		else
			scores[scores.size() - 1] = s;

		sort(scores.begin(), scores.end(), CompareScores());

		return true;
	}

} // namespace io
