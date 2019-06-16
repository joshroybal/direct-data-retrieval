#ifndef BST_HPP
#define BST_HPP

#include <vector>

class BST
{  
   public:
      BST() : _root(0) {}
      ~BST() { destroy(_root); }
      void insert(const std::string&, const int&);
      void preOrder() const;
      void postOrder() const;
      void levelOrder();
      void inOrder() const;
      void inOrder(std::ofstream&) const;
      void load(std::string*) const;
      void loadIdxVector(std::vector<int>&) const;
      std::vector<int> search(const std::string&) const;
      int height() const;
      int size() const;
   protected:
      struct Node;
      Node* _root;
      void destroy(Node*&);
      int node_height(const Node*) const;
      void insert(Node*&, const std::string&, const int&);
      void preOrder(Node*) const;
      void postOrder(Node*) const;
      void inOrder(const Node*) const;
      void load(Node*, std::string*, int&) const;
      void loadIdxVector(Node*, std::vector<int>&) const;
      Node* search(Node*, const std::string&) const;
      int h(Node *) const;
      int size(Node*) const;
};

// Node definition
struct BST::Node
{  
   std::string _key;
   std::vector<int> _index;
   int _height;
   Node* _left;
   Node* _right;
   Node(const std::string& key, int idx_no) : _key(key), _height(0),
      _left(0), _right(0) { _index.push_back(idx_no); }
};

#endif
