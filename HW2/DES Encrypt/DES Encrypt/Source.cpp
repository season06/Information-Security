#include<iostream>
#include<vector>
#include<string>
#include<sstream>
using namespace std;
// initial permutation
int IP[] = { 58, 50, 42, 34, 26, 18, 10, 2,
			 60, 52, 44, 36, 28, 20, 12, 4,
			 62, 54, 46, 38, 30, 22, 14, 6,
			 64, 56, 48, 40, 32, 24, 16, 8,
			 57, 49, 41, 33, 25, 17,  9, 1,
			 59, 51, 43, 35, 27, 19, 11, 3,
			 61, 53, 45, 37, 29, 21, 13, 5,
			 63, 55, 47, 39, 31, 23, 15, 7 };
// final permutation
int IP_1[] = { 40, 8, 48, 16, 56, 24, 64, 32,
			   39, 7, 47, 15, 55, 23, 63, 31,
			   38, 6, 46, 14, 54, 22, 62, 30,
			   37, 5, 45, 13, 53, 21, 61, 29,
			   36, 4, 44, 12, 52, 20, 60, 28,
			   35, 3, 43, 11, 51, 19, 59, 27,
			   34, 2, 42, 10, 50, 18, 58, 26,
			   33, 1, 41,  9, 49, 17, 57, 25 };
// key, 64->56
int PC_1[] = { 57, 49, 41, 33, 25, 17,  9,
				1, 58, 50, 42, 34, 26, 18,
			   10,  2, 59, 51, 43, 35, 27,
			   19, 11,  3, 60, 52, 44, 36,
			   63, 55, 47, 39, 31, 23, 15,
				7, 62, 54, 46, 38, 30, 22,
			   14,  6, 61, 53, 45, 37, 29,
			   21, 13,  5, 28, 20, 12,  4 };
// key, 56->48
int PC_2[] = { 14, 17, 11, 24,  1,  5,
				3, 28, 15,  6, 21, 10,
			   23, 19, 12,  4, 26,  8,
			   16,  7, 27, 20, 13,  2,
			   41, 52, 31, 37, 47, 55,
			   30, 40, 51, 45, 33, 48,
			   44, 49, 39, 56, 34, 53,
			   46, 42, 50, 36, 29, 32 };
// Function
// Expansion 32->48
int E[] = { 32,  1,  2,  3,  4, 5,
			 4,  5,  6,  7,  8, 9,
			 8,  9, 10, 11, 12, 13,
			12, 13, 14, 15, 16, 17,
			16, 17, 18, 19, 20, 21,
			20, 21, 22, 23, 24, 25,
			24, 25, 26, 27, 28, 29,
			28, 29, 30, 31, 32, 1 };
// S_box, 6->4
int S_box[8][4][16] = {
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},
	{
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	},
	{
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	},
	{
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},
	{
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},
	{
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	},
	{
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	},
	{
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
	}
};
// permutation
int P[] = { 16,  7, 20, 21,
			29, 12, 28, 17,
			 1, 15, 23, 26,
			 5, 18, 31, 10,
			 2,  8, 24, 14,
			32, 27,  3,  9,
			19, 13, 30,  6,
			22, 11,  4, 25 };

vector<int> ToBinary(string Text)
{
	vector<int> text(64);
	int c = 63;
	for (int i = Text.size() - 1; i >= 2; i--)
	{
		stringstream ss;
		ss << hex << Text[i];
		int temp;
		ss >> temp;
		for (int j = 0; j < 4; j++)
		{
			text[c--] = temp % 2;
			temp /= 2;
		}
	}
	return text;
}
vector<int> subkey[16];

vector<int> LeftShift(vector<int> k, int bit)
{
	vector<int> temp(28);
	if (bit == 1)
	{
		temp[27] = k[0];
		for (int i = 0; i < 27; i++)
			temp[i] = k[i + 1];
	}
	else
	{
		temp[26] = k[0];
		temp[27] = k[1];
		for (int i = 0; i < 26; i++)
			temp[i] = k[i + 2];
	}
	return temp;
}
void generateKey(vector<int> key_64)
{
	vector<int> key_56(56), C(28), D(28), key_48(48);
	// PC-1 , 64->56
	for (int i = 0; i < 56; i++)
		key_56[i] = key_64[PC_1[i] - 1];
	// Co, Do
	for (int i = 0; i < 28; i++)
		C[i] = key_56[i];
	for (int i = 28; i < 56; i++)
		D[i - 28] = key_56[i];
	// 16次, rotation
	for (int n = 1; n <= 16; n++)
	{
		// 左移
		if (n == 1 || n == 2 || n == 9 || n == 16)
		{
			C = LeftShift(C, 1);
			D = LeftShift(D, 1);
		}
		else
		{
			C = LeftShift(C, 2);
			D = LeftShift(D, 2);
		}
		// 56->48
		for (int i = 0; i < 28; i++)
		{
			key_56[i] = C[i];
			key_56[i + 28] = D[i];
		}
		for (int i = 0; i < 48; i++)
			key_48[i] = key_56[PC_2[i] - 1];
		subkey[n - 1] = key_48;
	}
}
vector<int> function(vector<int> R, vector<int> K)
{
	vector<int> ExR(48), box(32), output(32);
	// Expansion 32->48
	for (int i = 0; i < 48; i++)
		ExR[i] = R[E[i] - 1];
	// xor
	for (int i = 0; i < 48; i++)
		ExR[i] = ExR[i] ^ K[i];
	// S_BOX 6>4
	int n = 0;
	for (int i = 0; i < 48; i += 6)
	{
		int row = ExR[i] * 2 + ExR[i + 5];
		int col = ExR[i + 1] * 8 + ExR[i + 2] * 4 + ExR[i + 3] * 2 + ExR[i + 4];
		int num = S_box[i / 6][row][col];
		// ToBinary
		for (int j = 3; j >= 0; j--)
		{
			box[j + n] = num % 2;
			num /= 2;
		}
		n += 4;
	}
	// permutation
	for (int i = 0; i < 32; i++)
		output[i] = box[P[i] - 1];
	return output;
}
int main()
{
	string Key, plaintext;
	while (cin >> Key >> plaintext)
	{
		vector<int>plain(64), key(64);
		plain = ToBinary(plaintext);
		key = ToBinary(Key);
		// 產生key
		generateKey(key);
		for (int i = 0; i < 16; i++)
			subkey[i].resize(48);
		// initial_permutation IP
		vector<int> now(64);
		for (int i = 0; i < 64; i++)
			now[i] = plain[IP[i] - 1];
		// Lo, Ro
		vector<int> L(32), R(32);
		for (int i = 0; i < 32; i++)
			L[i] = now[i];
		for (int i = 32; i < 64; i++)
			R[i - 32] = now[i];
		// 16次, xor function Li Ri
		vector<int> newL(32);
		for (int n = 0; n < 16; n++)
		{
			newL = R;
			vector<int> F(32);
			F = function(R, subkey[n]);
			for (int i = 0; i < 32; i++)
				R[i] = L[i] ^ F[i];
			L = newL;
		}
		// 合併
		for (int i = 0; i < 32; i++)
		{
			now[i] = R[i];
			now[i + 32] = L[i];
		}
		// final permutation IP_1
		vector<int> output(64);
		for (int i = 0; i < 64; i++)
			output[i] = now[IP_1[i] - 1];
		// binary to hex
		string cipher = "0x";
		for (int i = 0; i < 64; i += 4)
		{
			int temp = 0;
			temp += output[i] * 8 + output[i + 1] * 4 + output[i + 2] * 2 + output[i + 3];
			if (temp < 10)
				cipher += temp + 48;
			else
				cipher += temp + 55;
		}
		cout << cipher;
	}
	return 0;
}