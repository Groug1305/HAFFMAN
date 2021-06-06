#include <iostream>
#include <map>
#include <list>
#include <ctime>
#include <string.h>
#include <fstream>
#include <vector>
#include <stdbool.h>
using namespace std;

class Uzel {
public:
	char s;
	int key;
	Uzel* left, * right;

	///////////////////////////////////////

	Uzel() {
		left = NULL;
		right = NULL;
	}

	Uzel(char S, int k) {
		s = S;
		key = k;
		left = NULL;
		right = NULL;
	}

	Uzel(Uzel* l, Uzel* r) {
		key = l->key + r->key;
		left = l;
		right = r;
	}

	~Uzel() {
		delete[] left;
		delete[] right;
	}

	friend ostream& operator << (ostream&, Uzel&);
};

ostream& operator << (ostream& out, Uzel& jija) {
	out << jija.s << ": " << jija.key << endl;
	return out;
}

class MapKeys {
public:
	map<char, int> Jija;

	///////////////////////////////////////

	void CreateMap(ifstream& file) {
		char c;
		while (file.get(c)) {
			Jija[c]++;
		}
		file.clear();
		file.seekg(0); //возвращаем указатель в начало файла
	}

	int HeadRead(ifstream& file) {
		unsigned long long len = 0;					//длина общего текста, не считая (собаки) головы
		int key, q;
		char s;

		file.read((char*)&q, sizeof(q));
		for (int i = 0; i < q; i++) {
			file.read((char*)&s, sizeof(s));		//считываем символ
			file.read((char*)&key, sizeof(key));	//считываем его значение
			Jija[s] = key;							//помещаем ключ в мапу
			len += key;
		}

		return len;
	}

	Uzel* CreateList() {
		list<Uzel*> List;
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			Uzel* p = new Uzel(it->first, it->second);			//создаем узлы и засовываем в конец листа
			List.push_back(p);
		}
		while (List.size() != 1) {
			List.sort(compare);													//сортируем по размеру key

			Uzel* right_from_list = List.front();								//берем первый как правый, убираем из листа
			List.pop_front();

			Uzel* left_from_list = List.front();								//берем второй как левый, убираем из листа
			List.pop_front();

			Uzel* p = new Uzel(left_from_list, right_from_list);				//создаем узел из левого и правого и пушим в конец
			List.push_back(p);
		}
		return List.front();													//возращаем
	}

	void Print() {
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			cout << it->first << ": " << it->second << endl;					//печатается структура вида " *символ_нейм*: *его_число*"
		}
	}

	static bool compare(Uzel* c1, Uzel* c2) {
		return c1->key < c2->key;
	}
};

vector<bool> bincode;
map<char, vector<bool>> table;

void CreateTable(Uzel* T) {
	if (T->left) {
		bincode.push_back(0);							//налево пушим ноль
		CreateTable(T->left);
	}
	if (T->right) {
		bincode.push_back(1);							//направо пушим единицу
		CreateTable(T->right);
	}
	if (((T->right) == NULL) && ((T->left) == NULL)) {	//в конце записываем в мапу символ и его код
		table[T->s] = bincode;
	}
	if (!bincode.empty()) bincode.pop_back();
}


int main() {
	ifstream fin("test.txt", ios::binary);
	if (!fin.is_open()) {
		cout << "test_error";
		return 1;
	}

	ofstream fout("encode.txt", ios::binary);
	if (!fout.is_open()) {
		cout << "encode_error";
		return 1;
	}

	MapKeys jija;

	jija.CreateMap(fin);					//создаем мапу
	Uzel* pudgers = jija.CreateList();		//создаем массив имени пуджа
	jija.Print();
	CreateTable(pudgers);					//после чего приступаем к созданию таблицы

	int k = table.size();
	fout.write((char*)&k, sizeof(k));										//записываем количество символов элементов
	for (auto it = jija.Jija.begin(); it != jija.Jija.end(); it++){
		fout.write((char*)&it->first, sizeof(it->first));					//пишем символ
		fout.write((char*)&it->second, sizeof(it->second));					//пишем количество
	}

	char tmp = 0;
	int count = 0;
	while (!fin.eof()) {								//пока не конец файла
		char c = fin.get();								//берем символ и его код
		vector<bool> tmp2 = table[c];
		for (int i = 0; i < tmp2.size(); i++) {			//по коду символа
			tmp = tmp | (tmp2[i] << (7 - count));
			count++;
			if (count == 8) {							//как только есть 8 битов - записываем в fout
				count = 0;
				fout << tmp;
				tmp = 0;
			}
		}
	}
	if (tmp) fout << tmp;
	fin.close();
	fout.close();

	return 0;
}