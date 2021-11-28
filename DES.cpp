#include <iostream>
#include <iomanip>
#include <cstdint>
#include <bitset>
#ifdef __GNUC__
#define __rdtsc __builtin_ia32_rdtsc
#else
#include <intrin.h>
#endif

using namespace std;

int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9, 
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4, 
};


int initial_permutation[64] = { 
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
};

int d_box_table[48] = { 
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1,
};

int s_box_table[8][64] =  { 
    { 
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
    },
    {   
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
    },
    { 
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
    },
    { 
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
    },
    { 
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
    },
    { 
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
    },
    { 
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
    },
    { 
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11,
    } 
};

int straight_permutation_table[32] = { 
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25,
};

int final_permutation_table[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25,
};

int shift_table[16] = { 
    1, 1, 2, 2,
    2, 2, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 1,
};

int key_compression_table[48] = { 
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32,
};

void reverse(uint64_t* x, int size){
    for(int i = 0; i < size / 2; i++){
        uint64_t temp = x[i];
        x[i] = x[size - i - 1];
        x[size - i -1] = temp;
    }
}

uint64_t read_from_input(char* input, uint8_t size){
    uint64_t ret = 0;
    for(;;++input){
        uint8_t dec = *input - '0';
        if(dec < 10)
            ret = ret << 4 | dec;
        else{
            uint8_t upper=(*input & 0xDF) - 'A';
            if(upper > 5)
                break;
            ret = ret << 4 | upper + 10;
        }
    }
    return ret;
}

uint64_t permute(uint64_t x, int* permutation, uint8_t size, uint8_t bit_num){
    uint64_t ans = 0;
    for(uint8_t i = 0; i < size; i++){
        bool bit = ((uint64_t(1) << (bit_num - permutation[i]) & x) != 0);
        ans = ans | (uint64_t(bit) << (size - i - 1));
    }
    return ans;
}

uint64_t* generate_keys(uint64_t key){
    uint64_t* ans = new uint64_t[16];
    uint32_t L = (key >> 28) & 0x000000000FFFFFFF;
    uint32_t R = key & 0x000000000FFFFFFF;
    for (uint8_t i = 0; i < 16; i++) {
        if(shift_table[i] == 1){
            L = (0x0FFFFFFF & (L << 1)) | ((L & 0x08000000) >> 27);
            R = (0x0FFFFFFF & (R << 1)) | ((R & 0x08000000) >> 27);
        }
        else{
            L = (0x0FFFFFFF & (L << 2)) | ((L & 0x0C000000) >> 26);
            R = (0x0FFFFFFF & (R << 2)) | ((R & 0x0C000000) >> 26);
        }
        uint64_t round_key = (uint64_t(L) << 28) | uint64_t(R);
 
        round_key = permute(round_key, key_compression_table, sizeof(key_compression_table) / sizeof(key_compression_table[0]), 56);
 
        ans[i] = round_key;
    }
    return ans;
}

uint32_t s_box_lookup(uint64_t x){
    uint8_t row, column;
    uint32_t ans;
    for(uint8_t i = 0; i < 8; i++){
        row = (uint8_t) ((x & (0x0000840000000000 >> 6 * i)) >> 42 - 6 * i);
        row = (row >> 4) | row & 0x01;
        
        column = (uint8_t) ((x & (0x0000780000000000 >> 6 * i)) >> 43 - 6 * i);
        
        ans <<= 4;
        ans |= (uint32_t) (s_box_table[i][16 * row + column] & 0x0f);
    }
    return ans;
}

uint64_t encrypt(uint64_t message, uint64_t key, uint64_t* round_keys){
    message = permute(message, initial_permutation, sizeof(initial_permutation) / sizeof(initial_permutation[0]), 64);
    uint32_t L = (message >> 32) & 0x00000000FFFFFFFF;
    uint32_t R = message & 0x00000000FFFFFFFF;
    
    for(uint8_t i = 0; i < 16; i++){
        uint64_t right_expanded = permute(R, d_box_table, sizeof(d_box_table) / sizeof(d_box_table[0]), 32);
        uint64_t round_key = round_keys[i];
        uint64_t x = round_key ^ right_expanded;
        uint32_t y = s_box_lookup(x);
        y = permute(y, straight_permutation_table, sizeof(straight_permutation_table) / sizeof(straight_permutation_table[0]), 32);
        x = y ^ L;
        L = x;
        if (i != 15) {
            uint32_t temp = L;
            L = R;
            R = temp;
        }
    }

    uint64_t result = (uint64_t(L) << 32) | uint64_t(R);
    result = permute(result, final_permutation_table, sizeof(final_permutation_table) / sizeof(final_permutation_table[0]), 64);
    return result;
}

uint64_t decrypt(uint64_t cipher, uint64_t key, uint64_t* round_keys){
    reverse(round_keys, 16);
    return encrypt(cipher, key, round_keys);
}

int main(int argc, char** argv){
    if(argc == 1){
        cout << "Enter command\n";
        return 0;
    }

    if(string(argv[1]) == "encrypt"){
        if(argc != 4){
            cout << "Enter correct number of arguments\n";
            return 0;
        }
        try{
            long long t1 = __rdtsc();

            uint64_t message = read_from_input(argv[2], 16);
            uint64_t key = read_from_input(argv[3], 16);

            key = permute(key, PC1, sizeof(PC1) / sizeof(PC1[0]), 64);
            uint64_t* keys = generate_keys(key);
            uint64_t cipher = encrypt(message, key, keys);
            
            long long t2 = __rdtsc();
            
            cout << uppercase << hex << cipher << '\n';
            cout << dec << "Cycles: " << t2-t1 << '\n';
        }
        catch(const char* msg){
            cout << msg << '\n';
            return 0;
        }
    }

    else if(string(argv[1]) == "decrypt"){
        if(argc != 4){
            cout << "Enter correct number of arguments\n";
            return 0;
        }
        try{
            long long t1 = __rdtsc();

            uint64_t cipher = read_from_input(argv[2], 16);
            uint64_t key = read_from_input(argv[3], 16);

            key = permute(key, PC1, sizeof(PC1) / sizeof(PC1[0]), 64);
            uint64_t* keys = generate_keys(key);
            uint64_t message = decrypt(cipher, key, keys);
            
            long long t2 = __rdtsc();
            
            cout << uppercase << hex << message << '\n';
            cout << dec << "Cycles: " << t2-t1 << '\n';
        }
        catch(const char* msg){
            cout << msg << '\n';
            return 0;
        }
    }

    else{
        cout << "Unknown command\n";
    }
}
