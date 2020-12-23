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

typedef struct node{
	int data;
	node* left;
	node* right;

	node(int i){
		this->data=i;
	}
};

/* create new node */
node* newNode(int i){
	node* newN=new node(i);
	newN->left=NULL; newN->right=NULL;

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
node* minValueNode(node* node) {
	struct node* current = node;

	while(current && node->left!=NULL){
		current=current->left;
	}

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

/* Driver program */
int main() {
	cout << "---Binary Search TREE---" << endl;

	node* root=NULL; //root node
	root=insertNode(root, 8);
	root=insertNode(root, 10);
	root=insertNode(root, 6);
	root=insertNode(root, 14);
	root=insertNode(root, 3);
	root=insertNode(root, 1);
	root=insertNode(root, 4);
	root=insertNode(root, 7);

	inorderTraversal(root);
	searchTree(root, 6)?cout<<"\n---Found 6---\n":cout<<"\n---Not found 6 in tree---\n";
	cout<<"Deleting 6 from tree"<<endl;
	deleteNode(root, 6);
	searchTree(root, 6)?cout<<"\n---Found 6---\n":cout<<"\n---Not found 6 in tree---\n";
	inorderTraversal(root);

	return 0;
}
