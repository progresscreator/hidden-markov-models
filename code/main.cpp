#include "observation.h"
#include "HMM.h"

// Optimize
// Main Function
int main(int argc, char *argv[]){

	// Check Input
	if(argc != 4){
		cout << "Use Format: optimize *.hmm *.obs *.hmm" << endl;
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

	// Declare Observation Set
	ObservationSet o1;
	
	// Read In Observation File
	noerr = o1.Process(argv[2]);
			
	if(!noerr){
		printf("Invalid .obs input");
		return 0;
		}

	// Produce Original Results
	vector<float> results_1 = h1.HMMRecognizeSet(&o1);

	// Optimize
	h1.OptimizeSet(&o1);

	// Produce New Results
	vector<float> results_2 = h1.HMMRecognizeSet(&o1);

	// Display Results
	for(int x = 0; x < o1.numObs; ++x)
			cout << results_1[x] << " " << results_2[x] << " " << endl;

	// Store Optimimized HMM
	h1.StoreHMM(argv[3]);
	
	return 0;
}