#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>

#include "../BOBHash32.h"

template<uint8_t key_len>
struct Hashtable {
	int win, len, curr;

	int *fingtable, *timetable;
	BOBHash32 *hash, *fing;

	public:
	string name;

	Hashtable(int len_, int win_) : len(len_ >> 3), win(win_) {
		fingtable = new int[len];
		timetable = new int[len];
		memset(fingtable, 0, sizeof(int) * len);
		memset(timetable, 0, sizeof(int) * len);

		random_device rd;
		hash = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
		fing = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
	}

	void insert(uint8_t *key) {
		curr += 1;

		int idx = hash->run((char *)key, key_len) % len;
		int fin = fing->run((char *)key, key_len);

		for (int bias = 0; bias < 8; bias++) {
			int point = (idx + bias) % len;
			if (timetable[point] == 0 || timetable[point] <= curr - win || fingtable[point] == fin) {
				fingtable[point] = fin;
				timetable[point] = curr;
				break;
			}
		}
	}

	int card() {
		map<int, int> fingmap;
		for (int idx = 0; idx < len; idx++) {
			if (timetable[idx] != 0 && timetable[idx] > curr - win) {
				fingmap[fingtable[idx]] = 1;
			}
		}
		return fingmap.size();
	}
};
