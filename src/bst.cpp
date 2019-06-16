#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <iomanip>
#include <algorithm>
#include "bst.hpp"
#include "csv.hpp"

// public BST class methods
void BST::insert(const std::string& x, const int& idx)
{
   insert(_root, x, idx);
}

void BST::preOrder() const
{
   std::cout << "\n pre-order traversal of binary search tree" << std::endl;
   preOrder(_root);
   std::cout << std::endl;
}

void BST::postOrder() const
{
   std::cout << "\n post-order traversal of binary search tree" << std::endl;
   postOrder(_root);
   std::cout << std::endl;
}

// use queue to pop out each level in order
void BST::levelOrder()
{
   std::queue<Node*> levelqueue;

   std::cout << "\n level-order traversal of binary search tree" << std::endl;
   levelqueue.push(_root);
   int l = 1;
   while (!levelqueue.empty()) {
      Node *nodeout = (Node *) levelqueue.front();
      levelqueue.pop();
      std::cout << ' ' << nodeout;
      std::cout << ' ' << std::setw(48) << std::left << nodeout->_key;
      std::cout << std::setw(10) << nodeout->_left;
      std::cout << std::setw(10) << nodeout->_right << std::endl;
      if (nodeout->_left) levelqueue.push((Node *) nodeout->_left);
      if (nodeout->_right) levelqueue.push((Node *) nodeout->_right);
   }
}

void BST::inOrder() const
{
   std::cout << "<table>\n";
   inOrder(_root);
   std::cout << "</table>\n";
}

void BST::load(std::string* a) const
{
   int i = 0;
   load(_root, a, i);
}

void BST::loadIdxVector(std::vector<int>& v) const
{
   loadIdxVector(_root, v);
}

std::vector<int> BST::search(const std::string& key) const
{
   Node* found = search(_root, key);
   if (found != NULL)
      return found->_index;
   else {  // return dummy empty vector when nothing found
      std::vector<int> empty;
      return empty;
   }
}

int BST::height() const
{
   return _root->_height;
}

int BST::size() const
{
   return size(_root);
}

// protected BST class methods
// post-order traversal node deletion
void BST::destroy(Node*& p)
{
   if (!p) return;
   destroy(p->_left);
   destroy(p->_right);
   delete p;
}

int BST::node_height(const Node* node) const
{
   if ( !node ) return -1;
   return node->_height;
}

void BST::insert(Node*& p, const std::string& key, const int& idx)
{
   if ( !p ) { p = new Node(key, idx); return; }

   if (key < p->_key) insert(p->_left, key, idx);
   else if (key > p->_key) insert(p->_right, key, idx);
   else { p->_index.push_back(idx); return; }

   p->_height = 1 + std::max(node_height(p->_left), node_height(p->_right));
}

void BST::preOrder(Node *p) const
{
   if (p) {
      std::cout << ' ' << p->_key << std::endl;
      preOrder(p->_left);
      preOrder(p->_right);
   }
}

void BST::postOrder(Node *p) const
{
   if (!p) return;
   postOrder(p->_left);
   postOrder(p->_right);
   std::cout << ' ' << p->_key << std::endl;
}

void BST::inOrder(const Node* p) const
{
   if ( !p ) return;
   inOrder(p->_left);
   std::vector<std::string> fields;
   getFields(fields, p->_key);
   std::cout << "<tr>";
   for (int i = 0; i < fields.size(); i++) {
      std::cout << "<td>" << fields[i] << "</td>";
   }
   std::cout << "<tr>\n";
   inOrder(p->_right);
}

// sort a using in-order traversal
void BST::load(Node* p, std::string* a, int& i) const
{
   if (!p) return;
   load(p->_left, a, i);
   a[i++] = p->_key;
   load(p->_right, a, i);
}

// create an index to sorted values using an in-order traversal of the tree
void BST::loadIdxVector(Node* p, std::vector<int>& v) const
{
   if (!p) return;
   loadIdxVector(p->_left, v);
   // might be longer than one if there're duplicates
   for (unsigned int i = 0; i < p->_index.size(); i++)
      v.push_back(p->_index[i]);
   loadIdxVector(p->_right, v);
}

BST::Node* BST::search(Node *p, const std::string& key) const
{
   if (p == 0)
      return p;
   else if (key == p->_key)
      return p;
   else if (key < p->_key)
      return search(p->_left, key);
   else
      return search(p->_right, key);
}

int BST::h(Node *p) const
{
   if (!p) return -1;
   int hl = h(p->_left);
   int hr = h(p->_right);
   return 1 + std::max(hl, hr);
}

int BST::size(Node* p) const
{
   if ( !p ) return 0;
   return 1 + size(p->_left) + size(p->_right);
}
