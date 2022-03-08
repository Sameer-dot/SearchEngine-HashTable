#pragma once

#include<iostream>
using namespace std;

//--------------------------------------Node Class--------------------------------------
template<class type>
class Node {
public:
	Node() {
		prev = 0;
		next = 0;
	}
	Node(type d, Node<type>* p = 0, Node<type>* n = 0) {
		data = d;
		prev = p;
		next = n;
	}
	template<class type>friend class List;
private:
	type data;
	Node<type>* next;
	Node<type>* prev;
};

//-----------------------------List Class to make DLL------------------------------
template<class type>
class List {
private:
	Node<type>* head;
	Node<type>* tail;
	int size;
	//------------------Iterator Class as nested class of List---------------------
	class ListIterator {
	public:
		template<class type>friend class List;
		ListIterator(Node<type>* t = NULL) {
			iptr = t;
		};

		ListIterator& operator++(int) {
			ListIterator old = *this;
			++(*this);
			return old;
		}

		ListIterator& operator++() {
			if (iptr)
				iptr = iptr->next;
			return (*this);
		}

		type& operator*() {
			return iptr->data;
		}

		bool operator==(const ListIterator& l) const
		{
			return iptr == l.iptr;
		}

		bool operator!=(const ListIterator& l) const {
			return !(iptr == l.iptr);
		}
	private:
		Node<type>* iptr;
	};
public:
	typedef ListIterator Iterator;
	Iterator begin() {
		Iterator I(head);
		return I;
	}
	Iterator end() {
		Iterator I(tail);
		return I;
	}

	Node<type>* get() {
		return head;
	}

	//---Default Constructor of List Class---
	List() {
		head = NULL;
		tail = NULL;
		size = 0;
	}

	//---Copy Constructor of List Class----
	List(const List<type>& list)
	{
		head = NULL;
		tail = NULL;
		for (Node<type>* tmp = list.head; tmp != 0; tmp = tmp->next) {
			insertAtEnd(tmp->data);
		}
	}

	//---Assignment Operator of List Class----
	List& operator=(const List<type>& obj) {
		if (size != 0) //deleting previous data to avoid memory leak
		{
			removeAll();
		}
		Node<type>* temp = obj.head;
		while (temp != NULL)
		{
			insertAtEnd(temp->data);
			temp = temp->next;
		}
		size = obj.size;
		return *this;
	}

	void removeAll() {
		Node<type>* temp = head;
		while (temp != head) {
			temp = head->next;
			delete head;
			head = temp;
		}
		head = tail = NULL;
	}

	//---Function to Insert the Node At End-----
	void insertAtEnd(type const element) {
		if (tail == 0) {
			head = tail = new Node<type>(element);
			size++;
		}
		else {
			Node<type>* newnode = new Node<type>(element, tail, 0);
			tail->next = newnode;
			newnode->prev = tail;
			tail = newnode;
			size++;
		}
	}

	//------Function to Delete Node-------
	bool Rem(Iterator& itr) {
		Node<type>* temp = (itr).iptr;
		if (temp->prev == nullptr) {
			head = temp->next;
			delete temp;
			return true;
		}
		else if (temp->next == nullptr) {
			tail = temp->prev;
			tail->next = NULL;
			delete temp;
			return true;
		}
		else {
			Node<type>* temp1 = temp->prev;
			temp1->next = temp->next;
			temp->next->prev = temp1;
			delete temp;
			return true;
		}
	}

	//-----Print Function----
	void print() {
		Node<type>* temp = head;
		while (temp != NULL) {
			cout << temp->data;
			temp = temp->next;
		}
	}

	//-----Destructor----
	~List()
	{
		Node<type>* temp = head;
		while (temp != head) {
			temp = head->next;
			delete head;
			head = temp;
		}
		head = tail = NULL;
	}
};