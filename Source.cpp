#include<iostream>
#include<string>
#include"List.h"
#include<fstream>
using namespace std;

//------------------------------------Doc_Info Class------------------------------------
class Term_Info;
class Search_Engine;
class Doc_Info {
	int Doc_iD;
	int term_f;
public:
	friend class Search_Engine;
	friend class Term_Info;
	Doc_Info() {
		Doc_iD = 0;
		term_f = 0;
	}
	Doc_Info(Doc_Info const& obj) {
		Doc_iD = obj.Doc_iD;
		term_f = obj.term_f;
	}
	Doc_Info(int Id, int freq) {
		Doc_iD = Id;
		term_f = freq;
	}
};


//------------------------------------Term_Info Class-----------------------------------
class Search_Engine;
class Term_Info {
	string key;
	List<Doc_Info> doc;
public:
	friend class Search_Engine;
	template<class T>friend class HashTable;
	Term_Info() {
		key = "\0";
	}

	Term_Info(string k) {
		key = k;
	}

	Term_Info operator*() {
		return *&(this->key);
	}


	bool operator<(const Term_Info& obj) {
		return key < obj.key;
	}

	bool operator>(const Term_Info& obj) {
		return key > obj.key;
	}

	friend ostream& operator<<(ostream& out, const Term_Info& obj) {
		out << obj.key << " ";
		return out;
	}
};

//---------------------Global Template Function to Grow the array-----------------------

template <class type>
void Grow(type*& array, int& size)
{
	type* temp = new type[size + 1];

	for (int i = 0; i < size; i++)
	{
		temp[i] = array[i];
	}
	size++;
	delete[]array;
	array = temp;
}


//---------------------HashTable Class--------------------------
class Term_Info;
template<class T>
class HashTable {
	int tableSize;
	int maxSize;
	T* table;

	//---------Hash Function to make index---------
	unsigned int hash(const string& key, int tSize) {
		unsigned int hashval = 0;
		for (char ch : key) {
			hashval = 37 * hashval + ch;
		}
		return hashval % tSize;
	}
public:

	//-----Default Constructor-----

	HashTable() {
		tableSize = 0;
		maxSize = 11;
		table = new T[maxSize];
	}


	//--------Copy Condtructor-------

	HashTable(const HashTable<T>& obj) {
		maxSize = obj.maxSize;
		tableSize = obj.tableSize;
		for (int i = 0; i < maxSize; i++) {
			table[i] = obj.table[i];
		}
	}

	//-------Insert Function-------
	void insert(Term_Info& ele) {
		int index = hash((ele.key), maxSize);
		float loadF = tableSize / maxSize;
		if (loadF < 2.0) {
			table[index].insertAtEnd(ele);
			tableSize++;
		}
		else {
			table[index].insertAtEnd(ele);
			tableSize++;
			Rehash(table, maxSize);
		}
	}

	//------Rehash Function------
	void Rehash(T*& tab, int& max) {
		int num = max;
		max = max * 2;
		T* temp = new T[max];
		for (int i = 0; i < num; i++) {
			if (table[i].get() != NULL) {
				List<Term_Info>::Iterator itr = table[i].begin();
				while (itr != NULL) {
					int index = hash((*itr).key, max);
					temp[index].insertAtEnd(*itr);
					itr++;
				}
			}
		}
		delete[]table;
		table = temp;
	}

	//---------Search Function---------
	List<Doc_Info>* get(string const key) {
		int index = hash(key, maxSize);                                       //If Key exists in Hash_Table then
		List<Term_Info>::Iterator itr = table[index].begin();                 //it returns Doc_Info List from it
		bool flag = false;													  //otherwise it return nullptr
		if (table[index].get() == NULL) {
			return nullptr;
		}
		if ((*itr).key == key) {
			return &((*itr).doc);
		}
		else if (++itr != NULL) {
			while (!flag && itr != NULL) {
				if ((*itr).key == key) {
					return &((*itr).doc);
				}
				else {
					itr++;
				}
			}
			return nullptr;
		}
		else {
			return nullptr;
		}
	}

	//------Delete Function------
	bool Delete(string& key) {
		int index = hash(key, maxSize);
		List<Term_Info>::Iterator itr = table[index].begin();
		bool flag = false;                                       //Checks if given Key is present in Table
		if (table[index].get() == NULL) {						 //then it deletes it's object from List 	
			return false;										 //and returns true otherwise it returns false
		}
		if ((*itr).key == key) {
			flag = table[index].Rem(itr);
			return flag;
		}
		else if (++itr != NULL) {
			while (!flag && itr != NULL) {
				if ((*itr).key == key) {
					flag = table[index].Rem(itr);
					return flag;
				}
				else {
					itr++;
				}
			}
			return false;
		}
		else {
			return false;
		}
	}

	//-----Function to Print Hash Table and Chains-----
	void print() {
		for (int i = 0; i < maxSize; i++) {
			if (table[i].get() != NULL) {
				List<Term_Info>::Iterator itr = table[i].begin();
				while (itr != NULL) {
					cout << (*itr).key << "-----> ";
					itr++;
				}
				cout << endl;
			}
			else {
				cout << "------\n";
			}
		}
	}

	//---Destructor---
	~HashTable() {
		delete[]table;
		table = 0;
	}
};

//--------------------------------Search_Engine Class-------------------------------

class Search_Engine {
	HashTable<List<Term_Info>>* Index;                              //Index of type List<Term_Info> is made
public:
	Search_Engine() {
		Index = new HashTable<List<Term_Info>>;
	}

	//</----------------Algorithm-1---------------\> 
	void Create_Index(string* fileName, int Dsize) {
		for (int i = 0; i < Dsize; i++) {								//For each document Add_Doc_to_index 
			Add_Doc_to_Index(fileName[i]);                              //Function is called
		}
	}

	//----Function to Calculate Term Frequency-----
	int termfreq(string key, string fname) {
		int size = 0;
		int ind = -1;
		int count = 0;
		string* word = new string[size];
		ifstream fin(fname.c_str());
		while (!fin.eof()) {
			Grow(word, size);
			ind++;
			fin >> word[ind];
		}
		fin.close();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < word[i].length(); j++)
				word[i][j] = tolower(word[i][j]);
		}
		for (int i = 0; i < size; i++) {
			if (key == word[i]) {
				count++;
			}
		}
		return count;
	}

	//---------Functions to Remove Duplicate Words--------

	void removeSpecificWord(string* arr, int idx, int& size)
	{
		int i;
		for (i = idx; i < size - 1; i++)
			arr[i] = arr[i + 1];
		size--;
	}

	void removeDuplicateWord(string* arr, int& size)
	{
		int i, j;
		string target;
		for (i = 0; i < size; i++)
		{
			arr[i];
			target = arr[i];
			for (j = i + 1; j < size; j++)
			{
				if (target == arr[j]) {
					removeSpecificWord(arr, j, size);
					arr[i];
					j--;
				}
			}
		}
	}


	void Add_Doc_to_Index(string fName) {
		ifstream fin(fName.c_str());
		int size = 0;
		int index = -1;
		string* arr = new string[size];
		while (!fin.eof()) {                                        //Adding words of each document in array
			string temp;											//then converting all the words to lower
			fin >> temp;											//case and then removing duplicates
			if (temp != " " && temp != "") {						//from array
				Grow(arr, size);
				index++;
				arr[index] = temp;
			}
		}
		fin.close();
		for (int i = 0; i < size; i++)                              //Converting keys to lower Case
		{
			for (int j = 0; j < arr[i].length(); j++)
				arr[i][j] = tolower(arr[i][j]);
		}
		removeDuplicateWord(arr, size);                            //Removing Duplicates from Array

		for (int i = 0; i < size; i++)
		{
			bool check = false;
			List<Doc_Info>* Duh = Index->get(arr[i]);                          //Searching each term in hash table 
			if (Duh != nullptr)												   //If term is found then it returns
			{																   //it's List and then adding doc_info  
				Doc_Info D(fName[3] - 48, termfreq(arr[i], fName));			   //object into the doc list in that term 
				Duh->insertAtEnd(D);										   //and then deleting that term from array

				shrink(arr, i, size);
				check = true;
			}
			if (check) {
				i--;
			}
		}

		if (size != 0)
		{
			for (int i = 0; i < size; i++)
			{
				Term_Info T(arr[i]);											//if any term left in array it			
				Doc_Info doc(fName[3] - 48, termfreq(arr[i], fName));           //adds that term in the Hash Table
				T.doc.insertAtEnd(doc);											//and initialize the Doc_Info List	
				Index->insert(T);                                               //in that term
			}
		}
		delete[]arr;
	}

	void shrink(string*& arr, int index, int& size) {
		for (int i = index; i + 1 < size; i++) {
			arr[i] = arr[i + 1];
		}
		size--;
	}


	//</-----------------Algorithm-2---------------\>
	void Search_Document(string* query, int Qsize) {
		int isize = 0;
		Doc_Info* iarr = new Doc_Info[isize];
		int Iindex = -1;
		for (int i = 0; i < Qsize; i++)
		{
			for (int j = 0; j < query[i].length(); j++)
				query[i][j] = tolower(query[i][j]);
		}

		for (int i = 0; i < Qsize; i++)                              //Here Instead of using Iterators 
		{															 //Checking each query term if it
			List<Doc_Info>* D = Index->get(query[i]);				 //exists in hash table then taking 	
			if (D != nullptr) {										 //the whole Doc_Indo List	
				List<Doc_Info>::Iterator itr = D->begin();
				if (itr == D->end()) {
					Grow(iarr, isize);                               //If there is only one 
					Iindex++;									     //Node in Doc_List then 
					iarr[Iindex] = *itr;							 //it inserts it in array	
				}
				else {
					for (; itr != NULL; itr++) {                      //else Inserting the whole
						Grow(iarr, isize);                            //doc_Info List from the 
						Iindex++;								      //term_info into the array
						iarr[Iindex] = *itr;
					}
				}
			}
		}

		if (isize == 0) {                                                       //Checking if no query 
			cout << "Not Found:(\n";                                            //word matched to key terms
		}                                                                       //then displays some msg

		//Rule-1
		int fsize = 0;
		int* farr = new int[fsize];
		int ind = -1;
		for (int i = 0; i < isize; i++) {
			int count = 1;
			for (int j = i + 1; j < isize; j++) {
				if (iarr[i].Doc_iD == iarr[j].Doc_iD) {
					count++;                                                       //Removing duplicate words 
					iarr[i].term_f = iarr[i].term_f + iarr[j].term_f;              //from the array and adding
					removeD(iarr, j, isize);									   //their corresponding term
					j--;														   //frequency(to rank them later)
				}
			}
			Grow(farr, fsize);
			ind++;
			farr[ind] = count;													   //Creating a parallel array
		}																		   //for storing query terms frequency
		for (int i = 0; i < fsize - 1; i++) {
			for (int j = 0; j < fsize - i - 1; j++) {
				if (farr[j] < farr[j + 1]) {
					swap(farr[j], farr[j + 1]);									   //Bubble sorting the arrays in 
					swap(iarr[j], iarr[j + 1]);									   //descending order w.r.t to query
				}																   //terms
			}
		}

		//Rule-2
		Doc_Info key;
		int k;
		for (int a = 0; a < fsize;) {
			int tcount = 0;
			for (int i = a; i < fsize; i++) {                                     //Calculating the number of Doc_Info 
																				  //having same query term frequency
				if (farr[i] == farr[a]) {
					tcount++;
				}
			}
			for (int j = a + 1; j < tcount + a; j++)
			{
				key = iarr[j];
				for (k = j - 1; (k >= 0) && (iarr[k].term_f < key.term_f); k--)		//Insertion sorting according to 	
				{																    //the term frequency in descending  
					iarr[k + 1] = iarr[k];                                          //order
				}
				iarr[k + 1] = key;
			}
			a += tcount;
		}

		//Rule-3
		for (int s = 0; s < fsize;) {
			int fcount = 0;
			for (int m = s; m < fsize; m++) {								  //Calculating the number of Doc_Info 
				if (iarr[m].term_f == iarr[s].term_f) {					      //having same term frequency
					fcount++;
				}
			}
			for (int j = s + 1; j < fcount + s; j++)
			{
				key = iarr[j];
				for (k = j - 1; (k >= 0) && (iarr[k].Doc_iD > key.Doc_iD) && (iarr[k].term_f == key.term_f); k--)    //Insertion sorting according to
				{																									 //the Doc_id and term_f in ascending order															 	
					iarr[k + 1] = iarr[k];
				}
				iarr[k + 1] = key;
			}
			s += fcount;
		}
		for (int i = 0; i < isize; i++) {                                           //Displaying the result for 
			cout << "Doc " << iarr[i].Doc_iD << endl;                               //each query term
		}
	}
	void removeD(Doc_Info*& arr, int j, int& isize) {
		for (int i = j; i < isize; i++) {
			arr[i] = arr[i + 1];
		}
		isize--;
	}

	//---Print Function---
	void print() {
		cout << endl;
		Index->print();
		cout << endl;
	}

	//---Delete Function---
	void Del(string word) {
		bool flag = false;                                   //Takes one word as a parameter
		flag = Index->Delete(word);                          //and calls Delete function of             
		if (flag) {                                          //Hash_Table Class if it exists     
			cout << "***Deleted Succesfully:)***";		     //there then it deletes and returns
		}												     //true otherwise false
		else {
			cout << "***Word isn't Present in List***";
		}
	}

	//---Destructor---
	~Search_Engine()
	{
		if (Index) {
			delete Index;
		}
		Index = NULL;
	}
};


//------------Function to tokenize Query------------------
void Querytokenize(string query, string*& Qarr, int& Qsize) {
	ofstream fout;
	fout.open("temp.txt");
	fout << query;
	fout.close();
	int Qindex = -1;                                            //Writing query string to file 
	ifstream fin;											    //then making the array of each  
	fin.open("temp.txt");										//word and then deleting the file
	while (!fin.eof()) {                                        //and then converting each word to  
		Grow(Qarr, Qsize);										//lowercase
		Qindex++;
		fin >> Qarr[Qindex];
	}
	fin.close();
	remove("temp.txt");
}


//----------------------------------Main-----------------------------------
int main() {
	Search_Engine S;
	int size = 4;
	string Docs[4] = { "Doc1.txt", "Doc2.txt", "Doc3.txt", "Doc4.txt" };
	S.Create_Index(Docs, size);
	S.Add_Doc_to_Index("Doc5.txt");
	//S.print();            //Uncomment it if you want to print Hash table
	string query;
	cout << "Enter Search Query: ";
	getline(cin, query);
	int Qsize = 0;
	string* Qarr = new string[Qsize];
	Querytokenize(query, Qarr, Qsize);
	S.Search_Document(Qarr, Qsize);
	return 0;
}