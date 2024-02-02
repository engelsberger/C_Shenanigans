#include<stdio.h>


typedef struct _node {
	int id;
	struct _node* left;
	struct _node* right;
	char* value;
} Node;


Node* create_node(int id, char* value) {

	Node* new_node = (Node*)malloc(sizeof(Node));

	if (new_node == NULL) {
		printf("Error while allocating memory for id %d", id);
		return NULL;
	}

	new_node->left = NULL;
	new_node->right = NULL;
	new_node->id = id;
	new_node->value = value;

	return new_node;
}


/*
* Adds a new node to the tree
* 
* @param root Expects a pointer to a pointer to the root node
* @param id The id for the new node, expects an integer
* @return 0 if successful
* @return 1 on error (or if node already exists)
*/
int add_node(Node* root, int id, char* value) {

	Node* temp = root;
	Node* new_node;


	while (1) {

		if (id < temp->id) {

			if (temp->left != NULL) temp = temp->left;
			else {

				if ((new_node = create_node(id, value)) == NULL) return 1;
				temp->left = new_node;
				break;
			}
		}
		else if (id > temp->id) {

			if (temp->right != NULL) temp = temp->right;
			else {

				if ((new_node = create_node(id, value)) == NULL) return 1;
				temp->right = new_node;
				break;
			}
		}
		else return 1;
	}

	return 0;
}


/*
* Deletes all nodes starting from a given root node
* Usage: root = delete_all(root);
* (It is best practice to set a pointer to freed memory to NULL)
* 
* @param root Expects a pointer to the root node
* @return NULL
*/
Node* delete_all(Node* root) {

	//If current node has a left child, jump to it
	if (root->left != NULL) delete_all(root->left);
	
	//If current node has a right child, jump to it
	if (root->right != NULL) delete_all(root->right);

	//Delete current node, gets here only when all children have been deleted
	free(root);

	return NULL;
}


/*
* Deletes a node from the given tree and sets a successor if the node has children
* If the node has one child, it will become the successor of the deleted node
* If the node has two children, the successor will be the minimum id of the right child
* Usage: root = delete_node_set_successor(root, id);
* (If the node does not exist, nothing happens to the tree)
* 
* @param root Expects a pointer to a pointer to the root node
* @param id The id to be deleted
* @return A pointer to the new root node
*/
Node* delete_node_set_successor(Node* root, int id) {

	Node* successor;
	Node* successor_parent;

	//Return NULL if node does not exist
	if (root == NULL) return NULL;

	/*
	* If the desired node has not been found, move down the tree recursively
	* The left/right child of the current node will stay the same if the child is not the node we are looking for,
	* otherwise it will be set to the successor of the deleted node (or NULL if the deleted node has no successor).
	*/
	if (id < root->id) {
		root->left = delete_node_set_successor(root->left, id);
		return root;
	}
	else if (id > root->id) {
		root->right = delete_node_set_successor(root->right, id);
		return root;
	}

	//This part of the function can only be reached if the current node is the one to be deleted

	//If the node has only one child, set it as successor and delete the current node
	if (root->left == NULL) {

		successor = root->right;
		free(root);
		return successor;
	}
	else if (root->right == NULL) {

		successor = root->left;
		free(root);
		return successor;
	}
	//If the node has two children, determine the successor, swap its id with the current node and delete the successor
	else {

		successor_parent = root;
		successor = root->right;

		//Find the successor, which is the minimal id of the right child of the current root node
		while (successor->left != NULL) {
			successor_parent = successor;
			successor = successor->left;
		}

		//Swap the successor with the current root
		root->id = successor->id;
		root->value = successor->value;

		/*
		* Since we swapped the successors id with the current root id, now the successor is the one we need to delete.
		* For that, we first set the left child of the successors parent (right child if the parent is the current root node)
		* to the successors right child (Successor is the leftmost child, so itself doesn't have a left child).
		* Then, we can simply delete the successor and return the root node, which now holds the successors id.
		*/

		//Replace the successors parents child with the successors child
		if (successor_parent == root) successor_parent->right = successor->right;
		else successor_parent->left = successor->right;

		//Delete successor and return new root
		free(successor);
		return root;
	}
}


/*
* Deletes a node from the given tree, including all its children
* Usage: root = delete_node_ignore_successor(root, id);
* (If the node does not exist, nothing happens to the tree)
*
* @param root Expects a pointer to a pointer to the root node
* @param id The id to be deleted
* @return A pointer to the new root node
*/
Node* delete_node_ignore_successor(Node* root, int id) {

	//Return NULL if node does not exist
	if (root == NULL) return NULL;

	/*
	* If the desired node has not been found, move down the tree recursively
	* The left/right child of the current node will stay the same if the child is not the node we are looking for,
	* otherwise it will be set to NULL
	*/
	if (id < root->id) {
		root->left = delete_node_ignore_successor(root->left, id);
		return root;
	}
	else if (id > root->id) {
		root->right = delete_node_ignore_successor(root->right, id);
		return root;
	}

	//Delete the current node and all its children (returns NULL)
	return delete_all(root);
}


/*
* Prints every element from the given tree in order
* 
* @param root Expects a pointer to the root node
*/
void print_tree(Node* root) {

	//If current node does not exist, skip it
	if (root != NULL) {

		//Jump down all the way to the left, print the smallest ID and step by step go back and print the larger IDs
		print_tree(root->left);
		printf("ID: %d, Value: %s\n", root->id, root->value);
		print_tree(root->right);
	}
}


/*
* Returns the value of the node with the given id
* 
* @param root Expects a pointer to the root node
* @param id The id from the node we are looking for
* @return The value of the node with the given id
* @return NULL if node was not found
*/
char* find_node(Node* root, int id) {

	//Return NULL if node does not exist
	if (root == NULL) return NULL;

	if (id < root->id) return find_node(root->left, id);
	else if (id > root->id) return find_node(root->right, id);

	return root->value;
}


int main(int argc, char* argv[]) {

	Node* root;
	char* temp;


	//First create a tree

	if ((root = create_node(10, "Root")) == NULL) return 1;

	if (add_node(root, 5, "Links")) return 1;
	if (add_node(root, 14, "Rechts")) return 1;
	if (add_node(root, 8, "Links Rechts")) return 1;
	if (add_node(root, 18, "Rechts Rechts")) return 1;
	if (add_node(root, 21, "Rechts Rechts Rechts")) return 1;
	if (add_node(root, 17, "Rechts Rechts Links")) return 1;
	if (add_node(root, 12, "Rechts Links")) return 1;
	if (add_node(root, 4, "Links Links")) return 1;

	printf("The original tree:\n");
	print_tree(root);
	printf("\n");

	//Test the find_node() function

	if ((temp = find_node(root, 18)) != NULL) printf("Node %d found, value: %s\n", 18, temp);
	else printf("Node %d not found\n", 18);
	if ((temp = find_node(root, 10)) != NULL) printf("Node %d found, value: %s\n", 10, temp);
	else printf("Node %d not found\n", 10);
	if ((temp = find_node(root, 9)) != NULL) printf("Node %d found, value: %s\n", 9, temp);
	else printf("Node %d not found\n", 9);
	printf("\n");

	//Test the delete_node_set_successor() function

	root = delete_node_set_successor(root, 10);

	printf("Delete Root and set successor, new Root has ID %d:\n", root->id);
	print_tree(root);
	printf("\n");

	//Test the delete_node_ignore_successor() function

	root = delete_node_ignore_successor(root, 5);

	printf("Delete \"Links\" and its children:\n");
	print_tree(root);
	printf("\n");

	//Test the delete_all() function

	root = delete_all(root);
	printf("Delete all:\n");
	print_tree(root);
}