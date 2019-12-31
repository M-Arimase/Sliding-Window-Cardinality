#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>

#include "../BOBHash32.h"

template<uint8_t key_len>
struct Covering {
	int win, len;
	double clock, perpk;

	int *sketch, counter[255], hyper_counter;
	BOBHash32 *hash;

	public:
	string name;

	Covering(int mem_, int win_) : len(mem_ - 1000), win(win_) {
		perpk = 127.0 / win;

		sketch = new int[len];
		for (int i = 0; i < len; i++) {
			sketch[i] = -1;
		}

		memset(counter, 0, sizeof counter);

		random_device rd;
		hash = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
	}

	void insert(uint8_t *key) {
		clock += perpk;
		if (clock >= 255) {
			clock -= 255;
		}

		int idx = hash->run((char *)key, key_len) % len, cur = floor(clock);

		if (sketch[idx] != -1) {
			counter[sketch[idx]] -= 1;
		}
		sketch[idx] = cur;
		counter[cur] += 1;

		for (int point = (idx / 16 * 16); point < len && point < (idx / 16 * 16) + 16; point++) {
			if (sketch[point] == -1) {
				continue;
			}

			if (sketch[point] > cur && sketch[point] - cur <= 127) {
				counter[sketch[point]] -= 1;
				sketch[point] = -1;
				continue;
			}

			if (sketch[point] < cur && cur - sketch[point] >= 128) {
				counter[sketch[point]] -= 1;
				sketch[point] = -1;
				continue;
			}
		}
	}

	int card() {
		// int mu = 0, cur = floor(clock);

		// for (int point = 0; point < len; point++) {
		// 	if (sketch[point] == -1) {
		// 		mu += 1;
		// 		continue;
		// 	}

		// 	if (sketch[point] > cur && sketch[point] - cur <= 127) {
		// 		mu += 1;
		// 		continue;
		// 	}

		// 	if (sketch[point] < cur && cur - sketch[point] >= 128) {
		// 		mu += 1;
		// 		continue;
		// 	}
		// }
	
		int cur = floor(clock), mu = len;
		
		if (cur >= 127) {
			for (int point = cur - 127; point <= cur - 1; point++) {
				mu -= counter[point];
			}
		}
		else {
			for (int point = 0; point <= cur - 1; point++) {
				mu -= counter[point];
			}
			for (int point = cur + 128; point <= 254; point++) {
				mu -= counter[point];
			}
		}

		return -1.0 * len * log(1.0 * mu / len);
	}
};
