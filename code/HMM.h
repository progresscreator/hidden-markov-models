#ifndef _HHM_H
#define _HMM_H
#include "observation.h"
#include <fstream>

struct Edge{

	int length;
	vector<int> states;
	float probability;
};

class HMM{

public:

	int stateCount;
	int observationCount;
	int timeCount;
	vector<string> stateIDs;
	vector<string> observationIDs;
	vector<vector<float>> matrix_a;
	vector<vector<float>> matrix_b;
	vector<float> vector_pi;

	HMM();
	void StoreHMM(char* filename);
	int	Process(char* filename);
	int Process(HMM* copy);
	int	ReturnObIndex(string ob);
	float ReturnObProbability(int index, string ob);
	void PrintEdge(Edge edge);

	float HMMRecognize(Observation* ob);
	vector<float> HMMRecognizeSet(ObservationSet* obs);
	Edge StatePath(Observation* ob);
	vector<Edge> StatePathSet(ObservationSet* obs);
	void BaumWelchMethod(Observation* ob);
	void Optimize(Observation* ob);
	void OptimizeSet(ObservationSet* obs);

	vector<vector<float>> MakeAlpha(Observation* ob);
	vector<vector<float>> MakeDelta(Observation* ob);
	vector<vector<int>> MakePhi(Observation* ob);
	vector<vector<float>> MakeBeta(Observation* ob);
	vector<vector<float>> MakeGamma(vector<vector<float>> alpha, vector<vector<float>> beta, int obCount);
	vector<vector<vector<float>>> MakeXi(vector<vector<float>> alpha, vector<vector<float>> beta, Observation* ob);

};

HMM::HMM(){

	stateCount = 0;
	observationCount = 0;
	timeCount = 0;
}

int HMM::Process(char* filename){

	int val = 0;
	char  text[500];
	FILE* file = fopen(filename, "r");

	if(file){

		// Get the first line--------------------------------------------------------
		fgets(text, sizeof(text), file);

		// If there's nothing there, close the file
		if(!text){
			fclose(file);
			return val;
		}

		// Get the State, Observation, and Time Counts
		int i = 0;
		string input(text);
		string value;
		stringstream stream(input);
		while( getline(stream, value, ' ') ){
			if(i==0)
				stateCount = atoi(value.c_str());
			if(i==1)
				observationCount = atoi(value.c_str());
			if(i==2)
				timeCount = atoi(value.c_str());
			i++;
		}

		// Allocate Memory
		// stateIDs: array of strings, length stateCount
		// observationIDs: array of strings, length observationCount
		// matrix_a: matrix of floats, stateCount x stateCount
		// matrix_b: matrix of floats, stateCount x observationCount
		// vector_pi: array of floats, length stateCount
		for(int y=0; y<stateCount; y++){
			matrix_a.push_back(vector <float>());
			matrix_b.push_back(vector <float>());
			vector_pi.push_back(0);
		}
  
		for(int y=0; y<stateCount; y++)
			for(int x=0; x<stateCount; x++)
				matrix_a[y].push_back(0);

		for(int y=0; y<stateCount; y++)
			for(int x=0; x<observationCount; x++)
				matrix_b[y].push_back(0);


		// Get Next Line--------------------------------------------------------
		fgets(text, sizeof(text), file);
		if(!text){
			fclose(file);
			return val;
		}

		// Convert to String
		string input2(text);
		string name;

		// Find Line Ending
		char end = '0';
		i = 0;
		while(end != '\n' && end != '\0'){
			end = input2[i];
			i++;
			}

		// Replace Line Ending with Null Terminator
		input2[i-1] = '\0';

		// Store State Names
		i = 0;
		stringstream stream2(input2);
		while( getline(stream2, name, ' ') ){
			if(i>stateCount){
				cout << "Error in HMM File: Invalid State Count\n";
				return val;
			}
			if(i<stateCount)
				stateIDs.push_back(name);
			i++;
		}

		// Get Next Line--------------------------------------------------------
		fgets(text, sizeof(text), file);
		if(!text){
			fclose(file);
			return val;
		}

		// Convert to String
		string input3(text);
		string word;

		// Find Line Ending
		end = '0';
		i = 0;
		while(end != '\n' && end != '\0'){
			end = input3[i];
			i++;
			}

		// Replace Line Ending with Null Terminator
		input3[i-1] = '\0';

		// Store State Names
		i = 0;
		stringstream stream3(input3);
		while( getline(stream3, word, ' ') ){
			if(i>observationCount){
				cout << "Error in HMM File: Invalid Observation Count\n";
				return val;
			}
			if(i<observationCount)
				observationIDs.push_back(word);
			i++;
		}

		// Get Next Line--------------------------------------------------------
		char * pxt;
		fgets(text, sizeof(text), file);
		if(!text){
			fclose(file);
			return val;
		}

		for(int x=0; x <stateCount;++x){

			fgets(text, sizeof(text), file);
			if(!text){
				fclose(file);
				return val;
			}

			pxt = strtok(text, " \n");
			for(int y=0; y<stateCount;++y){
				
				if(!pxt){
					fclose(file);
					return val;
				}

				matrix_a[x][y] = (float)atof(pxt);
				pxt = strtok(NULL, " \n");
			}
		}

		fgets(text, sizeof(text), file);
		if(!text){
			fclose(file);
			return val;
		}

		for(int x=0; x< stateCount;++x){
			fgets(text, sizeof(text), file);
			if(!text){
				fclose(file);
				return val;
			}

			pxt = strtok(text, " \n");
			for(int y=0; y<observationCount;++y){
				
				if(!pxt){
					fclose(file);
					return val;
				}

				matrix_b[x][y]=(float)atof(pxt);
				pxt = strtok(NULL, " \n");
			}
		}

		fgets(text, sizeof(text), file);
		if(!text){
			fclose(file);
			return val;
		}

		fgets(text, sizeof(text), file);
		if(!text){
			fclose(file);
			return val;
		}

		pxt = strtok(text, " \n");
		for(int x=0; x<stateCount;++x){
			if(!pxt){
				fclose(file);
				return val;
			}

			vector_pi[x] = (float)atof(pxt);
			pxt = strtok(NULL, " \n");
		}

		fclose(file);
		val = 1;

		return val;

	}

	return val;
}


int HMM::Process(HMM* copy){

	matrix_a.clear();
	matrix_b.clear();
	vector_pi.clear();
	observationIDs.clear();
	stateIDs.clear();


	int val = 0;

	// Copy Basics
	stateCount = copy->stateCount;
	observationCount = copy->observationCount;
	timeCount = copy->timeCount;

	// Allocate Memory
	for(int y=0; y<stateCount; y++){
			stateIDs.push_back("");
			matrix_a.push_back(vector <float>());
			matrix_b.push_back(vector <float>());
			vector_pi.push_back(0);
		}

	for(int i=0; i<observationCount; i++)
		observationIDs.push_back("");
  
	for(int y=0; y<stateCount; y++)
		for(int x=0; x<stateCount; x++)
			matrix_a[y].push_back(0);

	for(int y=0; y<stateCount; y++)
		for(int x=0; x<observationCount; x++)
			matrix_b[y].push_back(0);

	// Copy Information
	for(int i=0; i<matrix_a.size(); i++)
			for(int j=0; j<matrix_a[i].size(); j++)
				matrix_a[i][j] = copy->matrix_a[i][j];

	for(int i=0; i<matrix_b.size(); i++)
			for(int j=0; j<matrix_b[i].size(); j++)
				matrix_b[i][j] = copy->matrix_b[i][j];

	for(int i=0; i<vector_pi.size(); i++)
		vector_pi[i] = copy->vector_pi[i];

	for(int i=0; i<stateIDs.size(); i++)
		stateIDs[i] = copy->stateIDs[i];

	for(int i=0; i<observationIDs.size(); i++)
		observationIDs[i] = copy->observationIDs[i];


	val = 1;
	return val;
}

int	HMM::ReturnObIndex(string ob){

	for(int i = 0; i < observationCount; ++i){

		if(strcmp(observationIDs[i].c_str(), ob.c_str()) == 0)
			return i;
	}

	return 0;
}

vector<vector<float>> HMM::MakeAlpha(Observation* ob){

	// Allocate Alpha Matrix
	// Alpha: stateCount x observationCount
	vector<vector<float>> alpha;
	for(int y=0; y<stateCount; y++)
			alpha.push_back(vector <float>());

	for(int y=0; y<stateCount; y++)
		for(int x=0; x<observationCount; x++)
			alpha[y].push_back(0);

	int ob_count = ob->numWords;
	int ob_index = ReturnObIndex(ob->ReturnObservation(0));
	for(int i = 0; i < stateCount; ++i)
		alpha[i][0] = vector_pi[i] * matrix_b[i][ob_index];

	for(int z = 1; z < ob_count; ++z){

		ob_index = ReturnObIndex(ob->ReturnObservation(z));
		for(int c = 0; c < stateCount; ++c){

			float total = 0;
			for(int i = 0; i < stateCount; ++i)
				total += (alpha[i][z-1] * matrix_a[i][c]);

			alpha[c][z] = total * matrix_b[c][ob_index];
		}
	}

	return alpha;

}


float HMM::HMMRecognize(Observation* ob){

	float output = 0;
	vector<vector<float>> alpha;

	alpha = MakeAlpha(ob);

	for(int i = 0; i < stateCount; ++i)
		output += alpha[i][ob->numWords - 1];

	return output;
}

vector<float> HMM::HMMRecognizeSet(ObservationSet* obs){

	vector <float> output;

	for(int i = 0; i < obs->numObs; ++i)
		output.push_back(HMMRecognize(&obs->ReturnObservationSet(i)));

	return output;
}

vector<vector<float>> HMM::MakeDelta(Observation* ob){

	vector<vector<float>> delta;
	int total_obs    = ob->numWords;
	int ob_index = ReturnObIndex(ob->ReturnObservation(0));

	for(int i=0; i<stateCount; i++)
		delta.push_back(vector <float> () );

	for(int i=0; i<stateCount; i++)
		for(int j=0; j<observationCount; j++)
			delta[i].push_back(0);

	for(int x = 0; x < stateCount; ++x)
		delta[x][0] = vector_pi[x] * matrix_b[x][ob_index];

	for(int z = 1; z < total_obs; ++z){

		ob_index = ReturnObIndex(ob->ReturnObservation(z));
		for(int j = 0; j < stateCount; ++j){

			float top = delta[0][z-1] *  matrix_a[0][j];
			int priorState = 0;

			for(int i = 1; i < stateCount; ++i){
				if(delta[i][z-1] > top){
					top	= delta[i][z-1] *  matrix_a[i][j];
					priorState = i;
				}
			}

			delta[j][z] = top * matrix_b[j][ob_index];
		}
	}

	return delta;

}

vector<vector<int>> HMM::MakePhi(Observation* ob){

	vector<vector<float>> delta;
	vector<vector<int>> phi;
	int total_obs    = ob->numWords;
	int ob_index = ReturnObIndex(ob->ReturnObservation(0));

	for(int i=0; i<stateCount; i++){
		delta.push_back(vector <float> () );
		phi.push_back(vector <int> () );
	}

	for(int i=0; i<stateCount; i++)
		for(int j=0; j<observationCount; j++){
			delta[i].push_back(0);
			phi[i].push_back(0);
		}

	for(int x = 0; x < stateCount; ++x){
		delta[x][0] = vector_pi[x] * matrix_b[x][ob_index];
		phi[x][0] = 0;
	}

	for(int z = 1; z < total_obs; ++z){

		ob_index = ReturnObIndex(ob->ReturnObservation(z));
		for(int j = 0; j < stateCount; ++j){

			float top = delta[0][z-1] *  matrix_a[0][j];
			int priorState = 0;

			for(int i = 1; i < stateCount; ++i){
				if(delta[i][z-1] > top){
					top	= delta[i][z-1] *  matrix_a[i][j];
					priorState = i;
				}
			}

			delta[j][z] = top * matrix_b[j][ob_index];
			phi[j][z] = priorState;
		}
	}

	return phi;

}

Edge HMM::StatePath(Observation* ob)
{
	vector<vector<float>> delta;
	vector<vector<int>> phi;

	delta = MakeDelta(ob);
	phi = MakePhi(ob);
	
	Edge output;
	output.length = 0;
	output.probability = delta[0][(ob->numWords) - 1];
	int end_state = 0;
	int word_index = (ob->numWords) - 1;

	for(int x = 1; x < stateCount; ++x){

		if(delta[x][word_index] > output.probability){
			output.probability = delta[x][word_index];
			end_state = x;
		}
	}

	if(output.probability != 0.0f){

		output.length = ob->numWords;
		for(int z=0; z<output.length; z++)
			output.states.push_back(0);

		output.states[word_index] = end_state;

		for(int x = (output.length)-2; x >= 0; --x)
			output.states[x] = phi[output.states[x+1]][x+1];
	}

	return output;
}

vector<Edge> HMM::StatePathSet(ObservationSet* obs){

	vector<Edge> output;

	for(int x = 0; x < obs->numObs; ++x)
		output.push_back(StatePath(&obs->ReturnObservationSet(x)) );

	return output;
}

void HMM::PrintEdge(Edge edge){

	cout << edge.probability << " ";

	for(int x = 0; x < edge.length; ++x)
		cout << stateIDs[edge.states[x]] << " ";

	cout << endl;

}

float HMM::ReturnObProbability(int index, string ob){

	int index2 = ReturnObIndex(ob);
	float output = matrix_b[index][index2];
	return output;

}

vector<vector<float>> HMM::MakeBeta(Observation* ob){

	vector<vector<float>> beta;
	for(int i=0; i<stateCount; i++)
		beta.push_back(vector <float> () );

	for(int i=0; i<stateCount; i++)
		for(int j=0; j<observationCount; j++)
			beta[i].push_back(0);

	int ob_count = ob->numWords;
	for(int i = 0; i < stateCount; ++i)
		beta[i][ob_count-1]	= 1.0;

	for(int z = ob_count-2; z>=0; --z){

		int ob_index = ReturnObIndex(ob->ReturnObservation(z+1));

		for(int i = 0; i<stateCount; ++i){

			float tp = 0;
			for(int j = 0; j < stateCount; ++j)
				tp += (matrix_a[i][j] * beta[j][z+1] * matrix_b[j][ob_index]);

			beta[i][z] = tp;
		}
	}

	return beta;

}

vector<vector<float>> HMM::MakeGamma(vector<vector<float>> alpha, vector<vector<float>> beta, int obCount){

	vector<vector<float>> gamma;
	for(int i=0; i<stateCount; i++)
		gamma.push_back(vector <float> () );

	for(int i=0; i<stateCount; i++)
		for(int j=0; j<observationCount; j++)
			gamma[i].push_back(0);

	for(int x = 0; x < stateCount; ++x){

		for(int z = 0; z < obCount; ++z){

			float tp = 0;
			for(int j = 0; j < stateCount; ++j){
				
				float a = alpha[j][z];
				float b  = beta[j][z];
				tp += (alpha[j][z]) * (beta[j][z]);
			}

			gamma[x][z] = ((alpha[x][z] * beta[x][z]) / tp);
		}
	}

	return gamma;

}

vector<vector<vector<float>>> HMM::MakeXi(vector<vector<float>> alpha, vector<vector<float>> beta, Observation* ob){

	int	ob_count = ob->numWords;

	vector<vector<vector<float>>> xi;
	for(int i=0; i<stateCount; i++)
		xi.push_back(vector<vector<float>> () );

	for(int i=0; i<stateCount; i++)
		for(int j=0; j<stateCount; j++)
			xi[i].push_back(vector<float> () );

	for(int i=0; i<stateCount; i++)
		for(int j=0; j<stateCount; j++)
			for(int k=0; k<ob_count-1; k++)
				xi[i][j].push_back(0);

	float probability = HMMRecognize(ob);
	for(int i = 0; i < stateCount; ++i)
		for(int j = 0; j <stateCount; ++j)
			for(int k = 0; k < ob_count-1; ++k)
				xi[i][j][k] = ((alpha[i][k] * matrix_a[i][j] * ReturnObProbability(j, ob->ReturnObservation(k+1)) * beta[j][k+1]) / probability);

	return xi;

}

void HMM::BaumWelchMethod(Observation* ob){

	int	ob_count = ob->numWords;
	vector<vector<float>> alpha = MakeAlpha(ob);
	vector<vector<float>> beta = MakeBeta(ob);
	vector<vector<vector<float>>> xi = MakeXi(alpha, beta, ob);
	vector<vector<float>> gamma	= MakeGamma(alpha, beta, ob_count);

	if(ob_count > timeCount)
		timeCount = ob_count;

	for(int x = 0; x < stateCount; ++x){

		for(int y = 0; y < stateCount; ++y){

			float ns = 0.0;
			float ds = 0.0;
			for(int z = 0; z < (ob_count-1); ++z){
				ns += xi[x][y][z];
				ds += gamma[x][z];
			}

			if(ds >= 1)
				matrix_a[x][y] = (ns/ds);
		}


		for(int y = 0; y < observationCount; ++y){

			float ns = 0.0;
			float ds = 0.0;
			for(int z = 0; z < ob_count; ++z){

				if(ReturnObIndex(ob->ReturnObservation(z)) == y)
					ns += gamma[x][z];

				ds += gamma[x][z];
			}

			if(ds> 0)
				matrix_b[x][y] = (ns/ds);
		}

		vector_pi[x] = gamma[x][0];
	}

}

void HMM::Optimize(Observation* ob){

	if(HMMRecognize(ob) != 0){

		HMM h2;
		h2.Process(this);
		float pt = h2.HMMRecognize(ob);
		h2.BaumWelchMethod(ob);

		if(h2.HMMRecognize(ob) > HMMRecognize(ob))
			this->Process(&h2);

	}

}


void HMM::OptimizeSet(ObservationSet* obs){

	for(int x = 0; x < obs->numObs; ++x)
		Optimize(&obs->ReturnObservationSet(x));

}

void HMM::StoreHMM(char* filename){

	std::ofstream ofile;
	ofile.open(filename);

	if(ofile){
		
		ofile << stateCount << " " << observationCount << " " << timeCount << "\n";

		for(int x = 0; x < stateCount; ++x){
			
			ofile << stateIDs[x];

			if(x < (stateCount-1))
				ofile << " ";

			else
				ofile << "\n";
		}

		for(int x = 0; x < observationCount; ++x){

			ofile << observationIDs[x];

			if(x < (observationCount-1))
				ofile << " ";

			else
				ofile << "\n";
		}

		ofile << "a:\n";
		for(int x = 0; x < stateCount; ++x){
			for(int y = 0; y < stateCount; ++y){

				ofile << matrix_a[x][y];

				if(y < (stateCount-1))
					ofile << " ";
				
				else
					ofile << "\n";
				
			}
		}

		ofile << "b:\n";
		for(int x = 0; x < stateCount; ++x){
			for(int y = 0; y < observationCount; ++y){

				ofile << matrix_b[x][y];

				if(y < (observationCount-1))
					ofile << " ";
				
				else
					ofile << "\n";
				
			}
		}

		ofile << "pi:\n";
		for(int x = 0; x < stateCount; ++x){

			ofile << vector_pi[x];
			if(x < (stateCount-1))
				ofile << " ";
		}

		ofile.close();
	}

	else
		cout << "Problem Opening File for Saving.\nDo You Have Write Permission?" << endl;

}


#endif