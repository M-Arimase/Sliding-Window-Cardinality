#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>

#include "../BOBHash32.h"

template<uint8_t key_len, int d = 6>
struct BloomFilter {
	int len, count;

	int *bloomfilter;
	BOBHash32 *hash[d];

	public:
	string name;

	BloomFilter(int mem_) : len(mem_ << 3) {
		count =0;

		bloomfilter = new int[len];
		memset(bloomfilter, 0, sizeof(int) * len);

		random_device rd;
		for (int i = 0; i < d; i++) {
			hash[i] = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
		}
	}

	void insert(uint8_t *key) {
		int exist = 1;
		for (int i = 0; i < d; i++) {
			int idx = hash[i]->run((char *)key, key_len) % len;
			if (bloomfilter[idx] == 0) {
				exist = 0;
			}
			bloomfilter[idx] = 1;
		}

		if (exist == 0) {
			count += 1;
		}
	}

	int card() {
		return count;
	}

	void clear() {
		memset(bloomfilter, 0, sizeof(int) * len);
		count = 0;
	}
};
