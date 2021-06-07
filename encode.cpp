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
		file.seekg(0); //âîçâðàùàåì óêàçàòåëü â íà÷àëî ôàéëà
	}

	int HeadRead(ifstream& file) {
		unsigned long long len = 0;					//äëèíà îáùåãî òåêñòà, íå ñ÷èòàÿ (ñîáàêè) ãîëîâû
		int key, q;
		char s;

		file.read((char*)&q, sizeof(q));
		for (int i = 0; i < q; i++) {
			file.read((char*)&s, sizeof(s));		//ñ÷èòûâàåì ñèìâîë
			file.read((char*)&key, sizeof(key));	//ñ÷èòûâàåì åãî çíà÷åíèå
			Jija[s] = key;							//ïîìåùàåì êëþ÷ â ìàïó
			len += key;
		}

		return len;
	}

	Uzel* CreateList() {
		list<Uzel*> List;
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			Uzel* p = new Uzel(it->first, it->second);			//ñîçäàåì óçëû è çàñîâûâàåì â êîíåö ëèñòà
			List.push_back(p);
		}
		while (List.size() != 1) {
			List.sort(compare);													//ñîðòèðóåì ïî ðàçìåðó key

			Uzel* right_from_list = List.front();								//áåðåì ïåðâûé êàê ïðàâûé, óáèðàåì èç ëèñòà
			List.pop_front();

			Uzel* left_from_list = List.front();								//áåðåì âòîðîé êàê ëåâûé, óáèðàåì èç ëèñòà
			List.pop_front();

			Uzel* p = new Uzel(left_from_list, right_from_list);				//ñîçäàåì óçåë èç ëåâîãî è ïðàâîãî è ïóøèì â êîíåö
			List.push_back(p);
		}
		return List.front();													//âîçðàùàåì
	}

	void Print() {
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			cout << it->first << ": " << it->second << endl;					//ïå÷àòàåòñÿ ñòðóêòóðà âèäà " *ñèìâîë_íåéì*: *åãî_÷èñëî*"
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
		bincode.push_back(0);							//íàëåâî ïóøèì íîëü
		CreateTable(T->left);
	}
	if (T->right) {
		bincode.push_back(1);							//íàïðàâî ïóøèì åäèíèöó
		CreateTable(T->right);
	}
	if (((T->right) == NULL) && ((T->left) == NULL)) {	//â êîíöå çàïèñûâàåì â ìàïó ñèìâîë è åãî êîä
		table[T->s] = bincode;
	}
	if (!bincode.empty()) bincode.pop_back();
}



///////////////////////////////////////
void PrintVector(vector<bool>code) {
	for (auto it = code.begin(); it != code.end(); it++) {
		cout << *it;
	}
	cout << endl;
}

void PrintTable() {
	for (auto it = table.begin(); it != table.end(); it++)
	{
		cout << it->first << " - ";
		PrintVector(it->second);
	}
}


void PrintFile(const char* PATH) {
	ifstream fin(PATH);
	char s;
	while (fin >> s) {
		for (int i = 0; i < 8; i++) {
			int b = (s & (1 << (7 - i))) >> (7 - i);
			cout << b;
		}
	}
	fin.close();
}
///////////////////////////////////////




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

	jija.CreateMap(fin);					//ñîçäàåì ìàïó
	Uzel* pudgers = jija.CreateList();		//ñîçäàåì ìàññèâ èìåíè ïóäæà
	jija.Print();
	CreateTable(pudgers);					//ïîñëå ÷åãî ïðèñòóïàåì ê ñîçäàíèþ òàáëèöû

	int k = table.size();
	//cout << k;
	fout.write((char*)&k, sizeof(k));										//çàïèñûâàåì êîëè÷åñòâî ñèìâîëîâ ýëåìåíòîâ
	for (auto it = jija.Jija.begin(); it != jija.Jija.end(); it++){
		fout.write((char*)&it->first, sizeof(it->first));					//ïèøåì ñèìâîë
		fout.write((char*)&it->second, sizeof(it->second));					//ïèøåì êîëè÷åñòâî
	}

	char tmp = 0;
	int count = 0;
	while (!fin.eof()) {								//ïîêà íå êîíåö ôàéëà
		char c = fin.get();								//áåðåì ñèìâîë è åãî êîä
		vector<bool> tmp2 = table[c];
		for (int i = 0; i < tmp2.size(); i++) {			//ïî êîäó ñèìâîëà
			tmp = tmp | (tmp2[i] << (7 - count));
			count++;
			if (count == 8) {							//êàê òîëüêî åñòü 8 áèòîâ - çàïèñûâàåì â fout
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
