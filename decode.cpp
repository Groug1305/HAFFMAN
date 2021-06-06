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
	Uzel *left, *right;

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
		for (file.get(c); !file.eof(); file.get(c)) {
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
		for (auto it = Jija.begin(); it != Jija.end(); it++){
			cout << it->first << ": " << it->second << endl;					//печатается структура вида " *символ_нейм*: *его_число*"
		}
	}

	static bool compare(Uzel* c1, Uzel* c2) {
		return c1->key < c2->key;
	}
};

vector<bool> bincode;
map<char, vector<bool>> table;

void CreateTable(Uzel* T){
	if (T->left) {
		bincode.push_back(0);
		CreateTable(T->left);
	}
	if (T->right) {
		bincode.push_back(1);
		CreateTable(T->right);
	}
	if (((T->right) == NULL) && ((T->left) == NULL)){
		table[T->s] = bincode;
	}
	if (!bincode.empty()) bincode.pop_back();
}

int main() {
	ifstream fin("encode.txt", ios::binary);
	if (!fin.is_open()){ 
		cout << "encode_error"; 
		return 1; 
	}

	ofstream fout("decode.txt", ios::binary);
	if (!fout.is_open()){ 
		cout << "decode_error"; 
		return 1; 
	}

	MapKeys jija;

	int len = jija.HeadRead(fin);		//из мапы жижи получаем общую длинну, получаенную при считывании хэдера
	jija.Print();
	Uzel* pudgers = jija.CreateList();	//создаем массив узлов имени пуджерса, он у нас первый из листа

	Uzel* tmp = pudgers;				//копируем пуджерса во временную переменную
	CreateTable(pudgers);
	int count = 0;
	char с = fin.get();
	while (!fin.eof()) {
		bool bin = с & (1 << (7 - count));
		if (bin) tmp = tmp->right;						//при 1 смещаемся вправо
		else tmp = tmp->left;							//при нуле - влево

		if (tmp->left == NULL && tmp->right == NULL) {	//если дошли до конца, то выписываем 
			fout << tmp->s;								//символ и возвращаемся в начало листа
			tmp = pudgers;
		}
		count++;

		if (count == 8) {
			count = 0;
			fin >> с;
		}
	}

	return 0;
}