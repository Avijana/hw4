#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
		void removeFix(AVLNode<Key, Value>* n, int8_t diff);


};


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
	if((p->getBalance() == -1 || p->getBalance() == 0 || p->getBalance() == 1) && (n->getBalance() == -1 || n->getBalance() == 0 || n->getBalance() == 1)){
	AVLNode<Key, Value>* g = p->getParent();
	if(p != NULL && g != NULL)
	{
		if(p == g->getLeft())
		{
			g->updateBalance(-1);
			if(g->getBalance()==0)
			{
				return;
			}
			else if(g->getBalance()==-1)
			{
				insertFix(g,p);
			}
			else if(g->getBalance()==-2)
			{
				if(p->getLeft() == n) 
				{
					rotateRight(g);
					g->setBalance(0);
					p->setBalance(0);
				}
				else
				{
					rotateLeft(p);
					rotateRight(g);
					if(n->getBalance()==-1)
					{
						n->setBalance(0);
						p->setBalance(0);
						g->setBalance(1);
					}
					else if(n->getBalance()==0)
					{
						g->setBalance(0);
						p->setBalance(0);
						n->setBalance(0);
					}
					else
					{
						n->setBalance(0);
						p->setBalance(-1);
						g->setBalance(0);
					}
				}
			}
		}
		else if(p==g->getRight())
		{
			g->updateBalance(1);
			if(g->getBalance()==0)
			{
				return;
			}
			else if(g->getBalance()==1)
			{
				insertFix(g,p);
			}
			else if(g->getBalance()==2)
			{
				if(p->getRight() == n) 
				{
					rotateLeft(g);
					g->setBalance(0);
					p->setBalance(0);
				}
				else
				{
					rotateRight(p);
					rotateLeft(g);
					if(n->getBalance()==1)
					{
						n->setBalance(0);
						p->setBalance(0);
						g->setBalance(-1);
					}
					else if(n->getBalance()==0)
					{
						g->setBalance(0);
						p->setBalance(0);
						n->setBalance(0);
					}
					else
					{
						n->setBalance(0);
						p->setBalance(1);
						g->setBalance(0);
					}
				}
			}
		}
	}
}
}



/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
	if(this->root_ == NULL) //if tree empty create root and set balance to 0
	{
		AVLNode<Key, Value>* r = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL); 
		this->root_ = r;
		r->setBalance(0);
		return;
	}
  AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value> *>(this->root_);

  while(current != NULL)
  {
		if(new_item.first < current->getKey())
		{
			if(current->getLeft() == NULL) //insert left set balance
            {
                AVLNode<Key, Value>* iNode = new AVLNode<Key,Value>(new_item.first, new_item.second, current);
                current->setLeft(iNode);
								iNode->setBalance(0);
								if(iNode->getParent()->getBalance()== 1 || iNode->getParent()->getBalance()== -1)
								{
									iNode->getParent()->setBalance(0);
								}
								else
								{
									iNode->getParent()->updateBalance(-1);
									insertFix(iNode->getParent(), iNode);
								}
                return;
            }
            else
            {
                current = current->getLeft();
            }
		}
		else if(new_item.first > current->getKey()) //insert right set balance
		{
			if(current->getRight() == NULL)
            {
                AVLNode<Key, Value>* iNode = new AVLNode<Key,Value>(new_item.first, new_item.second, current);
                current->setRight(iNode);
								iNode->setBalance(0);
								if(iNode->getParent()->getBalance()==-1 || iNode->getParent()->getBalance()== 1)
								{
									iNode->getParent()->setBalance(0);
								}
								else
								{
									iNode->getParent()->updateBalance(1);
									insertFix(iNode->getParent(), iNode);
								}
                return;
            }
            else
            {
                current = current->getRight(); 
            }
		}
		else//if(keyValuePair.first == current->getKey())
		{
			current->setValue(new_item.second); //update value if key is same
			return;
		}
	}
}



template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
	if(node == NULL)
	{
    return;
  }

  AVLNode <Key, Value>* child = node->getLeft();
  AVLNode <Key, Value>* p = node->getParent();
  
	if(child == NULL)
	{
      return;
  }

  node->setLeft(child->getRight());
	if(child->getRight() != nullptr){
	child->getRight()->setParent(node); //point new roots right child to old root
	}
  
	if (p == NULL)
	{
    BinarySearchTree<Key, Value>::root_ = child;
  }
	else if(p->getLeft() == node)
	{
    p->setLeft(child);
  }
	else if(p -> getRight() == node)
	{
    p->setRight(child);
  }

  child->setParent(p); //set pointers
  child->setRight(node);
  node->setParent(child);
  return;
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
	if(node == NULL)
	{
    return;
  }

  AVLNode <Key, Value>* child = node->getRight();
  AVLNode <Key, Value>* p = node->getParent();
  
	if(child == NULL)
	{
      return;
  }

  node->setRight(child->getLeft());
	if(child->getLeft() != nullptr)
	{
  	child->getLeft()->setParent(node); //points new roots left child to old root
	}
	if(p == NULL)
	{
    BinarySearchTree<Key, Value>::root_ = child;
  }
	else if(p->getRight() == node)
	{
    p->setRight(child);
  }
	else if(p->getLeft() == node)
	{
    p->setLeft(child);
  }

  child->setParent(p); //set pointers
  child->setLeft(node);
  node->setParent(child);
	//p->setParent(node);
  return;
}

template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
    // TODO
		int nDiff = 0;


		if(n == NULL)
		{
			return;
		}

		AVLNode<Key, Value>* p = n->getParent();

		if(p != NULL)
		{
			if(n == p->getLeft())
			{
				nDiff = 1;
			}
			else
			{
				nDiff = -1;
			}
		}

		if(diff == -1)
		{
			if(n->getBalance() + diff == -2)
			{
				AVLNode<Key, Value>* c = n->getLeft();
				if(c->getBalance() == -1)
				{
					rotateRight(n);
					n->setBalance(0);
					c->setBalance(0);
					removeFix(p,nDiff);
				}
				else if(c->getBalance() == 0)
				{
					rotateRight(n);
					n->setBalance(-1);
					c->setBalance(1);
				}
				else if(c->getBalance()==1)
				{
					AVLNode<Key, Value>* g = c->getRight();
					rotateLeft(c);
					rotateRight(n);
					if(g->getBalance() == 1)
					{
						n->setBalance(0);
						c->setBalance(-1);
						g->setBalance(0);
					}
					else if(g->getBalance() == 0)
					{
						n->setBalance(0);
						c->setBalance(0);
						g->setBalance(0);
					}
					else if(g->getBalance() == -1)
					{
						n->setBalance(1);
						c->setBalance(0);
						g->setBalance(0);
					}
					removeFix(p, nDiff);
				}
			}
			else if(n->getBalance()+diff == -1)
			{
				n->setBalance(-1);
			}
			else if(n->getBalance()+diff == 0)
			{
				n->setBalance(0);
				removeFix(p,nDiff);
			}
		}
		else if(diff == 1)
		{
			if(n->getBalance() + diff == 2)
			{
				AVLNode<Key, Value>* c = n->getRight();
				if(c->getBalance() == 1)
				{
					rotateLeft(n);
					n->setBalance(0);
					c->setBalance(0);
					removeFix(p,nDiff);
				}
				else if(c->getBalance() == 0)
				{
					rotateLeft(n);
					n->setBalance(1);
					c->setBalance(-1);
				}
				else if(c->getBalance()==-1)
				{
					AVLNode<Key, Value>* g = c->getLeft();
					rotateRight(c);
					rotateLeft(n);
					if(g->getBalance() == -1)
					{
						n->setBalance(0);
						c->setBalance(1);
						g->setBalance(0);
					}
					else if(g->getBalance() == 0)
					{
						n->setBalance(0);
						c->setBalance(0);
						g->setBalance(0);
					}
					else if(g->getBalance() == 1)
					{
						n->setBalance(-1);
						c->setBalance(0);
						g->setBalance(0);
					}
					removeFix(p, nDiff);
				}
			}
			else if(n->getBalance()+diff == 1)
			{
				n->setBalance(1);
			}
			else if(n->getBalance()+diff == 0)
			{
				n->setBalance(0);
				removeFix(p,nDiff);
			}
		}
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO

	if(this->root_ == NULL) //if tree empty return
	{
		return;
	}

	AVLNode<Key, Value> *current = static_cast<AVLNode<Key, Value> *>(this->internalFind(key));

	AVLNode<Key, Value>* temp;
	
  if(current == NULL)
	{
        return;
	}
	

  

	if((current->getLeft() != NULL && current->getRight() != NULL)) //if two children
	{
		nodeSwap(current, static_cast<AVLNode<Key, Value> *>(this->predecessor(current)));
	}

	int8_t diff = 0; 
	AVLNode<Key, Value>* p = current->getParent();
	if(p!=NULL)
	{
		if(current == p->getLeft())
		{
			diff = 1;
		}
		else if(current == p->getRight())
		{
			diff = -1;
		}
	}

	if(current->getLeft() != NULL && current->getRight() == NULL) //if one child
	{
		temp = current->getLeft();
	}
	else if(current->getLeft() == NULL && current->getRight() != NULL) //if one child
	{
		temp = current->getRight();
	}
	else if(current->getLeft() == NULL && current->getRight() == NULL) //if no children
	{
		temp = NULL;
	}

	
    if(p == NULL)
		{
        this->root_ = temp; 
    } 
		else
		{
      if (p->getRight() == current) {
            p->setRight(temp); 
      }
			else if (p->getLeft() == current) {
            p->setLeft(temp); 
      }
    }
    if (temp != NULL) {
        temp->setParent(p); 
    }
    delete current; 
		removeFix(p, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
