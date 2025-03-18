#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node* next;
} node;

node* addEnd(node* last, int data)
{
    if (last == NULL)
    {
        node* newww = malloc(sizeof(node));
        newww->data = data;
        newww->next = newww;
        return newww;
    }
    node* neww = malloc(sizeof(node));
    neww->data = data;
    neww->next = last->next;
    last->next = neww;
    last = neww;
    return last;
}

void print(node* last)
{
    if (last == NULL)
    {
        printf("No elements in the list\n");
        return;
    }
    node* first = last->next;
    do
    {
        printf("%d ", first->data);
        first = first->next;
    }
    while (first != last->next);
    printf("\n");
}

int main()
{
    node* last = NULL;
    int n, data;

    printf("Enter the number of nodes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        printf("Enter the data for node %d: ", i+1);
        scanf("%d", &data);
        last = addEnd(last, data);
    }

    printf("Circular Linked List: ");
    print(last);

    printf("Enter the node to append: ");
    scanf("%d", &data);
    last = addEnd(last, data);
    print(last);

    return 0;
}
