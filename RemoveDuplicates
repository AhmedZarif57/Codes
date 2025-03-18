void removeDuplicates(struct Node* head) {
    if (head == NULL || head->next == NULL)
        return;

    struct Node* current = head;
    
    while (current != NULL) {
        struct Node* runner = current;
        
        while (runner->next != NULL) {
            if (current->data == runner->next->data) {
                struct Node* temp = runner->next;
                runner->next = runner->next->next;
                free(temp);
            } else {
                runner = runner->next;
            }
        }
        current = current->next;
    }
}
