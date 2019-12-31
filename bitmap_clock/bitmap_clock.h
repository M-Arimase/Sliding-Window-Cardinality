#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>

#include "../BOBHash32.h"

template<uint8_t key_len>
struct Bitmap {
	int win, len, mu, point;
	double clock, perpk;

	int *bitmap, *clkmap;
	BOBHash32 *hash;

	public:
	string name;

	Bitmap(int mem_, int win_) : len(mem_ << 2), win(win_) {
		mu = len;

		perpk = 1.0 * len / win;
		clock = 0.0;
		point = 0;

		bitmap = new int[len];
		clkmap = new int[len];
		memset(bitmap, 0, sizeof(int) * len);
		memset(clkmap, 0, sizeof(int) * len);

		random_device rd;
		hash = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
	}

	void insert(uint8_t *key) {
		clock += perpk;
		for (point; point < clock; point++) {
			if (clkmap[point % len] == 0 && bitmap[point % len] == 1) {
				bitmap[point % len] = 0;
				mu += 1;
			}
			clkmap[point % len] = 0;
		}
		if (clock >= len) {
			clock -= len;
			point -= len;
		}

		int idx = hash->run((char *)key, key_len) % len;

		if (bitmap[idx] == 0) 
			mu -= 1;
		bitmap[idx] = 1;
		clkmap[idx] = 1;
	}

	int card() {
		return -1.0 * len * log(1.0 * mu / len);
	}
};
