#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

#include "bitmap_clock.h"

const int TRACE_LEN = 26;
const int TUPLE_LEN = 13;

const int win = 100 * 1000;
const int mem = 5   * 1000;

auto *bitmap = new Bitmap<13>(mem, win);
map<string, int> strmap;

string strrep[win];

int card1 = 0;

int main() {
	ifstream fin("../data/20.dat", ios::binary);
	ofstream fout("log");

	double are = 0, count = 0;

	uint8_t key[TRACE_LEN];
	for (int pkt = 0; pkt < 100 * win; pkt++) {
		fin.read((char *)key, sizeof(char) * TRACE_LEN);

		string tuple;
		for (int i = 0; i < TUPLE_LEN; i++) {
			tuple += (char)key[i];
		}

		if (pkt >= win) {
			strmap[strrep[pkt % win]] -= 1;
			if (strmap[strrep[pkt % win]] == 0) {
				card1 -= 1;
			}
		}

		strrep[pkt % win] = tuple;
		if (strmap[tuple] == 0) {
			card1 += 1;
		}
		strmap[tuple] += 1;

		bitmap->insert(key);

		if ((pkt + 1) % 10000 == 0) {
			int card2 = bitmap->card();

			fout << card1 << " " << card2 << " " << bitmap->clock << endl;
			are += 1.0 * abs(card1 - card2) / card1;
			count += 1;
			fout << "average relative error: " << are / count << endl;
		}
	}

	return 0;
}
