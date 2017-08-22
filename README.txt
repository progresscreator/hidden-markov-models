Hidden Markov Models

Goals

The purpose of this project is to design a basic English sentence recognizer based on hidden Markov models. Our HMM should be able to recognize and parse sentences consisting of a basic vocabulary, and should successfully perform three recognition tasks: 

I. Pattern Recognition,
by utilizing the "forward part" of the forward/backward procedure discussed in lecture. That is, given an HMM and multiple observation sequences, the program should report the observation probability of each input sequence. 

II. State-Path Determination,
by using the Viterbi algorithm to determine and report the optimal state path for each observation set. 

III. Model Optimization,
by optimizing an input HMM using the Baum-Welch algorithm and reporting the probabilities before and after optimization. 

Main Code

The three goals of this project are met by using three separate main functions which all utilize the same underlying code. These functions are compiled into three separate executables which are specific to goals outlined above.

RECOGNIZE_main.cpp - compiles to recognize.exe. Reads in an hmm and one or more observation files; addresses Goal I by utilizing the forward/backward algorithm to output the observation probability of each input sequence. 
Usage: recognize.exe *.hmm *.obs (*.obs ...) 

STATEPATH_main.cpp - compiles to statepath.exe. Reads in an hmm and one or more observation files; addresses Goal II by using the Veterbi algorithm to report the optimal state path for each input observation set. 
Usage: statepath.exe *.hmm *.obs (*.obs ...) 

OPTIMIZE_main.cpp - compiles to optimize.exe. Reads in an hmm, an observation file, and the filename for an output hmm; addresses Goal III by optimizing the input hmm using the Baum-Welch algorithm; reports the pre- and post-optimization probabilities. 
Usage: optimize.exe *.hmm *.obs *.hmm 

Base Code

The base code is divided into two files, Observation.h and HMM.h, which manage the input observation criteria and input HMM criteria, respectively. Additionally, most algorithms that operate upon input HMMs are stored in HMM.h.

Observation.h - contains two classes, Observation and ObservationSet, which are responsible for managing the input and storage of single observations and observation groups, respectively. Observations are parsed from input files and stored into memory via functions defined within these two classes. 

HMM.h - contains one class, HMM, and one struct, Edge, which together are responsible for representing the structure, probabilities, and interrelationships of an input HMM. These input HMMs are parsed from input files and stored into memory via functions defined within class HMM. The forward/backward algorithm, Veterbi algorithm, and Baum-Welch method are also implemented within class HMM, along with methods for generating and storing the alpha, beta, gamma, delta, phi, and xi tables on which these computations are dependent. Needed observation sets are accessed directly from Observation.h, and methods are provided for the storage of an optimized HMM via text file. 


Recognize

My program performs well in utilizing the forward/backward algorithm, achieving 100% accuracy on all test data. My program also gracefully handles malformed input files, and is stable across all tested input observation and HMM files. I believe my implementation to be correct for this part of the assignment.

Question:
For the current application, why does this probability seem lower than we expect? What does this probability tell you? Does the current HMM always give a reasonable answer? For instance, what is the output probability for the below sentences?

Sentence 1: "robots do kids play chess" 
Sentence 2: "chess eat play kids" 

Answer:
For the current application, the probabilities seem lower than we expect because the input HMM has not been adequately trained on the observation set. Even though we are getting low probabilities for well-formed sentences, we are getting probabilities approaching zero for incorrect sentence formations -- indicating that the HMM is capable of separating incorrect from correct sentence formations. In this sense, the HMM output is indeed reasonable.

Probabilities for test sentences: 
Sentence 1: "robots do kids play chess" = 0.00151 
Sentence 2: "chess eat play kids" = 0.0 

Neither of these two sentences are correct sentence formations, and thus they receive very low probabilities. 

StatePath

My program performs well in utilizing the Veterbi algorithm, achieving 100% accuracy on all provided test data. My code is reliably able to determine and report the optimal state path for each input observation set. I believe my implementation to be correct for this part of the assignment.

Question:
What can we tell from the reported optimal path for syntax analysis purpose? Can the HMM always correctly distinguish "statement" from "question" sentence? Why?

Answer:
For the purposes of syntax analysis, the reported optimal path (Subject Predicate Object) enables us to segment the sentence and determine the individual meanings of each word. The HMM cannot always distinguish statements from questions, since some sentence formations (such as "robots eat kids") are valid as both questions and statements. Without the aid of punctuation, or interrogative hints like the word "do", the HMM will inevitably classify some statements as questions -- and vice versa.


Optimize

My program performs well in using the Baum-Welch method, achieving 100% accuracy on all provided test data. My code is reliably able to optimize input HMMs using the Baum-Welch method, and consistently outputs the correct optimized HMM in text format. I believe my implementation to be correct for this part of the assignment.

Question:
Why should you not try to optimize an HMM with zero observation probability?

Answer:
As I learned during coding, optimizing an HMM using the Baum-Welch method and zero observation probabilities produces divide-by-zero errors, since many formulae in Baum-Welch use P(O|lambda) as a denominator. Thus, it is never a good idea to introduce zero observation probabilities into the optimization algorithm.


Model Enhancement

Question:
What kinds of changes will you need to make in the above HMM? Please describe your solution with an example of the modified matrices a, b and pi in the submitted web page.

Answer:
The additional states of PRESENT TENSE and ADVERB must be added to the HMM state list. State-to-state relationship structures will need to be updated to account for the extra states: this includes updating the probabilities for state changes, probabilities for each observation for each state, and probabilities associated with initially entering into a particular state. Below, I provide sample matrices a, b, and sample vector pi with initial probability values set to uniform.


SUBJECT AUXILIARY PREDICATE OBJECT PRESENT-TENSE ADVERB
a:
	0.16667 0.16666 0.16667 0.16666 0.16667 0.16666
	
	0.16667 0.16666 0.16667 0.16666 0.16667 0.16666
	
	0.16667 0.16666 0.16667 0.16666 0.16667 0.16666
	
	0.16667 0.16666 0.16667 0.16666 0.16667 0.16666
	
	0.16667 0.16666 0.16667 0.16666 0.16667 0.16666
	
	0.16667 0.16666 0.16667 0.16666 0.16667 0.16666
b:
	0.125 0.125 0.125 0.125 0.125 0.125 0.125 0.125
	
	0.125 0.125 0.125 0.125 0.125 0.125 0.125 0.125
		
	0.125 0.125 0.125 0.125 0.125 0.125 0.125 0.125
		
	0.125 0.125 0.125 0.125 0.125 0.125 0.125 0.125
		
	0.125 0.125 0.125 0.125 0.125 0.125 0.125 0.125
		
	0.125 0.125 0.125 0.125 0.125 0.125 0.125 0.125
pi:
	0.16667 0.16666 0.16667 0.16666 0.16667 0.16666


----------------------------------------
Code:

Hidden Markov Model Project


binaries - contains executables built from my code:
	recognize.exe, statepath.exe, optimize.exe

code - contains c++ code and VS2005 project files


COMPILATION INSTRUCTIONS

To switch compilation between recognize, statepath, and optimize, you must
swap out the current main file (main.cpp) with the corresponding main file
(RECOGNIZE_main.cpp, STATEPATH_main.cpp, or OPTIMIZE_main.cpp). You can
do this by simply deleting the current main.cpp file, renaming one of the
three other main files to "main.cpp", and rebuilding the solution.

David House

