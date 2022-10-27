#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define MAX_LENGTH 70
#define MAX_LINES 100
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 20

void tokenize_line (char *input_line);  //function declaration for tokenizing cur_line line by line
int double_word_check(char *tokenized_word);
int remove_double_words(char concord_lines_tokenized);
int exclusion_check(char *tokenized_word); //function declaration for checking if each word being tokenized is an exclusion word.
static int str_cmp(const void *a, const void *b); //function declaration for comparing two char's, used in qsort() method.

int num_words = 0; //number of tokenized words added to concord_lines_tokenized
int exclusion_line_count = 0; //number of exclusion words added to exclusion_lines
int concord_line_count = 0; //number of concord lines added to concord_lines

char exclusion_lines[MAX_LINES][MAX_LENGTH]; //array to store exclusion words line by line
char concord_lines[MAX_LINES][MAX_LENGTH]; //array to store concord lines line by line
char concord_lines_tokenized[MAX_LINES * MAX_WORDS][MAX_WORD_LENGTH]; //all to-be-indexed lines are tokenized and stored here

int main() {


        char cur_line[MAX_LENGTH]; //used to store each line of text from stdin

        char exclusion_check[] = "''''";    //used with strcmp to compare to cur_line to see if its the start of exclusion
                                            //lines or start of concordance lines
        int value_check = 0; //value check is updated after to allow all the next lines to be put in the right category
                             //of either exclusion or concordance


        while (fgets(cur_line, sizeof(char) * 100, stdin)) {

                if(cur_line[strlen(cur_line) -1] == '\n') { //gets rid if trailing /n from each line
                        cur_line[strlen(cur_line) -1] = 0;
                }
                if(value_check == 1) { //copies exclusion lines into exclusion_lines
                        strcpy(exclusion_lines[exclusion_line_count], cur_line);
                        exclusion_line_count++;
                } else if(value_check == 2) {   //calls tokenize function on each line of curline
                        strcpy(concord_lines[concord_line_count], cur_line);
                        concord_line_count++;
                        tokenize_line(cur_line);
                }
                if(cur_line[0] == exclusion_check[0]) { //updatres value check for start of exclusion lines
                        value_check = 1;
                } else if(cur_line[0] == '"') { //updates value check for start of lines to-be-indexed
                        value_check = 2;
                }
        }

        qsort(concord_lines_tokenized, num_words, sizeof(concord_lines_tokenized[0]), str_cmp);  //sorts tokenized lines alphabetically

        for(int i = 0; i < num_words; i++) {                                                                 //nested for loops check 1 word at a time in concord_lines_tokenized with every
                for(int j = 0; j < concord_line_count; j++) {                                                //line of concord_lines
                        char *word = strstr(concord_lines[j], concord_lines_tokenized[i]); //pointer to word in each line, null if not found
                        char temp[MAX_LENGTH]; //temp array initialized to hold line that the word is in
                        if(word) { //checks if word is found

                                strcpy(temp, concord_lines[j]);

                                char *word_pointer = strstr(temp, concord_lines_tokenized[i]); //new pointer is needed to ensure each line of concord_lines is not overwritten
                                int k = 0;

                                if(strlen(temp) == strlen(word_pointer) || isspace(word_pointer[k - 1])) { //checks to see if word being pointed at is not a part of another word
                                        while(!isspace(word_pointer[k]) && k != strlen(temp)) {            //EX: a pointer to "cat" in "this is a catastrophe, said the cat" would point to "catastrophe"
                                                word_pointer[k] = toupper(word_pointer[k]);
                                                k++;
                                        }
                                        printf("%s\n",temp); //final stdout lines
                                }
                        }
                }
        }
}
static int str_cmp(const void *a, const void *b) {  //function to compare two chars for qsort()
        return strcmp(a, b);
}
void tokenize_line (char *input_line) {  //function that tokenizes words and copies them line by line into concord_lines_tokenized
        char *tokenized_word;

        tokenized_word = strtok (input_line, " ");

        while (tokenized_word && num_words < MAX_WORDS) {
                if(!exclusion_check(tokenized_word) && !double_word_check(tokenized_word)) {  //checks if each word is an exclusion and if its already a tokenized word,
                        strcpy(concord_lines_tokenized[num_words], tokenized_word);           //if both are false, tokenized word is copied
                        num_words++;
                }
                tokenized_word = strtok(NULL, " ");
        }
                return;
}
int exclusion_check(char *tokenized_word) {  //function to check if each word being tokenized is an exclusion word
        for(int i = 0; i < exclusion_line_count; i++) {
                if(!strcmp(tokenized_word, exclusion_lines[i])) {
                        return 1;
                }

        }
}
