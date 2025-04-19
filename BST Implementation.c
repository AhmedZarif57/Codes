#include<stdio.h>
#include<stdlib.h>
typedef struct node{
    struct node* left;
    int data;
    struct node* right;
}node;

node* create(int key){
    node* temp=malloc(sizeof(node));
    temp->data=key;
    temp->left=temp->right=NULL;
    return temp;
}

node* insert(node* root, int key){
    if(root==NULL)return create(key);
    if(root->data==key) return root;
    if(root->data>key) root->left=insert(root->left,key);
    if(root->data<key) root->right=insert(root->right,key);
    return root;
}

node* search(node* root, int key){
    if(root==NULL || root->data == key) return root;
    if(root->data>key) return search(root->left, key);
    if(root->data<key) return search(root->right, key);
}

int getmin(node* root){ //Deletion Code Part 1
    while(root!=NULL && root->left!=NULL)
        root=root->left;
    return root->data;
}

node* delNode(node* root,int key){ //Deletion Code Part 2
    if(root==NULL) return root; //No Elements in Tree
    if(root->data>key) root->left=delNode(root->left,key);
    if(root->data<key) root->right=delNode(root->right,key);
    else{
        if(root->left==NULL){
            node* temp=root->right;
            free(root);
            return temp;
        }
        if(root->right==NULL){
            node* temp=root->left;
            free(root);
            return temp;
        }
        else{
            root->data=getmin(root->right);
            root->right=delNode(root->right,getmin(root->right));
        }
    }
    return root;
}

void preorder(node* root){
    if(root == NULL)return;
    printf("%d  ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(node* root){
    if(root == NULL)return;
    postorder(root->left);
    postorder(root->right);
    printf("%d  ", root->data);
}

void inorder(node* root){
    if(root == NULL)return;
    inorder(root->left);
    printf("%d  ", root->data);
    inorder(root->right);
}

void run(node* root){
    printf("\n PreOrder \n");
    preorder(root);
    printf("\n PostOrder \n");
    postorder(root);
    printf("\n InOrder \n");
    inorder(root);
}

int main(){
    node* root=NULL;
    root=insert(root,20);
    root=insert(root,10);
    root=insert(root,30);
    root=insert(root,40);
    root=insert(root,70);
    root=insert(root,90);
    run(root);
    root=delNode(root,70);
    printf("\n\nAfter Deletion\n");
    run(root);
    printf((search(root,100)!=NULL)?"\n\nFound\n":"\n\nNot Found\n");
    return 0;
}
