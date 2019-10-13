#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
using namespace std;

void Caesar(string Key, string plaintext)
{
	int key = stoi(Key);
	for (int i = 0; i < plaintext.size(); i++)
	{
		plaintext[i] += key;
		if (plaintext[i] >= 123)	// 超過'z', -26回到'a'~'z'
			plaintext[i] -= 26;
		plaintext[i] = toupper(plaintext[i]);
	}
	cout << plaintext;
}
void Playfair(string key, string plaintext)
{
	vector<char> alph = { 'A','B','C','D','E','F','G','H','I','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	for (int i = 0; i < key.size(); i++)	//I J同一個字
		if (key[i] == 'J') key[i] = 'I';
	// 建立 5*5 matrix
	char matrix[5][5] = {};
	int m = 0, n = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (n < key.size())		// 將key放入matrix
			{
				vector<char>::iterator it;
				for (it = alph.begin(); it != alph.end(); it++)
				{
					if (*it == key[n]) // key重複字母 跳過
					{
						it = alph.erase(it);
						break;
					}
				}
				if (it == alph.end())	// 在alph中沒找到, 返回上一格繼續
				{
					j--; n++;
				}
				else
					matrix[i][j] = key[n++];
			}
			else			// 將剩下的字母放入matrix
				matrix[i][j] = alph[m++];
		}
	}
	string ciphertext = "";
	for (int i = 0; i < plaintext.size(); i++)
	{
		char a = plaintext[i] - 32; char b = plaintext[++i] - 32; //重複 插入X (插入後plaintext為偶數?)
		if (a == b)
		{	b = 'X'; i--;	}
		if (a == 'J') a = 'I';
		if (b == 'J') b = 'I';
		int pax = 0, pay = 0, pbx = 0, pby = 0;
		int ck = 0;
		// 從matrix中找字
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 5; y++)
			{
				if (a == matrix[x][y])
				{
					pax = x; pay = y;
					ck++;
				}
				else if (b == matrix[x][y])
				{
					pbx = x; pby = y;
					ck++;
				}
				if (ck == 2) break;
			}
		}
		// 轉成ciphertext
		if (pay == pby)		 //右
		{
			ciphertext += matrix[(pax + 1)%5][pay];
			ciphertext += matrix[(pbx + 1)%5][pby];
		}
		else if (pax == pbx) //下
		{
			ciphertext += matrix[pax][(pay + 1) % 5];
			ciphertext += matrix[pbx][(pby + 1) % 5];
		}
		else				// 對角
		{
			ciphertext += matrix[pax][pby];
			ciphertext += matrix[pbx][pay];
		}
	}
	cout << ciphertext;
}
void Vernam(string Key, string plaintext)
{
	string key = Key;
	string ciphertext = "";
	for (int i = 0; i < plaintext.size(); i++)
	{
		plaintext[i] = toupper(plaintext[i]);
		key += plaintext[i];	// autokey: key + plaintext		
		char temp = (plaintext[i] - 65) ^ (key[i] - 65);	// 變成數字做xor
		temp += 65;
		ciphertext += temp;
	}
	cout << ciphertext;
}
void Row(string Key, string plaintext)
{
	map<int, int> key; // key的數字->對應順序
	int max = 0;
	for (int i = 0; i < Key.size(); i++)
	{
		int num = Key[i] - 48;
		if (num > max) max = num;
		key[num] = i;
	}
	// 建立 row=max 的matrix
	int col = (plaintext.size() / max) + 1;
	vector<vector<char>> matrix;
	matrix.resize(col);
	int n = 0;
	for (int i = 0; i < plaintext.size(); i++)
	{
		matrix[n].push_back(plaintext[i]);
		if (matrix[n].size() == max)
			n++;
	}
	// 轉成ciphertext
	string ciphertext = "";
	for (int i = 0; i < key.size(); i++)
	{
		int t = 0;
		while (t < col && key[i + 1] < matrix[t].size())
		{
			ciphertext += toupper(matrix[t++][key[i + 1]]);
		}
	}
	cout << ciphertext;
}
void Rail_fence(string Key, string plaintext)
{
	int key = stoi(Key);
	vector<vector<char>> fence;
	fence.resize(key);
	for (int i = 0; i < plaintext.size(); i++)
	{
		int len = (key * 2) - 2; // 計算一圈的長度 1->2->3->2 (4)
		int n = i % len;	// 取餘數判斷去程還回程
		if (n < key)	// 去程
			fence[n].push_back(plaintext[i]);
		else			// 回程
			fence[len - n].push_back(plaintext[i]);
	}
	string ciphertext = "";
	for (int i = 0; i < key; i++)
		for (int j = 0; j < fence[i].size(); j++)
			ciphertext += toupper(fence[i][j]);
	cout << ciphertext;
}
int main(int argc, char *argv[])
{
	string cipher = argv[1], key = argv[2], plaintext = argv[3];
	//string cipher , key , plaintext ;
	//while (cin >> cipher >> key >> plaintext)
	{
		if (cipher == "caesar")
			Caesar(key, plaintext);
		else if (cipher == "playfair")
			Playfair(key, plaintext);
		else if (cipher == "vernam")
			Vernam(key, plaintext);
		else if (cipher == "row")
			Row(key, plaintext);
		else if (cipher == "rail_fence")
			Rail_fence(key, plaintext);
		else
			cout << "error";
	}
	return 0;
}