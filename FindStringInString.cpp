// FindStringInString.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;


vector<int> str_in_pi_function(string str) {
	int str_lenght = str.length();
	vector<int> pi(str_lenght);
	pi[0] = 0;
	int k;
	for (int i = 1; i < str_lenght; i++) {
		k = pi[i - 1]; // нужно наибольшее k. Берем такое так как это >= pi[i] - 1
		while (k > 0 && str[i] != str[k]) {
			k = pi[k - 1]; // следующее потенциальное наибольшее k
		}
		if (str[i] == str[k]) {
			k++;
		}
		pi[i] = k;
	}
	return pi;
}


vector<int>  str_in_z_function(string str) {
	int str_lenght = str.length();
	vector<int> z (str_lenght);
	int left = 0, right = 0;
	z[0] = str_lenght;
	for (int i = 1; i < str_lenght; i++) {
		z[i] = max(0, min(right - i, z[i - left]));
		while (z[i] + i < str_lenght && str[z[i]] == str[i + z[i]]) {
			z[i]++;
		}
		if (i + z[i] > right) {
			right = i + z[i];
			left = i;
		}
	}
	return z;
}


vector<int> pi_function_in_z(vector<int> pi) {
	vector<int> z(pi.size());
	for (int i = 1; i < pi.size(); i++) {
		if (pi[i] > 0) {
			z[i - pi[i] + 1] = pi[i]; 
			// на самом деле значение в этой ячейке больше или 
			// равно значения пи. Понятно из определения функций
		}
	}
	z[0] = pi.size();
	int i = 1;
	while (i < pi.size()) {
		int t = i;
		if (z[i] > 0) {
			for (int j = 1; j < z[i]; j++) {
				// смотрим значения функции. 
				// не заполняем ящейки, заполненные раньше, ...
				// ...когда говорили о больше или равно какого-то числа
				if (z[i + j] > z[j]) {
					break;
				}
				z[i + j] = min(z[j], z[i] - j); //нельзя больше, чем допустимо 
				t = i + j;
			}
		}
		i = t + 1;
	}
		return z;

}


vector<int> z_function_in_pi(vector<int> z) {
	vector<int> pi;
	pi.assign(z.size(), 0);
	for (int i = 1; i < z.size(); i++) {
			if (z[i]) //если не ноль
			{
				//перебираем все номера элементов сверху вниз
				// прервать, если уже pi тут посчитана раньше
				for (int j = z[i] - 1; j >= 0 && !(pi[i + j]); j--){
					pi[i + j] = 1 + j;
			}
		}
	}
	return pi;
}


string pi_function_in_str(vector<int> pi) {
	string str = "a";
	string alphabet = "bcdefghijklmnopqrstuvwxyz"; //"а" все равно нам не нужна для выбора
	//так как если префикс функция не ноль, то там точно не "а".
	int index = 1; // позиция, с которой можно начинать искать нужную букву (0-ая для "а", а ее мы не ищем)
	for (int i = 1; i < pi.size(); i++) {
		if (pi[i] == 0) {
			//нужно записать первый символ из алфавита, такой что он не префикс
			int last_pi = pi[i - 1]; //знаем длину предыдущего префикса
			vector<bool> number_prohibited_characters; // храним тут запрещенные символы
			number_prohibited_characters.assign((alphabet.size()), false);
			while (last_pi != 0) {
				number_prohibited_characters[int(str[last_pi]) - 97] = true;
				last_pi = pi[last_pi - 1];//сдвигаемся
			}
			//записали все символы, которые нельзя использовать
			//теперь нужно взять самый маленьких из тех, которые можно
			while (number_prohibited_characters[index] == true) {
				index++;
			}
			str = str + alphabet[index - 1];
			index = 1; 
		}
		else {
			// запишем уже известный символ
			str = str + str[pi[i] - 1];
		}
	}
	return str;
}


string z_function_in_str(vector<int> z) {
	z[0] = 0; //для корректной работы
	string str = "a"; // изначальная строка минимальна
	string alphabet = "bcdefghijklmnopqrstuvwxyz"; // буква а всё равно не нужна для нового символа 
	int length_pref = 0; // длина префикса
	int index_symbol = 0; // индекс последнего символа строки
	for (int i = 1; i < z.size(); i++) { // с единички, так как превая буква "а"
		if (z[i] == 0 && length_pref == 0) {
			// Префикс переписывать не нужно, так как его нет.
			// Поэтому просто запишем новую букву из тех, которые можно
			vector<bool> number_prohibited_characters; // храним тут запрещенные символы
			number_prohibited_characters.assign((alphabet.size()), false);
			for (int k = i - 1; k > 0; k--) {
				if (i - k == z[k]) {
					// если наша буква может увеличить длину префикса
					// то надо запретить писать такую букву
					number_prohibited_characters[int(str[z[k]]) - 97] = true;
				}
			}
			// теперь просто выберем первую из незапрещенных
			int index = 1;
			while (number_prohibited_characters[index] == true) {
				index++;
			}
			str = str + alphabet[index - 1];
		}
		// отмечаем, что это префикс и мы можем просто его переписать
		if (z[i] > length_pref) {
			index_symbol = 0;
			length_pref = z[i];
		}
		// запишем уже существующий префикс
		if (length_pref > 0) {
			length_pref--;
			str += str[index_symbol];
			index_symbol++;
		}
	}
	return str;
}


int main()
{
	int number;
	string str;
	//getline(cin, str);
	vector <int> input_data;
	while (cin >> number) {
		input_data.push_back(number);
	}
	str = z_function_in_str(input_data);
	cout << str;
    /*	
    vector<int> answer1 = str_in_pi_function(str);
	vector<int> answer2 = pi_function_in_z(answer1);
	for (int i = 0; i < answer2.size(); i++) {
		cout << answer2[i] <<" ";
	}*/
	return 0;
}
