#pragma once

#include "stdafx.h"

#include <stack>
#include <string>
#include <stdexcept>

#ifndef TREE_H
#define TREE_H

#define max(a, b) (a > b ? a : b)
#define height(n) (!n ? 0 : n->height) 
#define height_update(n) (max(height(n->left), height(n->right)) + 1)
#define balance_factor(n) (!n ? 0 : (height(n->left) - height(n->right)))

using namespace std;

template<class dataType>
class Tree {
private:
	typedef struct node {
		int key;

		dataType data;

		int height;

		node* left;
		node* right;
	} node;

	node* root;

	stack<void*> callStack;

	void delTree(node*&);

	string toString(node*);

	int minKey(node* const&) const;

	node*& leftRotate(node*&);
	node*& rightRotate(node*&);

	node*& newNode(const int&, const dataType&) const;

	node*& addNode(node*&, const int&, const dataType&);
	node* delWithKey(node*&, const int&);

public:
	Tree();
	virtual ~Tree();

	string toString();

	void addNode(const int&, const dataType&);
	void delWithKey(const int&);

	int minKey() const;

	template<typename dataType> friend ostream& operator<<(ostream&, const Tree<dataType>&);
};

template<class dataType>
Tree<dataType>::Tree() {
	this->root = NULL;
}

template<class dataType>
Tree<dataType>::~Tree() {
	if (this->root)
		delTree(this->root);
}

template<class dataType>
void Tree<dataType>::delTree(node*& root) {
	if (root->left)
		delTree(root->left);

	if (root->right)
		delTree(root->right);

	delete root;
}

template<class dataType>
string Tree<dataType>::toString() {
	if (!this->root)
		return "null";

	return toString(this->root);
}

template<class dataType>
string Tree<dataType>::toString(node* root) {
begin:
	string* aux = NULL;
	unsigned char* instP = NULL;

	//if (root->left) {
	if (!root->left) {
		goto P2;
	}

	if (!aux || (*aux).empty())
		aux = new string();

	*aux += "(";

	this->callStack.push(root);
	this->callStack.push(aux);

	instP = new unsigned char();
	*instP = 1;
	this->callStack.push(instP);

	root = root->left;

	goto begin;

P1:
	aux = (string*) this->callStack.top();
	this->callStack.pop();
	*aux = *(string*)this->callStack.top() + *aux;
	this->callStack.pop();
	root = (node*)this->callStack.top();
	this->callStack.pop();

	*aux += ")<-";
	//}
P2:

	if (!aux || (*aux).empty())
		aux = new string();

	*aux += "|" + to_string(root->key) + ':' + to_string(root->data) + "|";

	//if (root->right) {
	if (!root->right) {
		goto P4;
	}

	if (!aux || (*aux).empty())
		aux = new string();

	*aux += "->(";

	this->callStack.push(root);
	this->callStack.push(aux);

	instP = new unsigned char();
	*instP = 3;
	this->callStack.push(instP);

	root = root->right;

	goto begin;

P3:
	aux = (string*) this->callStack.top();
	this->callStack.pop();
	*aux = *(string*)this->callStack.top() + *aux;
	this->callStack.pop();
	root = (node*)this->callStack.top();
	this->callStack.pop();

	*aux += ")";
	//}
P4:

	if (this->callStack.empty())
		return *aux;
	else {
		unsigned char backP = *(unsigned char*)this->callStack.top();
		this->callStack.pop();
		this->callStack.push(aux);

		switch (backP) {
		case 1:
			goto P1;
		case 3:
			goto P3;
		}
	}

	return "";
}

template<class dataType>
typename int Tree<dataType>::minKey() const {
	if (!this->root)
		throw exception("Empty tree");

	return minKey(this->root);
}

template<class dataType>
typename int Tree<dataType>::minKey(node* const& root) const {
	node* aux = root;
	while (aux->left)
		aux = aux->left;

	return aux->key;
}

template<typename dataType>
typename Tree<dataType>::node*& Tree<dataType>::leftRotate(node*& root) {
	node* y = root->right;
	node* T2 = y->left;

	//Update conections:
	y->left = root;
	root->right = T2;

	//Update heights:
	root->height = height_update(root);
	y->height = height_update(y);

	return y;
}

template<typename dataType>
typename Tree<dataType>::node*& Tree<dataType>::rightRotate(node*& root) {
	node* x = root->left;
	node* T2 = x->right;

	//Update conections:
	x->right = root;
	root->left = T2;

	//Update heights:
	root->height = height_update(root);
	x->height = height_update(x);

	return x;
}

template<class dataType>
typename Tree<dataType>::node*& Tree<dataType>::newNode(const int& key, const dataType& data) const {
	node* ret = new node();

	ret->key = key;
	ret->data = data;
	ret->height = 1;
	ret->left = NULL;
	ret->right = NULL;

	return ret;
}

template<typename dataType>
void Tree<dataType>::addNode(const int& key, const dataType& data) {
	this->root = addNode(this->root, key, data);
}

template<typename dataType>
typename Tree<dataType>::node*& Tree<dataType>::addNode(node*& root, const int& key, const dataType& data) {
	//Normal insertion:--------------------------- 
	if (root == NULL)
		return newNode(key, data);

	if (key < root->key)
		root->left = addNode(root->left, key, data);
	else if (key > root->key)
		root->right = addNode(root->right, key, data);

	//Update height:---------------------------
	root->height = height_update(root);

	//Get balance factor:---------------------------
	int balance = balance_factor(root);

	//Balance this sub tree:---------------------------
	//LL Case:
	if (balance > 1 && key < root->left->key)
		return rightRotate(root);

	//RR Case:
	if (balance < -1 && key > root->right->key)
		return leftRotate(root);

	//LR Case:
	if (balance > 1 && key > root->left->key) {
		root->left = leftRotate(root->left);

		return rightRotate(root);
	}

	//RL Case:
	if (balance < -1 && key < root->right->key) {
		root->right = rightRotate(root->right);

		return leftRotate(root);
	}

	//If is balanced:
	return root;
}

template<typename dataType>
void Tree<dataType>::delWithKey(const int& key) {
	this->root = delWithKey(this->root, key);
}

template<typename dataType>
typename Tree<dataType>::node* Tree<dataType>::delWithKey(node*& root, const int& key) {
	//Normal deletion:---------------------------
	if (!root)
		return root;
	else if (key < root->key) //The node is on the left sub tree:
		root->left = delWithKey(root->left, key);
	else if (key > root->key) //The node is on the right sub tree:
		root->right = delWithKey(root->right, key);
	else { //The node is the current one:
		if (!root->left || !root->right) { //One or none children:
			node* aux = root->left ? root->left : root->right;

			//No child:
			if (!aux) {
				aux = root;
				root = NULL;
			}
			else //One child:
				*root = *aux;

			delete aux;
		}
		else { //Two children:
			node* aux = minKey(root->right);

			root->key = aux->key;

			root->right = delWithKey(root->right, aux->key);
		}
	}

	if (!root)
		return NULL;

	//Update height:---------------------------
	root->height = height_update(root);

	//Get balance factor:---------------------------
	int balance = balance_factor(root);

	//Balance this sub tree:---------------------------
	//LL Case:
	if (balance > 1 && balance_factor(root->left) >= 0)
		return rightRotate(root);

	//LR Case:
	if (balance > 1 && balance_factor(root->left) < 0) {
		root->left = leftRotate(root->left);

		return rightRotate(root);
	}

	//RR Case:
	if (balance < -1 && balance_factor(root->right) <= 0)
		return leftRotate(root);

	//RL Case:
	if (balance < -1 && balance_factor(root->right) > 0) {
		root->right = rightRotate(root->right);

		return leftRotate(root);
	}

	//If is balanced:
	return root;
}

template<typename dataType>
ostream& operator<<(ostream& os, Tree<dataType>& tree) {
	return os << tree.toString();
}

#endif // TREE_H