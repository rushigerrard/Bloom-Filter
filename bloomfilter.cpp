/*
To run the code: Please add the gcc 346 compiler: command: "add gcc346"
Then use the command: g++ bloomfilter.cpp -o output -g -w
Followed by: ./output

OR

use: add gcc346
Then: g++ rsghatpa_sagupta.cpp
Then: ./a.out
*/
//Hash Function: MurmurHash3 credit: Austin Appleby

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <bitset>
#include <cstdlib>
#include <stdint.h>

#define N 8

typedef struct bf_t {
	std::bitset<2000000> bloomSet;
}bf_t;

int prime[N] = { 319993, 331999, 391939, 393919, 919393, 933199, 939193, 939391 };

int MurmurHash3(const void *key, uint32_t len, uint32_t seed) {
	uint32_t c1 = 0xcc9e2d51;
	uint32_t c2 = 0x1b873593;
	uint32_t r1 = 15;
	uint32_t r2 = 13;
	uint32_t m = 5;
	uint32_t n = 0xe6546b64;
	uint32_t h = 0;
	uint32_t k = 0;
	uint8_t *d = (uint8_t *)key; // 32 bit extract from `key'
	const uint32_t *chunks = NULL;
	const uint8_t *tail = NULL; // tail - last 8 bytes
	int i = 0;
	int l = len / 4; // chunk length

	h = seed;

	chunks = (const uint32_t *)(d + l * 4); // body
	tail = (const uint8_t *)(d + l * 4); // last 8 byte chunk of `key'

										 // for each 4 byte chunk of `key'
	for (i = -l; i != 0; ++i) {
		// next 4 byte chunk of `key'
		k = chunks[i];

		// encode next 4 byte chunk of `key'
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;

		// append to hash
		h ^= k;
		h = (h << r2) | (h >> (32 - r2));
		h = h * m + n;
	}

	k = 0;

	// remainder
	switch (len & 3) { // `len % 4'
	case 3: k ^= (tail[2] << 16);
	case 2: k ^= (tail[1] << 8);

	case 1:
		k ^= tail[0];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;
		h ^= k;
	}

	h ^= len;

	h ^= (h >> 16);
	h *= 0x85ebca6b;
	h ^= (h >> 13);
	h *= 0xc2b2ae35;
	h ^= (h >> 16);

	return h%2000000;
}

//Bloom filter creation
bf_t * create_bf() {
	bf_t *bloom_filter = new bf_t[N];
	for (int i = 0; i<N; i++) {
		bloom_filter[i].bloomSet.reset();
    }
	return bloom_filter;
}

//insert the string in the filter *b
void insert_bf(bf_t *b, char *s) {
	int length = strlen(s);
	void *str = (void *)s;
	for (int i = 0; i<N; i++) {
		b[i].bloomSet.set(MurmurHash3(str, length, prime[i]));
	}

}

//check to see if the string already exists
int is_element(bf_t *b, char *s) {
	int length = strlen(s);
	void *str = (void *)s;
	for (int i = 0; i<N; i++) {
		if (!b[i].bloomSet.test(MurmurHash3(str, length, prime[i]))) {
			return 0;
		}
	}

	return 1;

}

void sample_string_A(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = (char)(1 + (((i / 254) / 254) % 254));
	s[3] = 'a';
	s[4] = 'b';
	s[5] = (char)(1 + ((i*(i - 3)) % 217));
	s[6] = (char)(1 + ((17 * i + 129) % 233));
	s[7] = '\0';
}
void sample_string_B(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = (char)(1 + (((i / 254) / 254) % 254));
	s[3] = 'a';
	s[4] = (char)(1 + ((i*(i - 3)) % 217));
	s[5] = (char)(1 + ((17 * i + 129) % 233));
	s[6] = '\0';
}
void sample_string_C(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = 'a';
	s[3] = (char)(1 + ((i*(i - 3)) % 217));
	s[4] = (char)(1 + ((17 * i + 129) % 233));
	s[5] = '\0';
}
void sample_string_D(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = (char)(1 + (((i / 254) / 254) % 254));
	s[3] = 'b';
	s[4] = 'b';
	s[5] = (char)(1 + ((i*(i - 3)) % 217));
	s[6] = (char)(1 + ((17 * i + 129) % 233));
	s[7] = '\0';
}
void sample_string_E(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = (char)(1 + (((i / 254) / 254) % 254));
	s[3] = 'a';
	s[4] = (char)(2 + ((i*(i - 3)) % 217));
	s[5] = (char)(1 + ((17 * i + 129) % 233));
	s[6] = '\0';
}



int main()
{
	long i, j;
	bf_t * bloom;
	bloom = create_bf();
	printf("Created Filter\n");
	for (i = 0; i< 1450000; i++)
	{
		char s[8];
		sample_string_A(s, i);
		insert_bf(bloom, s);
	}
	for (i = 0; i< 500000; i++)
	{
		char s[7];
		sample_string_B(s, i);
		insert_bf(bloom, s);
	}
	for (i = 0; i< 50000; i++)
	{
		char s[6];
		sample_string_C(s, i);
		insert_bf(bloom, s);
	}
	printf("inserted 2,000,000 strings of length 8,7,6.\n");

	for (i = 0; i< 1450000; i++)
	{
		char s[8];
		sample_string_A(s, i);
		if (is_element(bloom, s) != 1)
		{
			printf("found negative error (1)\n"); exit(0);
		}
	}
	for (i = 0; i< 500000; i++)
	{
		char s[7];
		sample_string_B(s, i);
		if (is_element(bloom, s) != 1)
		{
			printf("found negative error (2)\n"); exit(0);
		}
	}
	for (i = 0; i< 50000; i++)
	{
		char s[6];
		sample_string_C(s, i);
		if (is_element(bloom, s) != 1)
		{
			printf("found negative error (3)\n"); exit(0);
		}
	}
	j = 0;
	for (i = 0; i< 500000; i++)
	{
		char s[8];
		sample_string_D(s, i);
		if (is_element(bloom, s) != 0)
			j += 1;
	}
	for (i = 0; i< 500000; i++)
	{
		char s[7];
		sample_string_E(s, i);
		if (is_element(bloom, s) != 0)
			j += 1;
	}
	printf("Found %d positive errors out of 1,000,000 tests.\n", j);
	printf("Positive error rate %f\%.\n", (float)j / 10000.0);

}

