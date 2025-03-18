#include <stdio.h>
#include <stdlib.h>
typedef struct node{
    int data;
    struct node* next;
}node;

node* head=NULL;

void create(int data){
    node* neww=malloc(sizeof(node));
    neww->data=data;
    neww->next=NULL;
    if(head==NULL){
        head=neww;
        return;
    }
    node* temp=head;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next=neww;
}
void insert(int value, int pos){
    node*temp=head;
    node* neww=malloc(sizeof(node));
    neww->data=value;
    neww->next=NULL;
    int i=1;
    while(i<pos-1){
        temp=temp->next;
        i++;
    }
    neww->next=temp->next;
    temp->next=neww;
}
void print(){
    node* temp=head;
    if(temp==NULL)printf("No elements\n");
    while(temp!=NULL){
        printf("%d -> ",temp->data);
        temp=temp->next;
    }
    printf("NULL\n");
}
int main(){
    int n,x;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&x);
        create(x);
    }
    print();
    int v,p;
    scanf("%d%d",&v,&p);
    insert(v,p);
    print();
    return 0;
}
