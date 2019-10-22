#include<iostream>
#include<string>
#include<math.h>
using namespace std;
int ip[64] = { 58,50,42,34,26,18,10,2,
			60,52,44,36,28,20,12,4,
			62,54,46,38,30,22,14,6,
			64,56,48,40,32,24,16,8,
			57,49,41,33,25,17,9,1,
			59,51,43,35,27,19,11,3,
			61,53,45,37,29,21,13,5,
			63,55,47,39,31,23,15,7 };

int inv_ip[64] = { 40,8,48,16,56,24,64,32,
				39,7,47,15,55,23,63,31,
				38,6,46,14,54,22,62,30,
				37,5,45,13,53,21,61,29,
				36,4,44,12,52,20,60,28,
				35,3,43,11,51,19,59,27,
				34,2,42,10,50,18,58,26,
				33,1,41,9,49,17,57,25 };

int pc_1[56] = { 57,49,41,33,25,17,9,1,
			58,50,42,34,26,18,10,2,
			59,51,43,35,27,19,11,3,
			60,52,44,36,63,55,47,39,
			31,23,15,7,62,54,46,38,
			30,22,14,6,61,53,45,37,
			29,21,13,5,28,20,12,4 };

int pc_2[48] = { 14,17,11,24,1,5,3,28,
				15,6,21,10,23,19,12,4,
				26,8,16,7,27,20,13,2,
				41,52,31,37,47,55,30,40,
				51,45,33,48,44,49,39,56,
				34,53,46,42,50,36,29,32 };

int E[48] = { 32,1,2,3,4,5,4,5,
			6,7,8,9,8,9,10,11,
			12,13,12,13,14,15,16,17,
			16,17,18,19,20,21,20,21,
			22,23,24,25,24,25,26,27,
			28,29,28,29,30,31,32,1 };

int s0[8][4][16] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
				0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
				4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
				15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
				15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
				3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
				0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
				13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
				10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
				13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
				13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
				1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
				7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
				13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
				10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
				3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
				2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
				14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
				4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
				11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
				12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
				10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
				9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
				4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
				4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
				13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
				1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
				6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
				13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
				1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
				7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
				2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 };

int fun_per[32] = { 16,7,20,21,29,12,28,17,
				1,15,23,26,5,18,31,10,
				2,8,24,14,32,27,3,9,
				19,13,30,6,22,11,4,25 };
void to_bin(string hex, int bin[]) {
	for (int i = 3; i < 64; i += 4) {
		int num;
		hex[i / 4] = toupper(hex[i / 4]);
		if (hex[i / 4] >= 'A')
			num = hex[i / 4] - 55;
		else
			num = hex[i / 4] - 48;
		for (int j = 0; j < 4; j++) {
			bin[i - j] = num % 2;
			num /= 2;
		}
	}
}

string to_hex(int bin[]) {
	string hex;
	for (int i = 3; i < 64; i += 4) {
		int num = 0;
		for (int j = 0; j < 4; j++)
			num += bin[i - j] * pow(2, j);
		if (num > 9)
			hex += (num + 55);
		else
			hex += (num + 48);
	}
	return hex;
}

//0xafafafafafafafaf 0xabcdef0123456789
int main(int argc, char *argv[]) {
	string input_key, input_cipher;//輸入key和cipher
	input_key = argv[1];
	input_cipher = argv[2];

	int key[64], cipher[64];
	input_key.erase(0, 2);//將0x刪除
	input_cipher.erase(0, 2);//將0x刪除

	to_bin(input_key, key);//把key的hex轉成bin
	to_bin(input_cipher, cipher);//把cipher的hex轉成bin
	int cipher_p[64], key_p[56];//Permutation後的key和cipher

	for (int i = 0; i < 64; i++)
		cipher_p[i] = cipher[ip[i] - 1];//Permutation 

	for (int i = 0; i < 56; i++)
		key_p[i] = key[pc_1[i] - 1];//Permutation 64->56

	int c[28], d[28];
	for (int i = 0; i < 28; i++)
		c[i] = key_p[i], d[i] = key_p[i + 28];//將key分兩半 56->28,28

	int r[32], l[32];
	for (int i = 0; i < 32; i++)
		l[i] = cipher_p[i], r[i] = cipher_p[i + 32];//將cipher分兩半 64->32,32

	int key_16[16][48];

	for (int round = 0; round < 16; round++)
	{
		if (round == 0 || round == 1 || round == 8 || round == 15) {//向右位移1位
			int temp_c = c[0], temp_d = d[0];
			for (int i = 0; i <= 26; i++)
				c[i] = c[i + 1], d[i] = d[i + 1];
			c[27] = temp_c, d[27] = temp_d;
		}
		else {//向右位移2位
			int temp_c1 = c[0], temp_d1 = d[0], temp_c2 = c[1], temp_d2 = d[1];
			for (int i = 0; i <= 25; i++)
				c[i] = c[i + 2], d[i] = d[i + 2];
			c[27] = temp_c2, d[27] = temp_d2;
			c[26] = temp_c1, d[26] = temp_d1;
		}
		for (int i = 0; i < 28; i++)//combin c and d
			key_p[i] = c[i], key_p[i + 28] = d[i];

		for (int i = 0; i < 48; i++)//key 56->48
			key_16[round][i] = key_p[pc_2[i] - 1];
	}
	for (int round = 0; round < 16; round++) {
		int r_expan[48];
		for (int i = 0; i < 48; i++) //r 32->48
			r_expan[i] = r[E[i] - 1];

		int fun_xor[48];
		for (int i = 0; i < 48; i++)//xor
			fun_xor[i] = key_16[15 - round][i] ^ r_expan[i];

		int fun_s[32];//substitution s1~s8
		for (int i = 5, s_count = 3; i < 48; i += 6, s_count += 4) {
			int row = fun_xor[i] + fun_xor[i - 5] * 2;
			int col = fun_xor[i - 4] * 8 + fun_xor[i - 3] * 4 + fun_xor[i - 2] * 2 + fun_xor[i - 1];
			int num = s0[s_count / 4][row][col];
			for (int j = 0; j < 4; j++) {
				fun_s[s_count - j] = num % 2;
				num /= 2;
			}
		}

		int fun_p[32];
		for (int i = 0; i < 32; i++)
			fun_p[i] = fun_s[fun_per[i] - 1];

		int r_temp[32];
		for (int i = 0; i < 32; i++)
			r_temp[i] = r[i];

		for (int i = 0; i < 32; i++)
			r[i] = l[i] ^ fun_p[i];

		for (int i = 0; i < 32; i++)
			l[i] = r_temp[i];
	}

	int r_temp[32];
	for (int i = 0; i < 32; i++)
		r_temp[i] = r[i];

	for (int i = 0; i < 32; i++)
		r[i] = l[i];

	for (int i = 0; i < 32; i++)
		l[i] = r_temp[i];

	for (int i = 0; i < 32; i++)//combin c and d
		cipher_p[i] = l[i], cipher_p[i + 32] = r[i];

	int cipher_p_1[64];
	for (int i = 0; i < 64; i++)
		cipher_p_1[i] = cipher_p[inv_ip[i] - 1];

	string plaintext= to_hex(cipher_p_1);
	for (int i = 0; i < plaintext.size(); i++)
		plaintext[i] = tolower(plaintext[i]);
	cout << "0x" << plaintext << "\n";
}