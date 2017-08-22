#ifndef _OBSERVATION_H
#define _OBSERVATION_H

#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

class Observation {

public:

	int numWords;
	vector<string> words;
	Observation();
	~Observation();
	int Process(char * num, char* wordLine);
	string ReturnObservation(int index);
	void PrintObservation();

};

class ObservationSet {

public:

	int numObs;
	vector<Observation> observations;

	ObservationSet();
	int Process(char * file);
	Observation ReturnObservationSet(int index);
	void PrintObservationSet();

};

Observation::Observation(){

	numWords = 0;

}

Observation::~Observation(){

	words.clear();

}

int Observation::Process(char * num, char* wordLine){

	// Check for incoming data to process
	if( (!num) || (!wordLine) )
	{
		return 0;
	}

	// Convert num to integer for numWords
	numWords = atoi(num);

	// Convert wordLine to string for delimiting
	string wordstring(wordLine);

	// Find Line Ending
	char end = '0';
	int i = 0;
	while(end != '\n' && end != '\0'){
		end = wordstring[i];
		i++;
		}

	// Replace Line Ending with Null Terminator
	wordstring[i-1] = '\0';

	// Store Observation Words
	string word;
	stringstream stream(wordstring);
	while( getline(stream, word, ' ') )
		words.push_back(word);

	// Return Success
	return 1;

}

string Observation::ReturnObservation(int index){

	string obv = "NULL";
	if(index < 0)
		return obv;
	if(index>words.size())
		return obv;

	obv = words[index];
	
	return obv;
}

void Observation::PrintObservation(){

	cout << "OBSERVATION: " << endl;
	for(int i=0; i<numWords; i++)
		cout << ReturnObservation(i) << endl;
}

ObservationSet::ObservationSet(){

	numObs = 0;

}


int ObservationSet::Process(char * file){

	char text[500];
	char text2[500];
	int val = 0;

	// Open Observation File
	FILE* ifile = fopen(file, "r");

	if(ifile){

		fgets(text, sizeof(text), ifile);
		if(!text){
			fclose(ifile);
			return val;
		}

		if(!(numObs = atoi(text))){
			fclose(ifile);
			return val;
		}

		for(int i = 0; i < numObs; i++){

			fgets(text, sizeof(text), ifile);
			if(!text){
				fclose(ifile);
				return val;
			}

			fgets(text2, sizeof(text2), ifile);
			if(!text2){
				fclose(ifile);
				return val;
			}

			Observation o1;
			o1.Process(text,text2);
			observations.push_back(o1);
		}

		val = 1;
		return val;
	}

	return val;

}

Observation ObservationSet::ReturnObservationSet(int index){

	if(index < 0)
		printf("Out of Bounds!\n");
	if(index>observations.size())
		printf("Out of Bounds!\n");

	Observation obv = observations[index];
	
	return obv;

}

void ObservationSet::PrintObservationSet(){

	for(int i=0; i<observations.size(); i++){
		Observation temp = ReturnObservationSet(i);
		temp.PrintObservation();
	}
}

#endif