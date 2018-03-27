/// 786

/******************************************************************************/

#pragma once 

/******************************************************************************/

#include <map>
#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <cstdlib>

#include "extern/format.h"

/******************************************************************************/

#define prn(f, ...)    fmt::print(f "\n", ##__VA_ARGS__)
#define prnn(...)      fmt::print(__VA_ARGS__)

#define eprn(f, ...)   fmt::print(stderr, f "\n",  ##__VA_ARGS__)
#define eprnn(...)     fmt::print(stderr, __VA_ARGS__)

#ifdef NDEBUG
#define dprn(f, ...)   ;
#define dprnn(...)     ;
#else
#define dprn(f, ...)   {if(getenv("SEDEFDBG"))fmt::print(stderr, f "\n",  ##__VA_ARGS__);}
#define dprnn(...)     {if(getenv("SEDEFDBG"))fmt::print(stderr, __VA_ARGS__);}
#endif

#define cur_time()     chrono::high_resolution_clock::now() 
#define elapsed(t)     (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - (t)).count() / 1000.00)

/******************************************************************************/

const int    KMER_SIZE      = 12;
const int    WINDOW_SIZE    = 16; // <-- Needs to be changed
static_assert(KMER_SIZE <= 16, "k-mer space is 32-bit");

const double MAX_ERROR      = 0.30;
const double MAX_EDIT_ERROR = 0.15;
const double ERROR_RATIO    = (MAX_ERROR - MAX_EDIT_ERROR) / MAX_EDIT_ERROR;
const double MAX_GAP_ERROR  = MAX_EDIT_ERROR * ERROR_RATIO;
const double GAP_FREQUENCY  = 0.005;
const int    MIN_READ_SIZE  = 1000 * (1 - MAX_ERROR);

const size_t KB = 1024;
const size_t MB = 1024 * KB;
const size_t GB = 1024 * MB;

/******************************************************************************/

extern bool do_uppercase;
extern bool do_uppercase_seeds;
extern bool do_qgram;

/******************************************************************************/

struct DNA {
	char val[128];
	constexpr DNA(int def): val()
	{
		for (int i = 0; i < 128; i++) val[i] = def;
		val['A'] = val['a'] = 0;
		val['C'] = val['c'] = 1;
		val['G'] = val['g'] = 2;
		val['T'] = val['t'] = 3;
	}
};
constexpr auto dna_hash_lookup  = DNA(0);
constexpr auto dna_align_lookup = DNA(4);

struct RDNA {
	char val[128];
	constexpr RDNA(): val()
	{
		for (int i = 0; i < 128; i++) val[i] = 'N';
		val['A'] = 'T'; val['a'] = 't';
		val['C'] = 'G'; val['c'] = 'g';
		val['G'] = 'C'; val['g'] = 'c';
		val['T'] = 'A'; val['t'] = 'a';
	}
};
constexpr auto rev_comp_lookup = RDNA();

inline char hash_dna(char c) 
{
	return dna_hash_lookup.val[c];
}

inline char align_dna(char c) 
{
	return dna_align_lookup.val[c];
}

inline char rev_dna(char c) 
{
	return rev_comp_lookup.val[c];
}

template<class X, class Y>
inline bool in_map(const X &m, Y k)
{
	return m.find(k) != m.end();
}

inline double pct(double p, double tot)
{
	return 100.0 * p / tot;
}

/******************************************************************************/

double tau(double edit_error, int kmer_size);

int relaxed_jaccard_estimate(int s, int kmer_size);

std::vector<std::string> split(const std::string &s, char delim);

std::string rc(const std::string &s);
