/*#include "./intel_input.h"

void getInput(char* output){
        string input;
        input.size = 16;

        input.text = malloc(input.size * sizeof(char));

        strcpy(input.text, "");

        int keyCode;
        while(keyCode != 10){
                keyCode = getchar();
                if(keyCode != 10){
                        if(strlen(input.text) >= input.size){
                                input.size=input.size*2;
                                char temp[input.size];
                                strcpy(temp, input.text);

                                input.text =
                                realloc(input.text,
                                        input.size*sizeof(char));
                                strcpy(input.text, temp);

                        }
                        addChar(input.text, keyCode);

                }
        }
	strcpy(output, input.text);
}*/
