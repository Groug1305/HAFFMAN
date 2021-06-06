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
		cout << "HEAD READ" << endl;
		unsigned long long len = 0;					//����� ������ ������, �� ������ (������) ������
		int key, q;
		char s;

		file.read((char*)&q, sizeof(q));
		for (int i = 0; i < q; i++) {
			file.read((char*)&s, sizeof(s));		//��������� ������
			cout << s << ": ";
			file.read((char*)&key, sizeof(key));	//��������� ��� ��������
			cout << key << endl;
			Jija[s] = key;							//�������� ���� � ����
			len += key;
		}
		cout << "Len: " << len << endl;
		cout << "HEAD READ END" << endl;
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
	char s, tmp2;
	while (fin.get(s)) {
		for (int count = 0; count < 8 && len>0; count++) {
			tmp2 = s & (1 << (7 - count));
			if (tmp2) {
				tmp = tmp->right;
			}
			else if (!tmp2) {
				tmp = tmp->left;
			}

			if ((tmp->left == NULL) && (tmp->right == NULL)) {
				fout << (tmp->s);
				len--;
				tmp = pudgers;
			}
		}
	}
	fin.close();
	fout.close();

	return 0;
}