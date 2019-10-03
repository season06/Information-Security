#include<iostream>
#include<string>
using namespace std;
int main(int argc, char *argv[]) {
	while (1) {
		string type, cipher, key;
		//type = argv[1];
		//key = argv[2];
		//plaintext = argv[3];
		cin >> type >> key >> cipher;
		for (int i = 0; i < cipher.size(); i++)
			cipher[i] = tolower(cipher[i]);
		for (int i = 0; i < key.size(); i++)
			key[i] = tolower(key[i]);
		string plaintext;
		if (type == "caesar") {
			string plaintext = "";
			int num_key = 0;
			for (int i = key.size() - 1, j = 1; i >= 0; i--, j *= 10) {
				num_key += j * (key[i] - 48);
			}
			for (int i = 0; i < cipher.length(); i++) {
				int p = cipher[i] - 97;
				 p -= num_key;
				 if (p < 0)
					 p += 26;
				 plaintext += p + 97;
			}
			cout << plaintext << "\n";
		}
		else if (type == "playfair") {
			char matrix[5][5];
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					matrix[i][j] = NULL;
			string new_key = "";
			for (int i = 0; i < key.size(); i++) {
				if (key[i] == 'j')
					key[i] = 'i';
				size_t found = new_key.find(key[i]);
				if (found == std::string::npos)
					new_key += key[i];
			}
			for (int i = 0; i < new_key.size(); i++) {
				matrix[i / 5][i % 5] = new_key[i];
			}
			string alph = "abcdefghiklmnopqrstuvwxyz";
			int index = 0;
			for (int i = new_key.size(); i < 25; i++) {
				while (index < 26) {
					char temp = alph[index];
					index++;
					size_t found = new_key.find(temp);
					if (found == std::string::npos) {
						matrix[i / 5][i % 5] = temp;
						break;
					}
				}
			}
			for (int i = 0, j = 1; i < cipher.size() - 1; i += 2, j += 2) {
				if (cipher[i] == 'j')
					cipher[i] = 'i';
				if (cipher[j] == 'j')
					cipher[j] = 'i';
			}
			int i_x = 0, i_y = 0, j_x = 0, j_y = 0;
			for (int i = 0, j = 1; i < cipher.size() - 1; i += 2, j += 2) {
				for (int m = 0; m < 25; m++) {
					if (matrix[m / 5][m % 5] == cipher[i])
						i_x = m / 5, i_y = m % 5;
					if (matrix[m / 5][m % 5] == cipher[j])
						j_x = m / 5, j_y = m % 5;
				}
				if (i_x != j_x && i_y != j_y) {
					plaintext += matrix[i_x][j_y];
					plaintext += matrix[j_x][i_y];
				}
				else if (i_x == j_x && i_y != j_y) {
					if (i_y - 1 < 0)
						i_y = 5;
					if (j_y - 1 < 0)
						j_y = 5;
					plaintext += matrix[i_x][i_y - 1];
					plaintext += matrix[j_x][j_y - 1];
				}
				else if (i_x != j_x && i_y == j_y) {
					if (i_x - 1 < 0)
						i_x = 5;
					if (j_x - 1 < 0)
						j_x = 5;
					plaintext += matrix[i_x - 1][j_y];
					plaintext += matrix[j_x - 1][i_y];
				}
			}
			cout << plaintext << "\n";
		}
		else if (type == "vernam") {
			string new_key = "";
			string plaintext = "";
			for (int i = 0; i < cipher.size(); i++)
				cipher[i] = toupper(cipher[i]);
			for (int i = 0; i < key.size(); i++)
				key[i] = toupper(key[i]);
			int len = key.size();
			while (cipher.size()!=0)
			{
				string temp = "";
				if (cipher.size() < len)
					len = cipher.size();
				for (int i = 0; i < len; i++)
					temp += (cipher[i] - 65 ^ key[i] - 65) + 65;
				key = temp;
				cipher.erase(0, len);
				plaintext += temp;
			}
			for (int i = 0; i < plaintext.size(); i++)
				plaintext[i] = tolower(plaintext[i]);
			cout << plaintext << "\n";
		}
		else if (type == "row") {
			string plaintext = "";
			int c = key.size(), r = cipher.size() / c;
			if (cipher.size() % c == 0)
				r = cipher.size() / c;
			else
				r = cipher.size() / c + 1;
			char **matrix = (char **)malloc(sizeof(char*) * r);
			for (int i = 0; i < r; i++)
				matrix[i] = (char(*))malloc(sizeof(char) * c);
			for (int i = 0; i < r; i++)
				for (int j = 0; j < c; j++)
					matrix[i][j] = NULL;
			for (int i = 0; i < cipher.size(); i++) {
				matrix[i / c][i%c] = cipher[i];
			}
			for (int i = 1; i <= key.size(); i++) {
				size_t found = key.find(i + 48);
				int f = found;
				for (int j = 0; j < r; j++)
					if (matrix[j][found] != NULL)
						matrix[j][found]=cipher[0],cipher.erase(0,1);
			}
			for (int i = 0; i < r; i++)
				for (int j = 0; j < c; j++)
					plaintext += matrix[i][j];
			cout << plaintext << "\n";
		}
		else if (type == "rail_fence") {
			string plaintext = "";
			int num_key = 0;
			for (int i = key.size() - 1, j = 1; i >= 0; i--, j *= 10) {
				num_key += j * (key[i] - 48);
			}
			char **matrix = (char **)malloc(sizeof(char*) * num_key);
			for (int i = 0; i < num_key; i++)
				matrix[i] = (char(*))malloc(sizeof(char) * cipher.size());
			for (int i = 0; i < num_key; i++)
				for (int j = 0; j < cipher.size(); j++)
					matrix[i][j] = NULL;
			int command = 0, index = 0;
			for (int i = 0; i < cipher.size(); i++)
			{
				matrix[index][i] = cipher[i];
				if (command == 0) {
					index++;
					if (index == num_key - 1)
						command = 1;
				}
				else if (command == 1) {
					index--;
					if (index == 0)
						command = 0;
				}
			}
			int *num= (int(*))malloc(sizeof(int) * num_key);
			string split[100];
			for (int i = 0; i < num_key; i++) {
				int count = 0;
				for (int j = 0; j < cipher.size(); j++)
					if (matrix[i][j] != NULL)
						count++;
				num[i] = count;
			}
			int len = cipher.size();
			for (int i = 0; i < num_key; i++) {
				split[i] = split[i].append(cipher, 0, num[i]);
				cipher.erase(0, num[i]);
			}
			command = 0, index = 0;
			for (int i = 0; i < len; i++)
			{
				plaintext+=split[index][0];
				split[index].erase(0, 1);
				if (command == 0) {
					index++;
					if (index == num_key - 1)
						command = 1;
				}
				else if (command == 1) {
					index--;
					if (index == 0)
						command = 0;
				}
			}
			cout << plaintext << "\n";
		}
	}
	return 0;
}