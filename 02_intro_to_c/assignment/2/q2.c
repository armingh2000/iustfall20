#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    int key;
    struct node * left, * right;
}node_t;

typedef struct btree {
    node_t * root;
}btree_t;

node_t * new_node(int key)
{
    node_t * temp = malloc(sizeof(temp));
    temp -> key = key;
    temp -> left = temp -> right = NULL;
    return temp;
}

node_t * insert_node(node_t * node, int key)
{
    if(node == NULL) return new_node(key);

    if(key < node -> key)
        node -> left = insert_node(node -> left, key);
    else
        node -> right = insert_node(node -> right, key);

    return node;
}

node_t * min_key_node(node_t * node)
{
    node_t * current_node = node;
    while(current_node && current_node->left != NULL)
        current_node = current_node -> left;

    return current_node;
}



node_t * remove_node(node_t * root, int key)
{
    if(root == NULL) return root;

    if(key < root -> key)
        root -> left = remove_node(root->left, key);
    else if(key > root -> key)
        root -> right = remove_node(root -> right, key);
    else
    {
        if(root -> left == NULL)
        {
            node_t * temp = root -> right;
            free(root);
            return temp;
        }
        else if(root -> right == NULL)
        {
            node_t * temp = root -> left;
            free(root);
            return temp;
        }

        node_t * temp = min_key_node(root -> right);

        root -> key = temp -> key;
        root -> right = remove_node(root -> right, temp -> key);
    }

    return root;
}

void traverse(node_t * root)
{
    if(root != NULL)
    {
        traverse(root -> left);
        printf("%d -> ", root -> key);
        traverse(root -> right);
    }
}

node_t * find_node(node_t * root, int key)
{
    if(root == NULL) return NULL;

    if(key < root -> key)
        return find_node(root -> left, key);
    else if(key > root -> key)
        return find_node(root -> right, key);
    else
        return root;

    return NULL;
}

/* Add key to the tree
 * return 1 if successful otherwise 0.
 * */
int btree_add_node (btree_t *t, int key)
{
    t -> root = insert_node(t -> root, key);

    if(t -> root == NULL)
        return 0;
    return 1;
}

/* Remove a key from the tree
 * return 1 if successful otherwise 0.
 * */
int btree_remove_node (btree_t *t, int key)
{
    t -> root = remove_node(t -> root, key);

    if(t -> root == NULL)
        return 0;
    return 1;
}

/* Look up a key in the tree
 * return 1 if found otherwise 0.
 * */
int btree_search (btree_t *t, int key)
{
    node_t * temp = find_node(t -> root, key);

    if(temp == NULL)
        return 0;
    return 1;
}

void free_traverse(node_t * root)
{
    if(root != NULL)
    {
        free_traverse(root -> left);
        free_traverse(root -> right);
        free(root);
    }
}

void btree_free_nodes(btree_t * t)
{
    free_traverse(t -> root);
}


int main(int args, char * argv[])
{
    btree_t * t = malloc(sizeof(*t)); 
    t -> root = NULL;
    btree_add_node(t, 8);
    btree_add_node(t, 5);
    btree_add_node(t, 3);
    btree_add_node(t, 2);
    btree_add_node(t, 7);
    btree_add_node(t, 100);
    btree_add_node(t, 1);
    btree_add_node(t, 20);

    printf("%s", "initial tree:\n");
    traverse(t -> root);

    btree_remove_node(t, 100);
    printf("%s", "\nremoved 100 \n");
    traverse(t -> root);

    
    int res;

    res = btree_search(t, 20);
    printf("%s %d", "\nsearch for 20: ", res);


    res = btree_search(t, 2000);
    printf("%s %d\n", "\nsearch for 2000: ", res);
    
    btree_free_nodes(t);
    free(t);

    return EXIT_SUCCESS;
}
