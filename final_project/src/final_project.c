/*
 ============================================================================
 Name        : final_project.c
 Author      : Kevin Sapp

Major changes:
bin file reading and writing to save data
deleting an account

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bank_t{
	int account;
	float balance;
	char name[15];
	struct bank_t *next;
} bank_t;

// Converts binary string back to text and returns the allocated string pointer
char* binary_to_text(const char *binary) {
    int bin_len = strlen(binary);
    int text_len = bin_len / 8;

    // Allocate memory for the text plus the null terminator
    char *text = malloc((text_len + 1) * sizeof(char));
    if (text == NULL) {
        return NULL;
    }

    int text_idx = 0;
    for (int i = 0; i < bin_len; i += 8) {
        char ch = 0;
        for (int bit = 0; bit < 8; bit++) {
            ch <<= 1;
            if (binary[i + bit] == '1') {
                ch |= 1;
            }
        }
        text[text_idx++] = ch;
    }
    text[text_idx] = '\0';

    return text;
}

struct bank_t* find_tail(bank_t *head){
	struct bank_t* current=head;
	while (current->next!=NULL){
		current=current->next;
	}
//	printf("Tail is %d",current->account);
	return current;
}

struct bank_t* add_account(bank_t *head){
//	struct bank_t *tail=find_tail(head);
	struct bank_t *value=malloc(sizeof(struct bank_t));
	printf("Enter account number: "); scanf("%d",&value->account);
	printf("Enter account balance: "); scanf("%f",&value->balance);
	printf("Enter account name: "); scanf("%s",value->name);
	value->next=NULL;

	if (head==NULL){head=value;}
	struct bank_t* tail = head;
	while (tail->next!=NULL){
		tail=tail->next;
	}
	tail->next=value;
	value->next=NULL;
	return head;
//	printf("%d",value->account); printf("%p",&value->account);
}

struct bank_t* find_account(bank_t *head, int account){
	struct bank_t* current=head;
	struct bank_t* found=NULL;
	while(current!=NULL){
		if (current->account==account){found=current;}
		current=current->next;
	}
	if (found==NULL){printf("Error: No account found\n");return NULL;}
	else{return found;}
}

struct bank_t* delete_account(bank_t *head, int account){
	struct bank_t* current=head;
	struct bank_t* prev=NULL;
	struct bank_t* found=NULL;
	while(current!=NULL){
		if (current->account==account){
			found=current;
			if (prev!=NULL){prev->next=current->next;}
			else{
				head=current->next;
			}
			printf("Deleted account %d.",account);
			return head;
		}
		prev=current;
		current=current->next;
	}
	if (found==NULL){printf("Error: No account found\n");return NULL;}
//	else{return found;}
	return head;
}

void print_account(bank_t *value){
	printf("Account information:\n  Account number: %d\n  Balance: %.2f\n  Name: %s\n",value->account, value->balance, value->name);
}

void display_all_records(bank_t *value){
	if (value==NULL){printf("Cannot display anything. There are no records.\n");return;}
	printf("\nDisplaying all records:\n");
	while (value != NULL){
		print_account(value); printf("\n");
		if (value->next==NULL){break;}
		value = value->next;
	}
}



int save_struct_arr(bank_t *head){
	FILE *file = fopen("accounts.bin", "wb");
	if (file == NULL) {
		perror("Error opening file");
		return 1;
	}
	int amt_of_structs=0;
	bank_t *value = head;
	while (value != NULL){
		amt_of_structs++;
		size_t dataSize=sizeof(value->account)+sizeof(value->balance)+15;
		fwrite(value, dataSize, 1, file);
		if (value->next==NULL){break;}
		value = value->next;
	}
	fclose(file);
	printf("Amt of structs saved to 'accounts.bin': %d\n",amt_of_structs);
//	bank_t *current = head;
//	while (current != NULL) {
//		struct bank_t *temp = current;
//		current = current->next;
//		free(temp);
//	}
	return 0;
}

struct bank_t* read_struct_arr(){
	FILE *file = fopen("accounts.bin", "rb");
	if (file == NULL) {
		perror("Error opening file");
		return NULL;
	}
	struct bank_t *head = NULL;
	struct bank_t *tail = NULL;
	while (1) {
	        // Allocate temporary memory for the incoming data block
	        struct bank_t *newNode = (struct bank_t *)malloc(sizeof(struct bank_t));
	        if (newNode == NULL) {
	            perror("Memory allocation failed");
	            fclose(file);
	            return NULL;
	        }

	        // 2. Read exactly the data size into our new node's memory space
	        // fread returns the number of items successfully read (should be 1)
	        size_t bytesToRead=sizeof(newNode->account)+sizeof(newNode->balance)+15;
	        size_t itemsRead = fread(newNode, bytesToRead, 1, file);

	        // If fread returns 0, we reached the End-Of-File (EOF)
	        if (itemsRead < 1) {
	            free(newNode); // Clean up the unused temporary allocation
	            break;
	        }

	        // Initialize the new node's pointer to safe territory
	        newNode->next = NULL;

	        // 3 & 4. Link the node into the list
	        if (head == NULL) {
	            head = newNode; // This is the very first node (the head)
	            tail = newNode; // Tail also points to the start
	        } else {
	            tail->next = newNode; // Link old tail to new node
	            tail = newNode;       // Move tail pointer forward
	        }
	    }

	fclose(file);
//	display_all_records(head);
//	bank_t *current = head;
//	while (current != NULL) {
//		bank_t *temp = current;
//		current = current->next;
//		free(temp);
//	}
	return head;
}

struct bank_t* admin_menu(bank_t *head){
	int go = 1;
	int i;
	while (go==1){
	printf("\nMenu:\n 0. Save and Exit\n  1. New Account\n  2. Display all accounts\n  3. Find account\n  4. Edit account\n  5. Delete account\n  6. Load accounts from file\nEnter your choice: ");
			scanf("%d", &i);
			switch (i){
				case 1:
					head=add_account(head);
					break;
				case 2:
					display_all_records(head);
					break;
				case 3:
					int acc;
					printf("\nEnter account number to find: ");
					scanf("%d",&acc);
					struct bank_t* f;
					f=find_account(head,acc);
					if (f!=NULL){print_account(f);}
					break;
				case 4:
					int acc1;
					printf("\nEnter account number to edit: ");
					scanf("%d",&acc1);
					struct bank_t* e;
					e=find_account(head,acc1);
					if (e!=NULL){
						printf("Edit Enter account number (%d): ",e->account); scanf("%d",&e->account);
						printf("Enter account balance (%f): ",e->balance); scanf("%f",&e->balance);
						printf("Enter account name (");printf(e->name); printf("): "); scanf("%s",e->name);
					}
					break;
				case 5:
					int acc2;
					printf("\nEnter account number to delete: ");
					scanf("%d",&acc2);
					struct bank_t* temp;
					temp=delete_account(head,acc2);
					if (temp!=NULL){head=temp;}
					break;
				case 6:
					head=read_struct_arr();
					break;
				case 0:
					save_struct_arr(head);
					go=0;
					break;
				default:
					printf("Sorry, I couldn't make that work.\n");
					go=0;
					break;
		}
	}
	return head;
}

struct bank_t* user_menu(bank_t *head,bank_t *acc){
	int go = 1;
	int i;
	if (acc==NULL){return NULL;}
	while (go==1){
	printf("\nMenu:\n 0. Save and Exit\n  1. Display account\n  2. Edit account\n  3. Delete account\nEnter your choice: ");
			scanf("%d", &i);
			switch (i){
				case 1:
					print_account(acc);
					break;
				case 2:
					printf("Edit Enter account number (%d): ",acc->account); scanf("%d",&acc->account);
					printf("Enter account balance (%f): ",acc->balance); scanf("%f",&acc->balance);
					printf("Enter account name (");printf(acc->name); printf("): "); scanf("%s",acc->name);
					break;
				case 3:
					int del;
					printf("\nAre you sure you want to delete your account? 0 for delete: ");
					scanf("%d",&del);
					if (del!=0){printf("Okay, canceling deletion.\n");break;}
					struct bank_t* temp;
					temp=delete_account(head,acc->account);
					if (temp!=NULL){head=temp;}
					return head;
				case 0:
					save_struct_arr(head);
					go=0;
					break;
				default:
					printf("Sorry, I couldn't make that work.\n");
					go=0;
					break;
		}
	}
	return head;
}

int main(void) {
	struct bank_t* head=NULL;
	head=read_struct_arr();
	int go = 1;
//	int admin = 0;
	int s;
	while (go==1){
		printf("\nMenu:\n 0. Exit\n  1. Admin Login\n  2. User Login\nEnter your choice: ");
		scanf("%d", &s);
		switch (s){
			case 0:
				printf("\nBye!");
				go=0;
				break;
			case 1:
				char password[15];
				printf("\nEnter the password:  ");
				scanf("%s",password);
				password[strcspn(password, "\n")] = '\0';
				if (strcmp(binary_to_text("0110101001100101011100110111010101110011011010010111001101101011011010010110111001100111"),password)==0){
					printf("Correct!\n\n");
//					admin=1;
					struct bank_t* temp;
					temp=admin_menu(head);
					if (temp!=NULL){head=temp;}
				}
				else {
					printf("That's incorrect.\n");
				}
				break;
			case 2:
				int acc1;
				char accname[15];
				printf("Enter account number to edit: ");
				scanf("%d",&acc1);
				struct bank_t* e;
				e=find_account(head,acc1);
				if (e!=NULL){
					printf("Prove your identity by entering your name: ");
					scanf("%s",accname);
					if (strcmp(accname,e->name)==0){
						printf("That's right!\n");
						struct bank_t* temp;
						temp=user_menu(head,e);
						if (temp!=NULL){head=temp;}
					} else {
						printf("That's not right.");
					}
				} else{
					printf("Sorry. Couldn't find that account.\n");
				}
				break;
			default:
				printf("Sorry, I couldn't make that work.\n");
				go=0;
				break;
			}
		}


	return EXIT_SUCCESS;
}
