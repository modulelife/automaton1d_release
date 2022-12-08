#include <map>

std::map<char, unsigned int> letter_digival{
{ '0', 0 }, { '1',1 }, { '2',2 }, { '3',3 }, { '4',4 }, { '5',5 }, { '6',6 }, { '7',7 },
{ '8',8 }, { '9',9 }, { 'A',10 }, { 'B',11 }, { 'C',12 }, { 'D',13 }, { 'E',14 }, { 'F',15 },
{ 'G',16 }, { 'H',17 }, { 'I',18 }, { 'J',19 }, { 'K',20 }, { 'L',21 }, { 'M',22 }, { 'N',23 }
};

char digival_letter[24] =
{ '0', '1', '2', '3', '4', '5', '6', '7',
 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N' };

unsigned int Nbrh_length(0), L_nbr_num(0), Alphabet_size(0);
unsigned int nbrh_digiweight[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int Rule_length(0);
unsigned int Lattice_length(0),Gen_num(0);
unsigned int* mapping_list(nullptr);
unsigned int* index_buf(nullptr);
unsigned int** lattice_record(nullptr);

char** prev_run_depository[5]{ nullptr ,nullptr ,nullptr ,nullptr ,nullptr };



inline unsigned int* nbrh_head_specific(unsigned int* const& lst_gen_begin, unsigned int const& l_nbr_num, \
	unsigned int const& lattice_length) {
	unsigned int* nbrh_head = lst_gen_begin + (lattice_length - l_nbr_num) % lattice_length;
	return nbrh_head;
}

inline void ring_iter_incre(const unsigned int*& nbrh_iter, unsigned int* const& lattice_H, \
	unsigned int* const& lattice_T) {
	if (nbrh_iter == lattice_T)
		nbrh_iter = lattice_H;
	else
		++nbrh_iter;
}

inline void rfrsh_nbrh_indx(unsigned int* const& lst_gen_begin, unsigned int* const& index_buffer, \
	unsigned int* const& lattice_H, unsigned int* const& lattice_T)
{
	
	const unsigned int* Hnbr_iter(nullptr), * Tnbr_iter(nullptr);
	Hnbr_iter = Tnbr_iter = nbrh_head_specific(lst_gen_begin, L_nbr_num, Lattice_length);
	unsigned int* index_w_iter = index_buffer;
	unsigned int indx_temp(0),Hi_digival(nbrh_digiweight[0]),Lo_digival(nbrh_digiweight[Nbrh_length-1]);
	
	for (unsigned int i(0); i < Nbrh_length; i++) {
		indx_temp += (*Tnbr_iter) * nbrh_digiweight[i];
		ring_iter_incre(Tnbr_iter, lattice_H, lattice_T);
	}
    *index_w_iter = indx_temp;
	
	for (unsigned int j(0); j < Lattice_length - 1; j++) {
		indx_temp = (indx_temp - (*Hnbr_iter) * Hi_digival) * Alphabet_size + (*Tnbr_iter) * Lo_digival;
		ring_iter_incre(Tnbr_iter, lattice_H, lattice_T);
		ring_iter_incre(Hnbr_iter, lattice_H, lattice_T);
		*(++index_w_iter) = indx_temp;
	}

}

void engine_run(unsigned int** const& lattice_record, unsigned int* const& index_buffer, \
	unsigned int const& lattice_length, unsigned int const& gen_num)
{
	unsigned int Lst_gen_num(0);
	unsigned int Nxt_gen_num(1);
	unsigned int run_time(gen_num - 1);

	unsigned int* Lattice_H(nullptr);
	unsigned int* Lattice_T(nullptr);

	while (Lst_gen_num < run_time)
	{
		Lattice_H = lattice_record[Lst_gen_num];
		Lattice_T = Lattice_H + lattice_length - 1;
		rfrsh_nbrh_indx(Lattice_H, index_buffer, Lattice_H, Lattice_T);
		for (unsigned int i(0); i < lattice_length; i++) {
			lattice_record[Nxt_gen_num][i] = mapping_list[index_buffer[i]];
		}
		++Lst_gen_num;
		++Nxt_gen_num;
	}

}

void evolution_run(unsigned int** const& lattice_record, unsigned int* const& index_buffer, \
	unsigned int const& lattice_length, unsigned int const& gen_num, const unsigned int evo_cycle)
{
	unsigned int Lst_gen_num(0);
	unsigned int Nxt_gen_num(1);
	unsigned int run_time(gen_num - 1), evo_cntdown(1), rule_pos(0);

	unsigned int* Lattice_H(nullptr);
	unsigned int* Lattice_T(nullptr);

	while (Lst_gen_num < run_time)
	{
		if (evo_cntdown > evo_cycle) { rule_pos = Lst_gen_num; evo_cntdown = 1; }
		Lattice_H = lattice_record[Lst_gen_num];
		Lattice_T = Lattice_H + lattice_length - 1;
		rfrsh_nbrh_indx(Lattice_H, index_buffer, Lattice_H, Lattice_T);
		for (unsigned int i(0); i < lattice_length; i++) {
			lattice_record[Nxt_gen_num][i] = lattice_record[rule_pos][index_buffer[i]];
		}
		++evo_cntdown;
		++Lst_gen_num;
		++Nxt_gen_num;
	}

}

void alloc_run_regi(char**& run_depository, unsigned int const& width, unsigned int const& hight) {
	run_depository = new char* [hight];
	for (unsigned int i(0); i < hight; i++)
		run_depository[i] = new char[width]();
}

void delete_run_regi(char**& run_depository, unsigned int const& width, unsigned int const& hight){
	for (unsigned int i(0); i < hight; i++)
		delete[] run_depository[i];
	delete[] run_depository;
}

void deposit_run_record(unsigned int** const& run_record, char** const& run_depository, \
	unsigned int const& width, unsigned int const& hight)
{
	const unsigned int* lattice_r_iter(nullptr);
	char* deposi_w_iter(nullptr);

	for (unsigned int i(0); i < hight; i++) {
		lattice_r_iter = run_record[i];
		deposi_w_iter = run_depository[i];
		for (unsigned int j(0); j < width; j++){
			*deposi_w_iter = digival_letter[*lattice_r_iter];
			++lattice_r_iter;
			++deposi_w_iter;
		}
	}
}

void XOR_compare(char** const& run_depository_0, char** const& run_depository_1, \
	unsigned int** const& lattice_record, unsigned int const& width, unsigned int const& hight)
{
	const char* deposi_0_r_iter(nullptr);
	const char* deposi_1_r_iter(nullptr);
	unsigned int* lattice_w_iter(nullptr);

	for (unsigned int i(0); i < hight; i++) {
		deposi_0_r_iter = run_depository_0[i];
		deposi_1_r_iter = run_depository_1[i];
		lattice_w_iter = lattice_record[i];
		for (unsigned int j(0); j < width; j++) {
			if (*deposi_0_r_iter == *deposi_1_r_iter)
				*lattice_w_iter = 0;
			else
				*lattice_w_iter = 1;
			++deposi_0_r_iter;
			++deposi_1_r_iter;
			++lattice_w_iter;
		}
	}
}

