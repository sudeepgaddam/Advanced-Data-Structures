#ifndef RBTREE_H
#define RBTREE_H

#include <math.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf
#include <fstream>
using namespace std;

//id and count value default to 0 in dummy node
#define DUMMY_VALUE 0

/*
 * 
 * 
 * Structure for RB Node
 * T is used for generic type
 * id is the id of the key
 * count is the counter
 * red is true if node is red
 * 
 * 
 */

template <class T>
class RBNode {
public:
    T id;
    int count;
    //red-  true if red, false if black
    bool red;
    //Left, right, p(parent) ptrs
    RBNode *left, *right, *p;
	//Set left, right ptr's to dummy initially
    RBNode(T id, int count, RBNode *p, RBNode<int> *dummy) : id(id), red(true), count(count),p(p),
                                                            left(dummy), right(dummy) {}
    ~RBNode() {
    }
};


/*
 * Properties Of Red-Black Tree
 * 1. Every node is either red or black
 * 2. Root is always black
 * 3. Leaf(NIL) is black
 * 4. If a node is red, Both its childen are black
 * 5. For each noode, All simple path from node to descendant leaves contain
 *  	same number of black nodes
 * */



template <class T>
class RBTree{
	
	
	private:
	
//    RBNode<T> *root;
    RBNode<T> *dummy;
    int BlackNodes;
    int MaxHeight;

	//Inrange helper is the recursive function which is used in Inrange
    void InRangeHelper(RBNode<T> *node, int ID1, int ID2, int &InRangeCounter);
    //Insert Related, ertfixup takes care of the Red-Black property after inserting the node
    void RBInsertFixup(RBNode<T> *z);

    //Rotate Related
    void LeftRotate(RBNode<T> *z);
    void RightRotate(RBNode<T> *z);

    //General Methods
    RBNode<T> *RBMin( RBNode<T> *root);
    RBNode<T> *RBMax( RBNode<T> *root);

    //Delete related
    void RBDelete(RBNode<T> *z);
    void RBTransplant(RBNode<T> *u, RBNode<T> *v);
    void RBDeleteFixup(RBNode<T> *z);

    //Search Recursive and iterative methods
    RBNode<T> * RBSearchRecursive(const RBNode<T> *u,T key);
    RBNode<T> * RBSearchIter(T key);


    //Max height and node count
    int setMaxHeight();
    int getNodeCount();
    
    //Helper method used to free all nodes in tree. Called in destructor
    void FreeTree(RBNode<T> *node) {
		if (node != this->dummy) {
			FreeTree(node->left);
			FreeTree(node->right);
			delete node;
		}
	}

public:
    //Test Functions, Used to test RB and BST properties
    bool testRBProperties(const RBNode<T> *curr,int CurrBlackNodes);
	int countBlackNodes();
	    RBNode<T> *root;
    //Traversal methods, Helper Methods
    void Inorder(const RBNode<T> *root);
    void levelOrder(const RBNode<T> *root);

	
	/*
	 *
	 * Constructor Method, Used to initialise RBTree
	 * @inputs: ptr to dummy node
	 * @return: NA
	 * @Description: RBTree has a dummy ptr which points to the same dummy node
	 * which all the nodes point to. This is used to check in various parts of the code
	 * to see if nodes point to dummy or not
	 * We set BlackNodes to 0 initially, This is the Black Node count in the path from root 
	 * to Descendant Node 
	 * 
	 * 
	 * */
    RBTree(RBNode<T> *dummy)
    {
        this->dummy = dummy;
        this->BlackNodes = 0;
        this->root= this->dummy;
        
     }

	void printTree(const RBNode<T> *node,ostream& out) {
        if (node->right != NULL) {
            printTree(node->right, out, true, "");
        }
        printNodeValue(node, out);
        if (node->left != NULL) {
            printTree(node->left, out, false, "");
        }
    }
    void  printNodeValue(const RBNode<T> *node,ostream& out)  {
		if(node->id == 0)
			out<<("extNode//B");
		else out<<(to_string(node->id)+getColor(node));
		out<<endl;
	 }

	 void printTree(){
	 	printTree(this->root, cout);
	 }

	 void printTree(const RBNode<T> *node, ostream& out, bool isRight, string indent)  {
		if (node->right != NULL) {
			printTree(node->right, out, true, indent + (isRight ? "        " : " |      "));
		}
		out<<(indent);
		if (isRight) {
			out<<" /";
		} else {
			out<<" \\";
		}
		out<<"----- ";
		printNodeValue(node,out);
		if (node->left != NULL) {
			printTree(node->left, out, false, indent + (isRight ? " |      " : "        "));
		}
	}

	/*
	 *
	 * Destructor Method, Used to free all nodes in RBTree
	 * @inputs: 
	 * @return: NA
	 * @Description:  Calls FreeTree method which frees the entire tree
	 * starting with root
	 * 
	 * 
	 * */

    ~RBTree(){
		FreeTree(this->root);
	 }
    
    


    //Project related - Public methods which will be called by the commands 
    //used to test the project. These are made public
    void Increase(int ID, int m);
    void Reduce(int ID, int m);
    int Count(T ID);
    int InRange(int ID1, int ID2);
    void Next(int ID);
    void Previous(int ID);
    //void Display(const RBNode<T> *current, int indent);
    void colorTree(RBNode<T> *node , int lvl);
	void initialise(vector<RBNode<T> *> &arr);

    void RBInsert(RBNode<T> *z);
    RBNode<T> *sortedArrayToBST(vector<RBNode<T> *> &arr, int start, int end);



};
/*
 *
 * InRangeHelper
 * @inputs: RBNode<T> ptr,  id1,  id2, int
 * @return: -
 * @Description: Helper function called from InRange
 * If id1 less than node id, Go left
 * If id2 greater than node id, Go right
 * If node id between id1 and id2, then increment the counter
 * 
 * */
template <class T>
void RBTree<T>::InRangeHelper(RBNode<T> *node, int id1, int id2, int &InrangeCounter){
	   if ( this->dummy == node )
      return;
 
   if ( id1 < node->id )
     InRangeHelper(node->left, id1, id2, InrangeCounter);
   if ( id1 <= node->id && id2 >= node->id )
     InrangeCounter+=node->count;
   if ( id2 > node->id )
     InRangeHelper(node->right, id1, id2 , InrangeCounter);

}

/*
 *
 * InRange
 * @inputs: int id1, int id2
 * @return:
 * @Description: Count number of nodes, that are within range id1 and id2
 * calls InRangeHelper()
 * 
 * */
 
template <class T>
int RBTree<T>::InRange(int id1, int id2){
	 int InrangeCounter = 0;
	 InRangeHelper(root, id1,id2,InrangeCounter);
	 cout<< InrangeCounter << endl;
	
	return InrangeCounter;
}

/*
 *
 * setMaxHeight
 * @inputs: void
 * @return: int
 * @Description: Assumes a full binary tree, Traverses only left, 
 * That will be max height
 * 
 * */
template <class T>
int RBTree<T>:: setMaxHeight() {
	auto node = this->root;
	int count = 0;
	while(node !=dummy) {
		count++;
		node = node->left;
	}
	this->MaxHeight = count;
	return count;
}
/*
 *
 * countBlackNodes
 * @inputs: void
 * @return: int
 * @Description: Start from Root, count num Black nodes to the descendant leaf
 * 
 * 
 * */
template <class T>
int RBTree<T>::countBlackNodes(){
        int numBlack = 0;
        auto node = this->root;
        while(node != this->dummy){
            if(!(node->red)) numBlack++;
            node = node->left;
        }
        this->BlackNodes = numBlack;
        return numBlack;
}
/*
 *
 * testRBProperties
 * @inputs: const RBNode<T> *x, int CurrBlackNodes
 * @return: bool, True if success, False if RBproperties not satisfied
 * @Description: CurrBlackNodes will be 0 when passed initially
 * 
 * It tests property 4 and 5 mentioned above and also tests the BST property
 * 
 * 
 * */
template <class T>
bool RBTree<T>::testRBProperties(const RBNode<T> *x, int CurrBlackNodes){
		if(CurrBlackNodes > this->BlackNodes) return false;
		if(x!=this->dummy){
			if((x->left!=this->dummy && x->left->id >= x->id)|| (x->right!=this->dummy && x->right->id < x->id)) {
				cout<< "BST property not valid" <<endl;
				return false;
			}
			if(x->red) {
				if(!(x->left->red) && !(x->right->red)) {
					return testRBProperties(x->left,CurrBlackNodes) && testRBProperties(x->right,CurrBlackNodes);
				} else {
					cout<< "Red Doesn't has two Black children, Violation of RB property" <<endl;

					return false;
				}
				
			} else {
				return testRBProperties(x->left,CurrBlackNodes+1) && testRBProperties(x->right,CurrBlackNodes+1);
			}
			
		} else {
			if(CurrBlackNodes != this->BlackNodes) {
				cout<< "Root to descendant paths has unequal black node count, Violation of RB property" <<endl;

				return false;
			}
			
		}
		//cout<<"Its valid" << endl;
		return true;
}
/*
 *
 * Get Min node, Given the root
 * @inputs: RBNode<T> *curr
 * @return: RBNode<T> *
 * @Description: Keep on going Left till the Left is not dummy
 * That will be the Min
 * 
 * 
 * */
template <class T>
 RBNode<T> * RBTree<T>::RBMin( RBNode<T> *curr) {
    while(curr->left!=this->dummy) {
        curr= curr->left;
    }
    return curr;
}


/*
 *
 * Get Max node, Given the root
 * @inputs: RBNode<T> *curr
 * @return: RBNode<T> *
 * @Description: Keep on going right till the right is not dummy
 * That will be the Max
 * 
 * 
 * */
template <class T>
RBNode<T> * RBTree<T>::RBMax( RBNode<T> *curr) {
    while(curr->right!=this->dummy) {
        curr = curr->right ;
    }
    return curr;
}
/*
 *
 * Recursive search
 * @inputs:
 * @return:
 * @Description: Helper function , Not related to project
 * 
 * 
 * */
template <class T>
RBNode<T> * RBTree<T>::RBSearchRecursive(const RBNode<T> *x,T key) {

    if((!x) || (x->id == key)) return x;
    if(key < x->id) return RBSearch(x->left, key);
    else return RBSearch(x->right, key);
}

/*
 *
 * Iterative search
 * @inputs: key
 * @return: RBNode<T> * 
 * @Description: iteratively search based on the key value
 * */
template <class T>
RBNode<T> * RBTree<T>::RBSearchIter(T key) {
    auto x =this->root;

    while((x!=this->dummy) && (key!=x->id)) {
        if(key < x->id) x = x->left;
        else x=x->right;
    }
    return x;
}
/*
 *
 * Recursive Inorder Traversal
 * @inputs:RBNode<T> *curr
 * @return: void
 * @Description: Helper function, Not related to project
 * 
 * 
 * */

template <class T>
void RBTree<T>::Inorder(const RBNode<T> *curr) {
    if(curr!=this->dummy) {
        Inorder(curr->left);
        cout<<"Count: "<<curr->count<<" ID: "<<curr->id << " Red?: " << curr->red<<endl;
        Inorder(curr->right);
    }

    return;
}

/*
 *
 * RBtransplant - Used in Delete
 * @inputs: RBNode<T> *u, RBNode<T> *v
 * @return: void
 * @Description: Transplants u with v
 * If u's parent is dummy, then root will be v
 * If u is left child, V is the new child.
 * If u is right child, V is the new child
 * v's parent will be u's parent
 * 
 *
 * */
template <class T>
void RBTree<T>::RBTransplant(RBNode<T> *u, RBNode<T> *v ) {


    if((u->p) == this->dummy) root=v;
    else if (u == u->p->left) u->p->left = v;
    else u->p->right = v;
    //if(v)
    v->p = u->p;
    //cout<<"Transplant done" <<endl;
    return;
}
/*
 *
 * RBDeleteFixup - Used in Delete, To take care of RB properties
 * @inputs: RBNode<T> *x
 * @return: void 
 * @Description: 
 * 2 outer cases, If x is left child or x is right child, within these two cases, 
 * 4 cases arise, 
 * x's sibling w is red, 
 * x's sibling w is black of both of w's children are black
 * x's sibling w is black, w;s left chld is red and w's right child is black
 * x's sibling w is black and w's right child is red.
 * 
 * For case 1, make w color Black and x's parent color red 
 * and Left rotate on X's parent
 * For case 2, make w color Red and make X as X's parent and run the while loop
 * For case 3, make W's left color as black and w's color as Red and Right rotate on W
 * and W will be w's uncle now.
 * For case 3 and 4,  make w's color x's parent color 
 * x's parent color  black
 * and Left rotate on x's parent
 * 
 * 
 *
 * */
template <class T>
void RBTree<T>::RBDeleteFixup(RBNode<T> *x) {

    while( (x!=this->root) && (!(x->red))) {
        if(x == x->p->left) {
            //cout<<"Main if part" << endl;
            auto w = x->p->right;
            if(w && w->red) {
                //cout<<"W is red"<<endl;
                w->red = false;
                x->p->red = true;
                LeftRotate(x->p);
                w = x->p->right;

            }
            if(!(w->left->red) && !(w->right->red)) {
                w->red = true;
                x = x->p;


            } //Right black
            else {
                if(!(w->right->red)) {
                    w->left->red = false;
                    w->red = true;
                    RightRotate(w);
                    w = x->p->right;

                }

                w->red = x->p->red;
                x->p->red = false;
                w->right->red = false;
                LeftRotate(x->p);
                x = root;
            }

        } else {
            //Main else paprt
            //cout<<"Main else part" <<endl;

            auto w = x->p->left;
            if(w && w->red) {
                w->red = false;
                x->p->red = true;
                RightRotate(x->p);
                w = x->p->left;

            }
            if(!(w->right->red) && !(w->left->red)) {
                w->red = true;
                x = x->p;


            } else {

                if(!(w->left->red)) {
                    w->right->red = false;
                    w->red = true;
                    LeftRotate(w);
                    w = x->p->left;
                }
                w->red = x->p->red;
                x->p->red = false;
                w->left->red = false;
                RightRotate(x->p);
                x = root;
            }
        }
    }
    x->red = false;

    return;
}
/*
 *
 * @inputs: RBNode<T> *z
 * @return: void
 * @Description: Delete a node z from RBTree, Delete as it is from a 
 * normal BST using Transplant which takes care of substituting z with the 
 * child node if its of degree 1, and if its degree 2, Then substitute with 
 * z's succesor. Its taken care in RBTransplant.
 * After doing normal BST delete, Call RBDelete Fixup to take care of the RBProperty
 * 
 *
 * */
template <class T>
void RBTree<T>::RBDelete(RBNode<T> *z) {
    auto y =  z;
    bool y_real_col = y->red;
    RBNode<T> *x;
    if((z->left)==this->dummy) {
        //cout<<"Delete, Left Null" <<endl;
        x  = z->right;
        RBTransplant(z,z->right);

    } else if((z->right)==this->dummy) {
        //cout<<"Delete, Right Null" <<endl;

        x  = z->left;
        RBTransplant(z,z->left);
    } else {
       // cout<<"Delete, Left and Right non Null ";

        y = RBMin(z->right);
        //cout << y->id << endl;
        y_real_col = y->red;
        x = y->right;
        if(y->p == z) {
            x->p = y;
        } else {
			//auto  minNodeParent = y->p;
			//cout<< "y->id " << y->id << "y->right->id " << y->right->id << endl;
            RBTransplant(y,y->right);
            this->dummy->p = NULL;
            y->right = z->right;
            y->right->p = y;
            //minNodeParent->left = this->dummy;

        }
        RBTransplant(z,y);
        //cout<< z->id << y->id <<" " ;
        y->left = z->left;
        y->left->p = y;
        y->red = z->red;
        //cout<< "y->id " << y->id << "y->right->id " << y->right->id << endl;

        if(x == this->dummy) x = y;
    }

    //If black
    if(!y_real_col) {
		//if(x == this->dummy) x = y;
        RBDeleteFixup(x);

    }
    delete z;
    this->dummy->p = NULL;
    this->dummy->left = NULL;
    this->dummy->right = NULL;

    return;
}

    /*
     *  This function is used to rotate counter clockwise with respect to x
     *      |                      |
     *      x                      y
     *     / \                    / \
     *    s  y      to           x  u
     *      / \                 / \
     *     t   u               s   t
     * 
     * This takes care of preserving the BST property
     */
/*
 *
 * LeftRotate 
 * @inputs:
 * @return:
 * @Description:
 * 
 *
 * */
template <class T>
void RBTree<T>::LeftRotate(RBNode<T> *x) {
    auto y  = x->right;
    //cout<<"In Left rotate:  x-id: "<< x->id << endl;
    x->right = y->left;
    if((y->left)!=(this->dummy)) y->left->p = x;
    y->p  = x->p;
    if((x->p) ==dummy) this->root = y;
    else if (x == x->p->left) x->p->left = y;
    else x->p->right = y;
    y->left  = x;
    x->p = y;
    return;
}

    /*
     *  This function is used to rotate clockwise with respect to x
     *      |                      |
     *      x                      y
     *     / \                    / \
     *    y  u      to           s  x
     *   / \                       / \
     *  s  t                      t   u
     * 
     * This takes care of preserving the BST property
     */
/*
 *
 * RightRotate 
 * @inputs: RBNode<T> *x
 * @return: void
 * @Description: Rotates, Tree rooted at x as shown in above picture
 * while preserving the BST property
 * 
 *
 * */

template <class T>
void RBTree<T>::RightRotate(RBNode<T> *x) {
    auto y  = x->left;
    x->left = y->right;
    if((y->right)!=(this->dummy)) y->right->p = x;
    y->p  = x->p;
    if((x->p) ==this->dummy) root = y;
    else if (x == x->p->right) x->p->right = y;
    else x->p->left = y;
    y->right  = x;
    x->p = y;
    return;


}

/*
 *
 * RBInsert 
 * @inputs: RBNode<T> *z
 * @return: void
 * @Description: Inserts a node z at the right place according to 
 * BST property, After inserting, It calls Fixup method which takes 
 * care of Preserving RB properties
 * 
 *
 * */
template <class T>
void RBTree<T>::RBInsert(RBNode<T> *z) {
    RBNode<T> *y = this->dummy;
    auto x = this->root;
    //cout<<"In RBInsert"<<endl;

    while(x != (this->dummy)) {
        //cout<<"Inside while loop" << endl;
        y=x;
        if(z->id < x->id) x = x->left;
        else x = x->right;
    }
    //cout<<"end of while loop"<<endl;
    z->p = y;
    if (y==this->dummy) root= z;
    else if (z->id < y->id) y->left = z;
    else y->right = z;
    //cout<<"Before RBInsertFixup"<<endl;
    RBInsertFixup(z);
    return;
}
/*
 *
 * RBInsertFixup 
 * @inputs: RBNode<T> *z
 * @return: void
 * @Description: Two outer cases, If Z's parent is left child or 
 * Z's parent is right child of its parent. 
 * Within that, three cases, 1.z's uncle  y is red
 *  2.Z's uncle y is black and z is a right child
 * 3. z's uncle y is black and z is a left child. 
 * For case 1, Make z's parent and y's color black and 
 * z's grand parent red. and make z as z's GP.
 * For case 2, Do left rotation on Z's parent
 * 
 * For case 2 & 3,  Make Z's parent color black,
 * z's Grand parent Red, Right rotate on Z's Grand Parent.
 * 
 *
 * */
template <class T>
void RBTree<T>::RBInsertFixup(RBNode<T> *z) {
    //cout<<"Insert fixup for " << z->id << endl;
    while(z->p->red) {
        if (z->p == z->p->p->left) {
            auto y = z->p->p->right;

            if(y->red) {
                z->p->red = false;
                y->red = false;
                z->p->p->red = true;
                z= z->p->p;
            } else{
                if(z == z->p->right) {
                    z = z->p;
                    LeftRotate(z);
                }
                z->p->red = false;
                z->p->p->red = true;
                RightRotate(z->p->p);

            }
        } else {
            auto y = z->p->p->left;

            if(y->red) {
                //cout<<"Uncle red"<<endl;
                z->p->red = false;
                y->red = false;
                z->p->p->red = true;
                z= z->p->p;
            } else{

                if(z == z->p->left) {
                    z = z->p;
                    RightRotate(z);
                }

                z->p->red = false;
                z->p->p->red = true;
                LeftRotate(z->p->p);

            }
        }
    }

    root->red = false;



}

/*
 *
 * levelOrder, Helper function
 * @inputs: RBNode<T>* root
 * @return: void
 * @Description: Prints the nodes, Level by level, Using queue to insert
 * the nodes, It is just a helper function used in debugging. Its not related to 
 * project directly
 * 
 *
 * */
template <class T>
void RBTree<T>::levelOrder(const RBNode<T>* root) {
    if (!root) { return ; }
    vector<RBNode<T> *> row;
    vector<vector<RBNode<T> *> > result;
    queue<RBNode<T> *> q;
    q.push(root);
    int count = 1;

    while (!q.empty()) {
        if (q.front()->left!=this->dummy) { q.push(q.front()->left); }
        if (q.front()->right!=this->dummy) { q.push(q.front()->right); }
        row.push_back(q.front()), q.pop();
        if (--count == 0) {
            result.emplace_back(row), row.clear();
            count = q.size();
        }
    }

    for(auto a : result) {
        for(auto b : a) {
            cout<<"Count: "<<b->count<<" ID: "<<b->id << " Red?: " << b->red<<endl;
        }
        cout<<"--------"<<endl;
    }

    return;

}
/*
 *
 * Increase the count of the event theID by m. If theID is not present, insert it. Print the count
 *	of theID after the addition.
 * @inputs:
 * @return:
 * @Description:
 *
 */
template <class T>
void RBTree<T>::Increase(int ID, int m) {
	//if(m==0) return;
    auto res  = RBSearchIter( ID);
    if(this->dummy == res) {
        //If node not found, Insert
        RBNode<T> *node = new RBNode<int>(ID,m,dummy,dummy);
        RBInsert(node);
        cout<< m<< endl;

    } else {
        res->count+=m;
        cout<<  res->count<<endl;

    }
}
/*
 *
 * Decrease the count of theID by m. If theID’s count becomes less than or equal to 0, remove
 *	theID from the counter. Print the count of theID after the deletion, or 0 if theID is
 *	removed or not present.
 * @inputs:
 * @return:
 * @Description:
 *
 */
template <class T>
void RBTree<T>::Reduce(int ID, int m) {
    auto res  = RBSearchIter(ID);

    if(res==this->dummy) {
        //not present
        cout<<"0" << endl;
    } else {
        res->count-=m;
        if(res->count>0) {
            cout<< res->count<< endl;
        } else {
           // cout<< "Count = 0, After reducing, Deleting node" << res->id << endl;
            cout<<"0" << endl;
            RBDelete(res);
        }
    }

}
/*
 *
 *	Count of node with key id
 * @inputs: id of the node
 * @return: int, Counter of the node
 * @Description: For a given id, returns the count corresponding to that id
 * 	 Does iteraive search, If the id is found, then returns the count of the node found,
 * Otherwise, returns 0
 *
 */
template <class T>
int RBTree<T>::Count(T id) {

    auto res  = RBSearchIter(id);
    if(!res) {
        cout<<"0"<<endl;
        return 0;
    }

    cout<<res->count<<endl;
    return res->count;
}


/*
 * Next
 * @inputs: int key
 * @return: void
 * @Description: prints the Succesor of key in BST, It tries to search for key first, 
 * If it is found, It will go up the tree, till it finds greater than key
 * If it is not found, It will go up from external node till it finds the node
 * that is greater than key
 *
 */ 
 template <class T>
void RBTree<T>::Next(int key) {
    auto x =this->root;
    auto y = x;
    //Search iteratively first
    while((x!=this->dummy) && (key!=x->id)) {
        y = x;

        if(key < x->id) x = x->left;
        else x=x->right;
    }
    //If x is dummy, Go up till u get greater number
    if(x==this->dummy) {
		if(y->id > key) {
			//cout<<"Next for key: "<<key;
			cout<<y->id<<" "<<y->count<<endl;
		} else {
			//Go up till u get greater number than key
			auto par = y->p;
			while(par != this->dummy && par->id<=key) {
				y = par;
				par = par->p;
			}
       // cout<<"Next for key: "<<key;
        //cout<<"ID: "<<par->id<<"Count: "<<par->count<<endl;
        cout<<par->id<<" "<<par->count<<endl;
		}	
	} else {
		if(x->right == this->dummy) {
			auto par = x;
			while(par != this->dummy && par->id<=key) {
				x = par;
				par = par->p;
			}
			//cout<<"Next for key: "<<key;
			cout<<par->id<<" "<<par->count<<endl;

//			cout<<"ID: "<<par->id<<"Count: "<<par->count<<endl;
		} else {
        auto z = RBMin(x->right);
       // cout<<"Next for key: "<<key;

        cout<<z->id<<" "<<z->count<<endl;
		}
	}
    return;

}

/*
 * Previous
 * @inputs: int key
 * @return: void
 * @Description: prints the predecessor of key, It tries to search for key first, 
 * If it is found, It will go up the tree, till it finds lesser than key
 * If it is not found, It will go up from external node till it finds the node
 * that is lesser than key
 *
 */
 
 template <class T>
void RBTree<T>::Previous(int key) {
    auto x =this->root;
    auto y = x;
    //Search iteratively first
    while((x!=this->dummy) && (key!=x->id)) {
        y = x;

        if(key < x->id) x = x->left;
        else x=x->right;
    }
    //If x is dummy, Go up till u get greater number
    if(x==this->dummy) {
		if(y->id < key) {
			//cout<<"Prev for key: "<<key;
        cout<<y->id<<" "<<y->count<<endl;
		} else {
			//Go up till u get greater number than key
			auto par = y->p;
			while(par != this->dummy && par->id>=key) {
				y = par;
				par = par->p;
			}
        //cout<<"Prev for key: "<<key;
        cout<<par->id<<" "<<par->count<<endl;
		}	
	} else {
		if(x->left == this->dummy) {
			auto par = x;
			while(par != this->dummy && par->id>=key) {
				x = par;
				par = par->p;
			}
			//cout<<"Prev for key: "<<key;
        cout<<par->id<<" "<<par->count<<endl;
		} else {
        auto z = RBMax(x->left);
        //cout<<"Prev for key: "<<key;

        cout<<z->id<<" "<<z->count<<endl;
		}
	}
    return;
}
/*
 * sortedArrayToBST
 * @inputs: vector<pair<int,int>> &arr, int start, int end
 * @return: RBNode<T> * 
 * @Description: Takes a vector of <key,count> and builds balanced BST recursively,
 * Root will be middle element
 * Left will point to root of left half
 * Right will point to root of right half
 *
 */

template <class T>
RBNode<T> * RBTree<T>::sortedArrayToBST(vector<RBNode<T> *> &arr, int start, int end)
{
    /* Base Case */
    if (start > end)
      return this->dummy;
 
    int mid = start + (end-start)/2;
    RBNode<T>  *node = arr[mid];

    node->left =  sortedArrayToBST(arr, start, mid-1);
    if(node->left != this->dummy)
    	node->left->p = node;
    node->right = sortedArrayToBST(arr, mid+1, end);
    if(node->right != this->dummy)
    	node->right->p = node; 
    return node;
}

/*
 * 
 *  @inputs: vector<pair<int,int>> &arr
 * @return: void

 * This fuction will be called from Main
 * It will inturn call sortedArrayToBST and it assigns root
 * 
 * It will also take care of coloring nodes, Red and black at alternate levels
 * 
 * 
 */ 
 
template <class T>
void RBTree<T>::initialise(vector<RBNode<T> *> &arr)
{
	this->root = sortedArrayToBST(arr, 0, arr.size()-1);
	this->root->red = false;
	this->setMaxHeight();
	colorTree(this->root, 0);
	this->dummy->red = false;


}

/*
 * colorTree
 * @inputs: RBNode<T> *node
 * @return: void
 * @Description: 
 *
 */
template <class T>
void RBTree<T>::colorTree(RBNode<T> *node, int lev )
{
    if (node == NULL || node == this->dummy)
        return;
    if(lev == this->MaxHeight)
    	node->red = true;
    else
    	node->red = false;
   	colorTree(node->left,lev+1);
    colorTree(node->right,lev+1);
	return;

}
#endif

