#pragma once
#ifndef _A1DENGINE_HPP_
#define _A1DENGINE_HPP_
#include <map>
extern std::map<char, unsigned int> letter_digival;
extern char digival_letter[];

extern unsigned int Nbrh_length;
extern unsigned int L_nbr_num;
extern unsigned int Alphabet_size;
extern unsigned int nbrh_digiweight[];
extern unsigned int Rule_length;
extern unsigned int Lattice_length;
extern unsigned int Gen_num;
extern unsigned int* mapping_list;
extern unsigned int* index_buf;
extern unsigned int** lattice_record;

extern char**       prev_run_depository[];


unsigned int* nbrh_head_specific(unsigned int* const&, unsigned int const&, unsigned int const&);

void ring_iter_incre(const unsigned int*& nbrh_iter, unsigned int* const& lattice_T, unsigned int* const& lattice_H);

void rfrsh_nbrh_indx(unsigned int* const& lst_gen_begin, unsigned int* const& index_buffer, \
	unsigned int* const& lattice_H, unsigned int* const& lattice_T);

void engine_run(unsigned int** const& lattice_record, unsigned int* const& index_buffer, \
	unsigned int const& lattice_length, unsigned int const& gen_num);

void evolution_run(unsigned int** const& lattice_record, unsigned int* const& index_buffer, \
	unsigned int const& lattice_length, unsigned int const& gen_num, const unsigned int evo_cycle);

void alloc_run_regi(char**& run_depository, unsigned int const& width, unsigned int const& hight);

void delete_run_regi(char**& run_depository, unsigned int const& width, unsigned int const& hight);

void deposit_run_record(unsigned int** const& run_record, char** const& run_depository, \
	unsigned int const& width, unsigned int const& hight);

void XOR_compare(char** const& run_depository_0, char** const& run_depository_1, \
	unsigned int** const& lattice_record, unsigned int const& width, unsigned int const& hight);







#endif // !_A1DENGINE_HPP_