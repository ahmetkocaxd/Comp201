#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// TASK 1 FUNCTIONS


//helper function for encryptMessage
//if the char is a letter, shifts it. if not, return without any changes (for punctuation characters)
//the expression in the first return type is for shifting the letters inside the alphabet. If I used
//ch + shift, the last letters of alphabet becomes different characters. (shifting z is by a number is not 
//going to be a letter)
char encryptChar(char ch, int shift){

	if(isalpha(ch)){
		char base = isupper(ch) ? 'A' : 'a';
		return (ch + shift - base) % 26 + base;
	}

	return ch;
}

//encrypting the message by reading it from an input file, using encryptChar function and
//writing it into an outputFile.
void encryptMessage(FILE *inputFile, FILE *outputFile, int shift){
	
	char temp[512];

	while(fgets(temp, sizeof(temp), inputFile) != NULL){

		int length = strlen(temp);
		
		for(int i = 0; i < length; i++){

			temp[i] = encryptChar(temp[i], shift);
		}

		fputs(temp, outputFile);
	}

}


// TASK 2 FUNCTIONS

//helper function for decryptMessage. Same with encryptChar, the difference is substracting the shift index 
//and add 26 to the sum since the first letters of alphabet may become some other non-letter character
//( for example A - 2 is not a letter).
//This function also is used in upcoming tasks.
char decryptChar(char ch, int shift){

	if(isalpha(ch)){
		char base = isupper(ch) ? 'A' : 'a';
		return (ch - shift - base + 26) % 26 + base;
	}

	return ch;
}

//decrypting Message, similar to encrypting. reading input file, shifting indeces to the original values, writing it into output file.
//this method is also used for upcoming tasks.
void decryptMessage(FILE *inputFile, FILE *outputFile, int shift){
	
	char temp[512];

	while(fgets(temp, sizeof(temp), inputFile) != NULL){

		int length = strlen(temp);
		
		for(int i = 0; i < length; i++){

			temp[i] = decryptChar(temp[i], shift);
		}

		fputs(temp, outputFile);
	}

}

// TASK 3 FUNCTIONS

//finding most frequent letter by creating a int array with 26 indeces (each index represents a letter)
//reading the inputFile and storing the number of each letter in the in array. Finding the index with max int number,
//which represents the repetition number of that index's letter. Convert it to a letter by adding it with 'a' and return it.
char findMostFrequentLetter(FILE *inputFile){
	
	int freq[26] = {0};
	char ch;
	int indexAtAlphabet;
	char temp[512];

	while(fgets(temp, sizeof(temp), inputFile) != NULL){

		int length = strlen(temp);
		
		for(int i = 0; i < length; i++){

			ch = temp[i];
			if(isalpha(ch)){
				ch = tolower(ch);
				indexAtAlphabet = ch -'a';
				freq[indexAtAlphabet]++;
			}
		}	
	}

	int max = 0;

	for(int i = 1; i < 26; i++){
		
		if(freq[i] > freq[max]){
			max = i;
		}
	}

	char mostFrequentLetter = 'a' + max;

	return max + 'a';


}

// TASK 4 FUNCTIONS

//finding the most frequent word by reading the input file, saving it into char arrays (message and messagecpy), then apply strtok to split the text with the known
//delimiters and find the 3 letter length words. Counting each one of them with the help of strstr (applying it untill there is no occurence of that world and
//increment count each time). If the count become larger than maxCount, then set that token as maxWord and its count as maxCount. Finally, return the pointer
//to the beginning of the found 3 letter length word. 
char* findMostFrequentWord(FILE *inputFile){

	
	
	char line[400];
	char *message = malloc(1);
	message[0] = '\0';
	char *token;
	char *maxWord;
	int maxCount = 0;
	

	while(fgets(line, sizeof(line), inputFile) != NULL){
		
		message = realloc(message, strlen(message) + strlen(line) + 1);
		strcat(message, line);
	}
	

	char messagecpy[2000];
	strcpy(messagecpy, message);

	token = strtok(message, " ,.':\"\n");

	
	while(token!=NULL){
		

		if(strlen(token) == 3){
			
			int count = 0;
			char *p = messagecpy;

			p = strstr(p, token);

			while(p){
				
				count++;
				p += 3;
				p = strstr(p, token);
				
			}

			if(count > maxCount){
				
				maxCount = count;
				maxWord = token;
				
			}
		}
		token = strtok(NULL, " ,.':\"\n");
	}
	
	
	free(message);	
	return maxWord;

}




int main() {
	
	// TASK 1

	FILE *inputFile, *outputFile;
	int shift = 1;

	inputFile = fopen("message1.txt", "r");
	if(inputFile == NULL){
		perror("Error opening input file.");
		exit(1);
	}

	outputFile = fopen("message1_encrypted.txt", "w");
	if(outputFile == NULL){
		perror("Error opening output file.");
		fclose(inputFile);
		exit(1);
	}

	encryptMessage(inputFile, outputFile, shift);

	fclose(inputFile);
	fclose(outputFile);

	// TASK 2
	
	shift = 5;

	inputFile = fopen("message2_encrypted.txt", "r");
	if(inputFile == NULL){
		perror("Error opening input file.");
		exit(1);
	}

	outputFile = fopen("message2_decrypted.txt", "w");
	if(outputFile == NULL){
		perror("Error opening output file.");
		fclose(inputFile);
		exit(1);
	}

	decryptMessage(inputFile, outputFile, shift);

	fclose(inputFile);
	fclose(outputFile);


	// TASK 3
	
	

	inputFile = fopen("message3_encrypted.txt", "r");
	if(inputFile == NULL){
		perror("Error opening input file.");
		exit(1);
	}

	outputFile = fopen("message3_decrypted.txt", "w");
	if(outputFile == NULL){
		perror("Error opening output file.");
		fclose(inputFile);
		exit(1);
	}

	char mostFrequentLetter = findMostFrequentLetter(inputFile);
	shift = (mostFrequentLetter - 'e' + 26) % 26;
	
	//I had to close files and reopen them, because it always gave an empty txt file, I suppose the operations in
	//findMostFrequentLetter made the pointers point end of the files.
	fclose(inputFile);
	fclose(outputFile);

	inputFile = fopen("message3_encrypted.txt", "r");
	if(inputFile == NULL){
		perror("Error opening input file.");
		exit(1);
	}

	outputFile = fopen("message3_decrypted.txt", "w");
	if(outputFile == NULL){
		perror("Error opening output file.");
		fclose(inputFile);
		exit(1);
	}


	decryptMessage(inputFile, outputFile, shift);

	fclose(inputFile);
	fclose(outputFile);


	// TASK 4
	

	inputFile = fopen("message4_encrypted.txt", "r");
	if(inputFile == NULL){
		perror("Error opening input file.");
		exit(1);
	}

	outputFile = fopen("message4_decrypted.txt", "w");
	if(outputFile == NULL){
		perror("Error opening output file.");
		fclose(inputFile);
		exit(1);
	}
	
	//since it is known that most frequent word is "the", dereferencing the the pointer's value (the letter)
	//and finding its shifting number.
	char *mostFrequentWordPtr = findMostFrequentWord(inputFile);
	shift = (*mostFrequentWordPtr - 't' + 26) % 26;


	//I had to close files and reopen them, because it always gave an empty txt file, I suppose the operations in
	//findMostFrequentLetter made the pointers point end of the files.
	fclose(inputFile);
	fclose(outputFile);


	inputFile = fopen("message4_encrypted.txt", "r");
	if(inputFile == NULL){
		perror("Error opening input file.");
		exit(1);
	}

	outputFile = fopen("message4_decrypted.txt", "w");
	if(outputFile == NULL){
		perror("Error opening output file.");
		fclose(inputFile);
		exit(1);
	}


	decryptMessage(inputFile, outputFile, shift);

	fclose(inputFile);
	fclose(outputFile); 

	return 0;
}

// The Caesar cipher method is not a very good way of encryption in my opinion. I believe, its strength is also its weakness.
// It is not very hard to retrieve the enrcrypted data, which is kind of a strength for the reciever of the message. But this
// also applies to the ones who should not see the message. Since there are 26 letters in english alphabet, there are 25 possible
// letter shift options, which can be done in a for loop easily. That is how the message could be retrieved for the ones who do not
// know the shifting index. Maybe, instead of using linear shifting indeces, using more complex way of shifting letters could be a
// better way to secure data. For example, instead of shifting every letter by N, shifting them N^2 times would be much harder to
// solve. Or some other method which does not include mathematical operation, for example shifting every letter by their poisition number
// of the alphabet, shifting a once, shifting b twice, shifting c three times etc. Or determining every shifting indeces of each different
// letter randomly and sharing it only with sender and reciever could be a lot more secure.
