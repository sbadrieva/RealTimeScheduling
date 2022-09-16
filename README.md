# RealTimeScheduling
Rate Monotonic, Deadline Monotonic and Earliest Deadline First Scheduling Algorithms Implementation and Experiments


Abstract

This article presents a real time simulation of the three most popular scheduling algorithms, Rate Monotonic, Earliest Deadline and Deadline monotonic based on controlled simulated inputs. We utilized random number generators to simulate our input parameters such as arrival times, execution times, deadlines and periods for each respective process. We proposed an algorithm for modifying the generated period durations of each process so the modified period durations are close to each other and will result in the smallest possible least common multiples. This solves the problem of systems running out of memory while looking for the closest LCM especially those periods with odd or prime numbers. In extensive testing, our proposed algorithm shows Earliest Deadline scheduling have the highest success rate in feasibility. 


***************************************************************************************************************************************
***************************************************************************************************************************************
HOW TO RUN CODE
***************************************************************************************************************************************
***************************************************************************************************************************************


What and where each file is:
The input file holds the input information
The sim file is a bash script where different options are parsed
The algorithm specific codes are in there respective folders 
The folder named Paper holds the final paper report 
The file titled input_generator.c outputs the values needed for our input based on specific ranges and probabilities defined
The Folder titled Final Presentation holds the presentation for our project
The folder titled Results holds the findings of each algorithm



To run code:
Note: If input file is changed, execution time of processes needs to be hardcoded into the cpp files of each algorithm in folders RM_Algorithm, EDF_Algorithm, and DM_algorithm 

Change into the directory titled CSC716FinalProject
Run command "chmod +x sim"
Run different commands based on preferred options. Commands include:
	Default output: 	./sim <input

 
	Algorithm Specific Output: 	./sim -a RM <input
					./sim -a EDF <input
					./sim -a DM <input


	Detailed Output:	./sim -d <input

	Verbose Output:		./sim -v <input 

Note: If running ./sim returns "operation not permitted" then first run the following command before running sim:
xattr -d com.apple.quarantine <path to sim>
 




