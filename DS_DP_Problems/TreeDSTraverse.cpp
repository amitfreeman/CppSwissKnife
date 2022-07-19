#include <iostream>
using namespace std;

// Tree traversal in C++
/*       1
    12       9
5       6
*/
/*
inorder -> left - root -right
           5 12 6 1 9
preorder ->  root - left -right
            1 12 5 6 9
postorder -> left -  right -root
            5 6 12 9 1
*/

/* Define node struct, and its constructor */
 struct node {
  int item;
  struct node* left;
  struct node* right;

  node(int data){
	  this->item=data;
	  left=right=nullptr;
  }
};

void inorderTraversal(node* root){
	if(root==nullptr)
		return;

	inorderTraversal(root->left);
	cout<<root->item<<" -> ";
	inorderTraversal(root->right);
}

void preorderTraversal(node* root){
	if(root==nullptr)
		return;

	cout<<root->item<<" -> ";
	preorderTraversal(root->left);
	preorderTraversal(root->right);
}

void postorderTraversal(node* root){
	if(root==nullptr)
		return;

	postorderTraversal(root->left);
	postorderTraversal(root->right);
	cout<<root->item<<" -> ";
}

/* post order used, as to delete root node at last */
void deleteTree(node* root){
	if(root==nullptr)
		return;

	deleteTree(root->left);
	deleteTree(root->right);
	delete root;
}

int calcTreeHeight(node* root){
	if(!root)
	   return 0;

	int left=calcTreeHeight(root->left);
	int right=calcTreeHeight(root->right);

	return max(right, left)+1;
}

int main() {
	cout << "---Starting, creating nodes---" << endl;

	node* root=new node(1);
	root->left=new node(12);
	root->right=new node(9);
	root->left->left=new node(5);
	root->left->right=new node(6);

	cout<<"Height of Tree is: "<<calcTreeHeight(root)<<endl;

	cout << "Inorder traversal ";
	inorderTraversal(root);

	cout << "\nPreorder traversal ";
	preorderTraversal(root);

	cout << "\nPostorder traversal ";
	postorderTraversal(root);
	
	/* delete tree nodes */
	cout<<"\nDeleting Tree"<<endl;
	deleteTree(root);

	return 0;
}