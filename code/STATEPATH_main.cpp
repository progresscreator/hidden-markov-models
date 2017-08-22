#include "observation.h"
#include "HMM.h"

// StatePath
// Main Function
int main(int argc, char *argv[]){

	// Check Input
	if(argc < 3){
		cout << "Use Format: statepath *.hmm *.obs (*.obs ...)" << endl;
		return 0;
	}

	// Declare HMM
	HMM h1;

	// Read In HMM
	int noerr = h1.Process(argv[1]);

	// Check HMM Input
	if(!noerr){
		printf("Invalid .hmm input");
		return 0;
	}

	// Determine Observation Inputs
	int observationInputs = argc-2;
	char** observationFiles = &argv[2];

	// Declare Vector of ObservationSets
	vector<ObservationSet> ObservationSets;
	for(int x=0; x<(argc - 1); x++){
		ObservationSet o1;
		ObservationSets.push_back(o1);
	}

	// Read In All Observation Files
	for(int x = 0; x < observationInputs; ++x){
			noerr = ObservationSets[x].Process(observationFiles[x]);
			
			if(!noerr){
				printf("Invalid .obs input");
				return 0;
			}
		}

	// StatePath
	for(int x = 0; x < observationInputs; ++x){

			vector<Edge> output = h1.StatePathSet(&ObservationSets[x]);
			for(int y = 0; y < ObservationSets[x].numObs; ++y)
				h1.PrintEdge(output[y]);
		}
	
	return 0;
}