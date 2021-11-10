#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;
   
int PC1[] = {
    57, 49, 41, 33, 25, 17, 9, 
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4, 
};


int initial_permutation[] = { 
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
};

int d_box_table[] = { 
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1,
};

int s_box_table[8][4][16] =  { 
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

int straight_permutation_table[] = { 
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25,
};

int final_permutation_table[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25,
};

int shift_table[] = { 
    1, 1, 2, 2,
    2, 2, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 1,
};

int key_compression_table[] = { 
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32,
};

char bin2hex_util(string bin){
    if(bin == "0000"){
        return '0';
    }
    if(bin == "0001"){
        return '1';
    }
    if(bin == "0010"){
        return '2';
    }
    if(bin == "0011"){
        return '3';
    }
    if(bin == "0100"){
        return '4';
    }
    if(bin == "0101"){
        return '5';
    }
    if(bin == "0110"){
        return '6';
    }
    if(bin == "0111"){
        return '7';
    }
    if(bin == "1000"){
        return '8';
    }
    if(bin == "1001"){
        return '9';
    }
    if(bin == "1010"){
        return 'A';
    }
    if(bin == "1011"){
        return 'B';
    }
    if(bin == "1100"){
        return 'C';
    }
    if(bin == "1101"){
        return 'D';
    }
    if(bin == "1110"){
        return 'E';
    }
    return 'F';
}

string bin2hex(string bin){
    string hex = "";
    for (int i = 0; i < bin.length(); i += 4) {
        string ch = "";
        ch += bin[i];
        ch += bin[i + 1];
        ch += bin[i + 2];
        ch += bin[i + 3];
        hex += bin2hex_util(ch);
    }
    return hex;
}

string hex2bin(char hex){
    hex = toupper(hex);
    switch(hex){
        case '0':
            return "0000";
        case '1':
            return "0001";
        case '2':
            return "0010";
        case '3':
            return "0011";
        case '4':
            return "0100";
        case '5':
            return "0101";
        case '6':
            return "0110";
        case '7':
            return "0111";
        case '8':
            return "1000";
        case '9':
            return "1001";
        case 'A':
            return "1010";
        case 'B':
            return "1011";
        case 'C':
            return "1100";
        case 'D':
            return "1101";
        case 'E':
            return "1110";
        case 'F':
            return "1111";   
    }
}

string hex2bin(string hex){
    string bin;
    for(char hex_char: hex){
        bin += hex2bin(hex_char);
    }
    return bin;
}

string string_xor(string x1, string x2){
    string ans = "";
    for (int i = 0; i < x1.size(); i++) {
        if (x1[i] == x2[i]) {
            ans += '0';
        }
        else {
            ans += '1';
        }
    }
    return ans;
}

string shift_left(string bin, int k){
    string right = "";
    string ans = "";
    for(int i = 0; i < k; i++)
        right += bin[i];
    for(int i = k; i < bin.length(); i++)
        ans += bin[i];
    for(int i = 0; i < k; i++)
        ans += right[i];
    return ans;
}

string permute(string key, int permutation_combination[], int size){
    string ans = "";
    for(int i = 0; i < size; i++)
        ans += 'X';
    for(int i = 0; i < size; i++){
        ans[i] = key[permutation_combination[i] - 1];
    }
    return ans;
}

vector<string> generate_keys(string key){
    vector<string> ans;
    string L = key.substr(0, 28);
    string R = key.substr(28, 28);
 
    for (int i = 0; i < 16; i++) {
        L = shift_left(L, shift_table[i]);
        R = shift_left(R, shift_table[i]);
 
        string round_key = L + R;
 
        round_key = permute(round_key, key_compression_table, sizeof(key_compression_table) / sizeof(key_compression_table[0]));
 
        ans.push_back(round_key);
    }
    
    return ans;
}

// Help from the internet
string s_box_lookup(string x){
    string ans = "";
    for (int i = 0; i < 8; i++) {
        int row = 2 * int(x[i * 6] - '0') + int(x[i * 6 + 5] - '0');
        int col = 8 * int(x[i * 6 + 1] - '0') + 4 * int(x[i * 6 + 2] - '0') + 2 * int(x[i * 6 + 3] - '0') + int(x[i * 6 + 4] - '0');
        int val = s_box_table[i][row][col];
        ans += char(val / 8 + '0');
        val = val % 8;
        ans += char(val / 4 + '0');
        val = val % 4;
        ans += char(val / 2 + '0');
        val = val % 2;
        ans += char(val + '0');
    }
    return ans;
}

string encrypt(string message, string key, vector<string>& round_keys){
    message = permute(message, initial_permutation, sizeof(initial_permutation) / sizeof(initial_permutation[0]));
    string L = message.substr(0, 32);
    string R = message.substr(32, 32);
    
    for(int i = 0; i < 16; i++){
        string right_expanded = permute(R, d_box_table, sizeof(d_box_table) / sizeof(d_box_table[0]));

        string round_key = round_keys[i];

        string x = string_xor(round_key, right_expanded);
        
        string y = s_box_lookup(x);

        y = permute(y, straight_permutation_table, sizeof(straight_permutation_table) / sizeof(straight_permutation_table[0]));
         
        x = string_xor(y, L);
        L = x;
        if (i != 15) {
            swap(L, R);
        }
    }

    string result = L + R;
  
    result = permute(result, final_permutation_table, sizeof(final_permutation_table) / sizeof(final_permutation_table[0]));
    
    result = bin2hex(result);
    
    return result;
}

string decrypt(string cipher, string key, vector<string>& round_keys){
    reverse(round_keys.begin(), round_keys.end());
    return encrypt(cipher, key, round_keys);
}

int main(int argc, char* argv[]){
    
    if(argc == 1){
        cout << "Enter command\n";
        return 0;
    }

    if(strcmp(argv[1], "encrypt") == 0){
        if(argc != 4){
            cout << "Enter correct number of arguments\n";
            return 0;
        }
        string message = argv[2];
        string key = argv[3];

        message = hex2bin(message);
        key = permute(hex2bin(key), PC1, sizeof(PC1) / sizeof(PC1[0]));
        vector<string> round_keys = generate_keys(key);
        cout << encrypt(message, key, round_keys) << '\n';
    }

    else if(strcmp(argv[1], "decrypt") == 0){
        if(argc != 4){
            cout << "Enter correct number of arguments\n";
            return 0;
        }
        string cipher = argv[2];
        string key = argv[3];

        cipher = hex2bin(cipher);
        key = permute(hex2bin(key), PC1, sizeof(PC1) / sizeof(PC1[0]));
        vector<string> round_keys = generate_keys(key);
        cout << decrypt(cipher, key, round_keys) << '\n';
    }

    else{
        cout << "Unknown command\n";
    }
}
