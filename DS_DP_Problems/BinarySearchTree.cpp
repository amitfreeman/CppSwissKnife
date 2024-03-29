/*============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Amit Paropkari
// Version     :
// Description : BST implementation
//============================================================================*/

/*
 *              8
 *          3       10
 *      1       6        14
 *           4     7
 */

#include <iostream>
using namespace std;

struct node{
	int data;
	node* left;
	node* right;

	node(int i){
		this->data=i;
		left=right=nullptr;
	}
};

/* create new node */
node* newNode(int i){
	node* newN=new node(i);
	return newN;
}

/* insert new node in tree
 *  check order recursively, create new and maintain order
 *  using returning same existing node upwards
 */
node* insertNode(node* root, int data){
	if(root == NULL) return newNode(data);

	if(data < root->data)
		root->left=insertNode(root->left, data);
	else
		root->right=insertNode(root->right, data);

	return root;
}

/* simple inorder traversal and printing */
void inorderTraversal(node* root){
	if(root==NULL)
		return;

	inorderTraversal(root->left);
	cout<<root->data<<" -> ";
	inorderTraversal(root->right);
}

/* next min value, inorder successor */
node* minValueNode(node* nd) {
	node* current = nd;

	while(current && nd->left!=nullptr)
		current=current->left;

	return current;
}

/* delete node from tree
 * traverse inorder, chose successor
 * for replacing node value to maintain order
 */
node* deleteNode(node* root, int data){
	if(root==NULL) return NULL;

	/* find node to be deleted */
	if(data < root->data)
		root->left=deleteNode(root->left, data);
	else if(data > root->data)
		root->right=deleteNode(root->right, data);
	else{
		/* 1st part, if node is with only one child/no child */
		if(root->left == NULL){
		    node* temp=root->right;
		    delete root;
		    return temp;
		}else if(root->right == NULL){
			node* temp=root->left;
			delete root;
			return temp;
		}

		/* if node has two children, find next min value */
		node* temp=minValueNode(root->right);
		/*replace value */
		cout<<"\nsuccessor found: "<<temp->data<<endl;
		root->data=temp->data;
		/* delete min value - inorder successor */
		root->right=deleteNode(root->right, temp->data);

	}

	return root;
}

/* search in BST */
bool searchTree(node* root, int num){
	if(root==NULL) return false;

	if(num < root->data)
		searchTree(root->left, num);
	else if(num > root->data)
		searchTree(root->right, num);
	else
		return true;
}

int calcHeight(node* root){
	if(!root)
	   return 0;

	int left=calcHeight(root->left);
	int right=calcHeight(root->right);

	return max(left, right)+1;
}

/* Driver program */
int main() {
	cout << "---Binary Search TREE---" << endl;

	node* root=nullptr; //root node
	root=insertNode(root, 8);
	root=insertNode(root, 10);
	root=insertNode(root, 6);
	root=insertNode(root, 14);
	root=insertNode(root, 3);
	root=insertNode(root, 1);
	root=insertNode(root, 4);
	root=insertNode(root, 7);

	cout<<"Height of tree = "<<calcHeight(root)<<endl;

	inorderTraversal(root);
	searchTree(root, 6)?cout<<"\n---Found 6---\n":cout<<"\n---Not found 6 in tree---\n";
	cout<<"Deleting 6 from tree"<<endl;
	deleteNode(root, 6);
	searchTree(root, 6)?cout<<"\n---Found 6---\n":cout<<"\n---Not found 6 in tree---\n";
	inorderTraversal(root);
	cout<<endl;

    cout<<"Inserting new elements"<<endl;
	root=insertNode(root, 27);
	root=insertNode(root, 33);
	root=insertNode(root, 150);
	root=insertNode(root, -10);

	inorderTraversal(root);
	cout<<"\nHeight of tree = "<<calcHeight(root)<<endl;
	searchTree(root, 14)?cout<<"\n---Found 14---\n":cout<<"\n---Not found 14 in tree---\n";
	deleteNode(root, 33);
	cout<<"root: "<<root->data<<endl;
	inorderTraversal(root);

	deleteNode(root, 8);
	cout<<"\nroot: "<<root->data<<endl;
	inorderTraversal(root);
	
	return 0;
}
