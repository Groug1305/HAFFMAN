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
		unsigned long long len = 0;
		int key, q;
		char s;

		file.read((char*)&q, sizeof(q));
		for (int i = 0; i < q; i++) {
			file.read((char*)&s, sizeof(s));		//считываем символ
			file.read((char*)&key, sizeof(key));	//считываем его значение
			Jija[s] = key;
			len += key;
		}

		return len;
	}

	Uzel* CreateList() {
		list<Uzel*> List;
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			Uzel* p = new Uzel(it->first, it->second);
			List.push_back(p);
		}
		while (List.size() != 1) {
			List.sort(compare);

			Uzel* r = List.front();
			List.pop_front();

			Uzel* l = List.front();
			List.pop_front();

			Uzel* p = new Uzel(l, r);
			List.push_back(p);
		}
		return List.front();
	}

	void Print() {
		for (auto it = Jija.begin(); it != Jija.end(); it++){
			cout << it->first << ": " << it->second << endl;
		}
	}

	static bool compare(Uzel* c1, Uzel* c2) {
		return c1->key < c2->key;
	}
};

/*int main() {
	const char* str = "I LOVE жижа";
	int tmp[256] = { 0 };

	for (int i = 0; i < strlen(str) - 1; i++) {
		tmp[str[i]]++;
	}

	list <Uzel> jija;

	for (int i = 0; i < 256; i++) {
		if (tmp[i] != 0) {
			Uzel a = Uzel(i, tmp[i]);
			jija.push_back(a); //заполнили список узлами, создаваемые по тмп массиву
		}
	}
	jija.sort(compare);

	list <Uzel> ::iterator it;
	for (it = jija.begin(); it != jija.end(); it++) {
		cout << (*it);
	}

	return 0;
}*/