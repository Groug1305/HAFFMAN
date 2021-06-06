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
		file.seekg(0); //���������� ��������� � ������ �����
	}

	int HeadRead(ifstream& file) {
		unsigned long long len = 0;					//����� ������ ������, �� ������ (������) ������
		int key, q;
		char s;

		file.read((char*)&q, sizeof(q));
		for (int i = 0; i < q; i++) {
			file.read((char*)&s, sizeof(s));		//��������� ������
			file.read((char*)&key, sizeof(key));	//��������� ��� ��������
			Jija[s] = key;							//�������� ���� � ����
			len += key;
		}

		return len;
	}

	Uzel* CreateList() {
		list<Uzel*> List;
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			Uzel* p = new Uzel(it->first, it->second);			//������� ���� � ���������� � ����� �����
			List.push_back(p);
		}
		while (List.size() != 1) {
			List.sort(compare);													//��������� �� ������� key

			Uzel* right_from_list = List.front();								//����� ������ ��� ������, ������� �� �����
			List.pop_front();

			Uzel* left_from_list = List.front();								//����� ������ ��� �����, ������� �� �����
			List.pop_front();

			Uzel* p = new Uzel(left_from_list, right_from_list);				//������� ���� �� ������ � ������� � ����� � �����
			List.push_back(p);
		}
		return List.front();													//���������
	}

	void Print() {
		for (auto it = Jija.begin(); it != Jija.end(); it++){
			cout << it->first << ": " << it->second << endl;					//���������� ��������� ���� " *������_����*: *���_�����*"
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

	int len = jija.HeadRead(fin);		//�� ���� ���� �������� ����� ������, ����������� ��� ���������� ������
	jija.Print();
	Uzel* pudgers = jija.CreateList();	//������� ������ ����� ����� ��������, �� � ��� ������ �� �����

	Uzel* tmp = pudgers;				//�������� �������� �� ��������� ����������
	CreateTable(pudgers);
	int count = 0;
	char � = fin.get();
	while (!fin.eof()) {
		bool bin = � & (1 << (7 - count));
		if (bin) tmp = tmp->right;						//��� 1 ��������� ������
		else tmp = tmp->left;							//��� ���� - �����

		if (tmp->left == NULL && tmp->right == NULL) {	//���� ����� �� �����, �� ���������� 
			fout << tmp->s;								//������ � ������������ � ������ �����
			tmp = pudgers;
		}
		count++;

		if (count == 8) {
			count = 0;
			fin >> �;
		}
	}

	return 0;
}