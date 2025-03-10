#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int TABLE_SIZE = 10;

struct hashnode{
	char data[100];
	int key;
	int hash;
	struct hashnode *next;
}*hashtable[100], *curr, *temptable[100];

void initialize_temp(){
	for(int i = 0; i < TABLE_SIZE; i++){
		temptable[i] = NULL;
	}
}

void initialize_table(){
	for(int i = 0; i < TABLE_SIZE; i++){
		hashtable[i] = NULL;
	}
}

int key_val(char data[]){
	int total = 0;
	for(int i =0; i < strlen(data); i++){
		total += (int) data[i];
	}
	return total;
}

//Hash Functions
int mid_square(int key){
	int hash, squared;
	char storesquare[100];
	// square the key
	squared = key * key;
	sprintf(storesquare, "%d", squared);
	
	//find the middle value;
	hash =(int) storesquare[strlen(storesquare) / 2];
	return (hash - 48);
}

int division(int key){
	int hash;
	hash = key;
	return hash;
}

int folding( int key){
	int hash, split;
	int sum = 0;
	split = key;
	//split key into several parts
	while(split != 0){
		sum += split % 100;
		split /= 100;
	}
	//sum them all up and remove excess
	hash = sum;
	return hash;
}

int digit_extraction(int key){
	int hash, extract, tenamount = 0;
	extract = key;
	int sum = 0;
	while(extract != 0){
		sum += (extract % 10) * (10 * tenamount);
		tenamount++;
		extract /= 100;
	}
	hash = sum;
	return hash;
}

int rotate(int key){
	int hash, newvalue, reverse = 0;
	newvalue = division(key);
	while(newvalue != 0){
		reverse = reverse*10 + newvalue % 10;
		newvalue /= 10;
	}
	hash = reverse;
	return hash;
}


// insert
void insert_chaining(char data[], int hashkey, int key){
	struct hashnode *tnode = (struct hashnode*) malloc (sizeof(struct hashnode));
	tnode->hash = hashkey;
	tnode->key = key;
	strcpy(tnode->data, data);
	tnode->next = NULL;
	
	if(hashtable[hashkey]==NULL){
		hashtable[hashkey] = tnode;
	}
	else{
		curr = hashtable[hashkey];
		while(curr->next != NULL ) curr = curr->next;
		curr->next = tnode;
	}
}

void insert_linear(char data[], int hashkey, int key){
	int cur_index;
	struct hashnode *tnode = (struct hashnode*) malloc (sizeof(struct hashnode));
	tnode->hash = hashkey;
	tnode->key = key;
	strcpy(tnode->data, data);
	tnode->next = NULL;
	
	if(hashtable[hashkey] != NULL){
		cur_index = hashkey;
		while(hashtable[cur_index] != NULL){
			cur_index++;
		}
		hashtable[cur_index] = tnode;
	}else{
		hashtable[hashkey] = tnode;
	}
	
}

void rehashing(){
	int original_size = TABLE_SIZE;
	int j = 0;
	TABLE_SIZE = TABLE_SIZE * 2;
	for(int i = 0; i < original_size; i++){
		if(hashtable[i] == NULL){
			continue;
		}
		else
		{
			temptable[j] = hashtable[i];
			j++;
			
		}
	}
	
	initialize_table();
	
	for(int i = 0; i < j; i++){
		insert_linear(temptable[i]->data, temptable[i]->key % TABLE_SIZE, temptable[i]->key);
	}
	
}

void display_table(){
	for(int i = 0 ; i < TABLE_SIZE; i++){
		if(hashtable[i]== NULL){
			printf("%d : EMPTY\n", i);
		}
		else{
			printf("%d : %d %s", i, hashtable[i]->key, hashtable[i]->data);
			curr = hashtable[i]->next;
			while(curr != NULL){
				printf("-> %d %s ", curr->key, curr->data);
				curr = curr->next;
			}
			printf("\n");
		}
	}
	return;
}

int main(){
	int hashf_choice, key, i, collision_choice;
	int hash_val;
	char string[100];
	printf("Pick a hash function : \n");
	printf("1. Mid-square\n");
	printf("2. division\n");
	printf("3. Folding\n");
	printf("4. Digit extraction\n");
	printf("5. Rotate\n");
	
	do{
		printf("Choice : ");
		scanf("%d", &hashf_choice);getchar();
	}while(hashf_choice > 5 || hashf_choice < 1);
	
	printf("\nPick Collision Handling\n");
	printf("1. Linear Probing\n");
	printf("2. Chaining\n");
	printf("3. Rehashing\n");
	
	do{
	printf("Choice : ");
	scanf("%d", &collision_choice);getchar();
	}while(collision_choice < 1 || collision_choice > 3);

	while(1){
		printf("Input Data (type -1 to exit): ");
		scanf("%s", string);getchar();
		if(strcmp("-1", string) == 0){
			break;
		}
		key = key_val(string);
		
		switch(hashf_choice){
			case 1:
				hash_val = mid_square(key);
				break;
			case 2:
				hash_val = division(key);
				break;
			case 3:
				hash_val = folding(key);
				break;
			case 4:
				hash_val = digit_extraction(key);
				break;
			case 5:
				hash_val = rotate(key);
				break;
		}
		
		switch(collision_choice){
			case 1:
				insert_linear(string, hash_val % TABLE_SIZE, hash_val);
				break;
			case 2:
				insert_chaining(string, hash_val % TABLE_SIZE, hash_val);
				break;
			case 3:
				insert_linear(string, hash_val % TABLE_SIZE, hash_val);
				break;
		}
	}
	
	if(collision_choice == 3){
		rehashing();
	}
	display_table();

	return 0;
}
