/// 786

/******************************************************************************/

#pragma once 

/******************************************************************************/

#include <map>
#include <vector>
#include <string>
#include <functional>

#include "extern/format.h"

/******************************************************************************/

#define prn(f, ...)    fmt::print(f "\n", ##__VA_ARGS__)
#define prnn(...)      fmt::print(__VA_ARGS__)

#define eprn(f, ...)   fmt::print(stderr, f "\n",  ##__VA_ARGS__)
#define eprnn(...)     fmt::print(stderr, __VA_ARGS__)

/******************************************************************************/

const int    KMER_SIZE      = 14;
const int    WINDOW_SIZE    = 16; // <-- Needs to be changed
static_assert(KMER_SIZE <= 16, "k-mer space is 32-bit");

const int    MIN_READ_SIZE  = 1000;
const double MAX_EDIT_ERROR = 0.15;
const double ERROR_RATIO    = (0.25 - MAX_EDIT_ERROR) / MAX_EDIT_ERROR;
const double MAX_GAP_ERROR  = MAX_EDIT_ERROR * ERROR_RATIO;
const double GAP_FREQUENCY  = 0.005;

/******************************************************************************/

struct pair_hash {
	template<class T1, class T2>
	size_t operator () (const std::pair<T1, T2> &p) const {
		return std::hash<T2>{}(p.second);
	}
};

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

double tau(double edit_error = MAX_EDIT_ERROR);

int relaxed_jaccard_estimate(int s);

std::vector<std::string> split(const std::string &s, char delim);

std::string rc(const std::string &s);
