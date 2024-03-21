#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

int main(){
	char text[MAX_LEN] = "";
	printf("Input a text line: ");
	fgets(text, MAX_LEN, stdin);
	text[strlen(text) - 1] = 0;
	printf("Input text = [%s]\n", text);
	char word[255] = "";
	int prev = 1;
	int count = 0;
	int save_count = 0;
	int no_word = 0;
	int len = strlen(text);
	printf("[");

	for(int i = 0; i < len; i++){
		if(isspace(text[i])){
			if(prev != 1){
				printf("]");
				prev = 1;
			}
		}
		if(!isspace(text[i])){
			printf(" ");
			prev = 0;
		}
		if(prev == 1){
			if(!isspace(text[i+1])){
				printf("[");
			}
		}
	}
	int a = 0;
	printf("]\n");
	for(int i = 0; i < len; i++){
		if(isspace(text[i])){
			if(prev == 1){
				count = i+1;
			}else{
			word[a] = '\0';
			a = 0;
			printf("words[%d] = (%d, %d, %s)\n",no_word,count,i, word);
			count = i+1;
			no_word++;
			prev = 1;
			}
		}else{
			word[a] = text[i];
			a++;
			prev = 0;
		}
		if(i == len-1){
			word[a] = '\0';
			printf("words[%d] = (%d, %d, %s)\n",no_word,count, len, word);
			
		}
	}
	return 0;
}

