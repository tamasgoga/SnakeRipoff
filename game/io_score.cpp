#include "io_score.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iostream> // FOR TESTING PURPOSES

#include "../core/core_base.hpp"
#include "../core/core_error.hpp"

using namespace std;
using namespace std::chrono;


namespace io {


	const char* ScoreFile::SAVE_FILE_REL_PATH = "res/snake.save";


	ScoreFile::ScoreFile()
		: scores()
	{
		string timestamp, score;

		ifstream f(SAVE_FILE_REL_PATH);
		cout << "File open {\n";
		while (getline(f, timestamp, ',')) {
			getline(f, score);

			struct score s;
			istringstream timestamp_ss(timestamp), score_ss(score);
			timestamp_ss >> s.timestamp;
			score_ss >> s.score;

			scores.push_back(s);

			cout << '\t' << s.timestamp << ", " << s.score << endl;
		}
		cout << "}\n";
		f.close();
	}


	void ScoreFile::save(uint score) {
		auto now = static_cast<long long>(duration_cast<chrono::milliseconds>(high_resolution_clock::now().time_since_epoch()).count());
		ofstream f(SAVE_FILE_REL_PATH);
		f << now << ',' << score << '\n';
		f.close();
		cout << "Your final score @ " << now << ": " << score << endl;
	}

} // namespace io
