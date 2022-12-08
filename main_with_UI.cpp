//函数名小写，变量首字母大写，形参名称小写
//所有数位运算统一左高位

#include <iostream>
#include <fstream>
#include <string>
#include "A1Dengine.hpp"                   //已包含<map>
#include "A1Dlibrary.hpp"
#include "BMPprocess.hpp"                  //已包含"windows.h"

using namespace std;

//UIfunction declarations
void msg(const char* message);
void info(const char* infomation);
void err(const char* error_message);
void set_value(unsigned int& parameter);
void load_into_str(const char* path,string& strname);
//void lattice_copy(const unsigned int* src, unsigned int* dst, unsigned int len);
unsigned int int_pow(unsigned int& base, unsigned int& exp);
void str_to_ary(string const& str, unsigned int* const& ary, unsigned int const& length);
string ary_to_str(unsigned int* const& ary, unsigned int const& length);
void set_clr(RGBTRIPLE& clr);
int count_invalid_letter(string const& str);
string rule_input(""), lattice_init("");


char lattice_ini_r_path[] = "D:\\RunningData\\automaton1d\\initialize\\lattice_initial.txt";
char rule_ini_r_path[] = "D:\\RunningData\\automaton1d\\initialize\\rule_input.txt";
char autfile_w_path[] = "D:\\RunningData\\automaton1d\\debug\\automata.bmp";
char XORfile_w_path[] = "D:\\RunningData\\automaton1d\\debug\\XOR.bmp";
char evofile_w_path[] = "D:\\RunningData\\automaton1d\\debug\\evoluton.bmp";
/*D:\\RunningData\\automaton1d*/

int main()
{
    
    
    cout << "\n--------------------- Automaton1d ver1.5.6 ---------------------\n\n";
    //cout << "    -+-+-+-+-+-+-+-+-+- user:  +-+-+-+-+-+-+-+-+- " << endl << endl;


    automaton_init :
    //设置自动机参数：邻域长度，左邻居数，字母表大小，迭代次数
    //初始化邻域-位权重表，计算邻域组态数（规则长度），
    msg("to initialize a automaton please set following parameters\n  all the parameter should be positive integer!\n\n  recommended configuration: \n");
    msg("Nbrh_length = 3, Alphabet_size range [2,24]\n  Nbrh_length = 4, Alphabet_size range [2,11]\n  Nbrh_length = 5, Alphabet_size range [2,7]\n  Nbrh_length = 6, Alphabet_size range [2,5]\n  Nbrh_length = 7, Alphabet_size range [2,4]\n  Nbrh_length = 8 or 9, Alphabet_size = 2 or 3\n");
    msg("\n  set neighborhood length : ");
    set_value(Nbrh_length);
    msg("set left neighbor number : ");
    set_value(L_nbr_num);
    msg("set alphabet size : ");
    set_value(Alphabet_size);
    msg("set generation number : ");
    set_value(Gen_num);
    {
        unsigned int Digiweight = 1;
        unsigned int max_index = Nbrh_length - 1;
        for (unsigned int i(0); i < Nbrh_length; i++) {
            nbrh_digiweight[max_index - i] = Digiweight;
            Digiweight *= Alphabet_size;
        }
    }
    Rule_length = int_pow(Alphabet_size, Nbrh_length);
    //分配数组mapping_list
    
    try {
        mapping_list = new unsigned int[Rule_length]();
    }
    catch (bad_alloc) {
        err("out of memory , mapping_list initialization failed");
        exit(1);
    }
    
    
    //set_run_clr_table :
    //此处设置运行结果调色板
    msg("set color table with hex RGB code :\n");
    for (unsigned int i(0); i < Alphabet_size; i++) {
        cout << "               " << digival_letter[i] << " color : ";
        set_clr(run_clr_table[i]);
    }



    lattice_initialize :
    //加载lattice第一代，统计长度
    load_into_str(lattice_ini_r_path, lattice_init);
    msg("load first lattice generation from file : \"lattice_initial.txt\" \n"); 
    {
        int invalid_letter_count = count_invalid_letter(lattice_init);
        if (invalid_letter_count) {
            err("input contains invalid letter");
            
            enter_option_code_2:
            int option_2(0);
            msg("re edit \" lattice_initial.txt \" manually ? enter : 1 when done\n");
            cin >> dec >> option_2;
            switch (option_2) {
            case 1:goto lattice_initialize;
            default: err("invalid option code"); goto enter_option_code_2;
            }
        }
    }
    
    Lattice_length = lattice_init.size();
    cout << "-- lattice length is set to " << dec << Lattice_length << " cells" << endl;
    
    //engine preparation
    //分配二维数组lattice——record

    try {
        index_buf = new unsigned int[Lattice_length]();
        lattice_record = new unsigned int* [Gen_num]();
        for (unsigned int i(0); i < Gen_num; i++) {
            lattice_record[i] = new unsigned int[Lattice_length]();
        }
    }
    catch (bad_alloc) {
        err("out of memory , lattice_record initialization failed");
        exit(1);
    }
    //转换lattice输入至可运行格式
    str_to_ary(lattice_init, lattice_record[0], Lattice_length);



    load_rule :
    //加载规则，判断长度是否吻合
    load_into_str(rule_ini_r_path, rule_input);
    msg("load rule from file : \"rule_input.txt\" \n");
    {
        int invalid_letter_count = count_invalid_letter(rule_input);
        if (invalid_letter_count) {
            err("input contains invalid letter");

            enter_option_code_3:
            int option_3(0);
            msg("re edit \" rule_input.txt \" manually ? enter : 1 when done\n");
            cin >> dec >> option_3;
            switch (option_3) {
            case 1:goto load_rule;
            default: err("invalid option code"); goto enter_option_code_3;
            }
        }
    }
    if (Rule_length != rule_input.size()) {
        cout << "-- rule length does not match calculated value which is " << Rule_length << endl;
        msg("re edit \"rule_input.txt\" manually ? enter : 1 when done , 2 to quit ,\n");
        int option_0(0);
        enter_option_code_0:
        cin >> dec >> option_0;
        switch (option_0) {
        case 1:goto load_rule; break;
        case 2:goto before_quit; break;
        default:err("invalid option code"); goto enter_option_code_0;
        }
    }
    //转换rule输入至可运行格式
    str_to_ary(rule_input, mapping_list, Rule_length);

    //engine running
    engine_run(lattice_record, index_buf, Lattice_length, Gen_num);

    //BMP buffer & file out
    {

        write_bmp(autfile_w_path, lattice_record, Lattice_length, Gen_num, run_clr_table);
        info("image output completed\n");

    }

    after_first_run_options: //a.k.a. control flow
    {
        msg("enter : 1 to reset rule , 2 to reset lattice ,\n          3 to reset automaton , 4 to quit ,\n          5 to XOR test , 6 to continue\n          7 to run evoluta\n");
        int option_1(0);
    enter_option_code_1:
        cin >> dec >> option_1;
        switch (option_1) {
        case 1:
            goto load_rule; break;
        case 2:
            delete[] index_buf;
            for (unsigned int i(0); i < Gen_num; i++)
                delete[] lattice_record[i];
            delete[] lattice_record;
            goto lattice_initialize; break;
        case 3:
            delete[] mapping_list;
            delete[] index_buf;
            for (unsigned int i(0); i < Gen_num; i++)
                delete[] lattice_record[i];
            delete[] lattice_record;
            goto automaton_init; break;
        case 4:
            goto before_quit; break;
        case 5: {
            alloc_run_regi(prev_run_depository[4], Lattice_length, Gen_num);
            alloc_run_regi(prev_run_depository[0], Lattice_length, Gen_num);
            deposit_run_record(lattice_record, prev_run_depository[4], Lattice_length, Gen_num);
            int mut_pos = Lattice_length % 2 == 0 ? (Lattice_length / 2) : ((Lattice_length - 1) / 2);
            unsigned int mut_val(0);
            cout << "  latticce central position mutate " << dec << lattice_record[0][mut_pos] << " to ";
            set_value(mut_val);
            lattice_record[0][mut_pos] = mut_val;
            engine_run(lattice_record, index_buf, Lattice_length, Gen_num);
            deposit_run_record(lattice_record, prev_run_depository[0], Lattice_length, Gen_num);
            XOR_compare(prev_run_depository[4], prev_run_depository[0], lattice_record, Lattice_length, Gen_num);
            write_bmp(XORfile_w_path, lattice_record, Lattice_length, Gen_num, xor_clr_table);
            info("image output completed\n");
            delete_run_regi(prev_run_depository[4], Lattice_length, Gen_num);
            delete_run_regi(prev_run_depository[0], Lattice_length, Gen_num);
            str_to_ary(lattice_init, lattice_record[0], Lattice_length);
            engine_run(lattice_record, index_buf, Lattice_length, Gen_num);
            goto after_first_run_options; break;
        }
        case 6: {
            string lattice_end = ary_to_str(lattice_record[Gen_num - 1], Lattice_length);
            str_to_ary(lattice_end, lattice_record[0], Lattice_length);
            engine_run(lattice_record, index_buf, Lattice_length, Gen_num);
            write_bmp(autfile_w_path, lattice_record, Lattice_length, Gen_num, run_clr_table);
            info("image output completed\n");
            goto enter_option_code_1; break;
        }
        case 7:
            if (Lattice_length >= Rule_length) {
                unsigned int evo_cycle(1);
                msg("set evolution cycle : ");
                set_value(evo_cycle);
                evolution_run(lattice_record, index_buf, Lattice_length, Gen_num, evo_cycle);
                write_bmp(evofile_w_path, lattice_record, Lattice_length, Gen_num, run_clr_table, 10);
                info("image output completed\n");
                goto after_first_run_options; break;
            }
            else {
                err("lattice length must >= rule length");
                goto after_first_run_options; break;
            }
        default:err("invalid option code"); goto enter_option_code_1;
        }
    }

    before_quit:
    //before-quit cleanings
    delete[] mapping_list;
    delete[] index_buf;
    for (unsigned int i(0); i < Gen_num; i++)
        delete[] lattice_record[i];
    delete[] lattice_record;
    exit(0);
}


//UIfunctions

inline void msg(const char* message) {
    cout << "  " << message;
}//缩进两格打印消息

inline void info(const char* infomation) {
    cout << "[info] " << infomation << endl;
};

inline void err(const char* error_message) {
    cerr << "[error] " << error_message << endl;
};

inline void set_value(unsigned int& parameter) {
    cin >> dec >> parameter;
}

void load_into_str(const char* path,string& strname) {
    ifstream f;
    f.open(path, ios::in);
    if (!f.is_open()) {
        cerr << "#error : loading failed\n";
        exit(1);
    }
    getline(f, strname);
    f.close();
}

/*
void lattice_copy(const unsigned int* src, unsigned int* dst, unsigned int len) {
    for (int i(0); i < len; i++) {

    }
}
*/

unsigned int int_pow(unsigned int& base, unsigned int& exp) {
    unsigned int result(1);
    for (unsigned int i(0); i < exp; i++) {
        result *= base;
    }
    return result;
}

void str_to_ary(string const& str, unsigned int* const& ary, unsigned int const& length) {
    unsigned int* ary_iter(ary);
    for (unsigned int i(0); i < length; i++) {
        *ary_iter = letter_digival.at(str[i]);
        ++ary_iter;
    }
}

string ary_to_str(unsigned int* const& ary, unsigned int const& length) {
    string str("");
    const unsigned int* ary_iter(ary);
    char ch;
    for (unsigned int i(0); i < length; i++) {
        ch = digival_letter[*ary_iter];
        ++ary_iter;
        str += ch;
    }
    return str;
}

void set_clr(RGBTRIPLE& clr) {
    unsigned int hex_rgb_in(0);
    BYTE temp = 0;
    cin >> hex >> hex_rgb_in;
    clr.rgbtBlue = hex_rgb_in | temp;
    clr.rgbtGreen = (hex_rgb_in >> 8) | temp;
    clr.rgbtRed = (hex_rgb_in >> 16) | temp;
}

int count_invalid_letter(string const& str) {
    int letter_count[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    int length = str.size();
    int err_count(0);
    
    for (int i(0); i < length; i++)
        ++letter_count[letter_digival.at(str[i])];

    for (int i(Alphabet_size); i < 24; i++)
        err_count += letter_count[i];

    return err_count;
}


