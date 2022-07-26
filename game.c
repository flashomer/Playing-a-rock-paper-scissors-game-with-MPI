#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>	/* For every time random number	*/
#include <mpi.h>		/* For MPI functions	*/


// Create random number function
int randNumber(int n)
{
	int randMax = RAND_MAX - (RAND_MAX % n);
	int ret;
	while ((ret = rand()) >= randMax);
	return ret/(randMax / n);
}


int main(int argc, char *argv[])
{

	char *selectedName[] = { "ROCK", "PAPER", "SCISSORS" }; 	/* Selected number Name of items  */
	int score1=0,score2=0; 						/* Score1 of Process-0 and Score2 of Process-1 */
	int rNum1,rNum2; 						/* Process-0 Random Number value and Process-1 Random Number value */
	int randMaxNum=3; 						/* Maximum random number value */
	int TurnNumber = 0,maxTurnNumber = 20; 				/* Game turn number value and Max Loop number value */
	
	int	comm_sz; 						/* Number of processes */
	int	my_rank; 						/* My process rank	*/
	
	
	//Tells MPI to do all the necessary setup.
	MPI_Init(NULL, NULL);
	
	//number  of processes in the communicator
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
	
	//the process making this call
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	
	if (my_rank == 0) { printf("The Game Starts\n---\n"); }
	
	
	// The time is reset each time the game starts
	srand(time(0));
	
	
	// Game Turn Loop
	for(int i = 0; i<maxTurnNumber; i++){

		
		rNum1 = randNumber(randMaxNum); /* generate random number of process-0 */
		rNum2 = randNumber(randMaxNum); /* generate random number of process-1 */
		
					
		if (my_rank != 0) {
		
			//Suppose process q calls MPI_Send 
			MPI_Send(&rNum1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
		} else {

			//Suppose that process r calls MPI_Recv
			MPI_Recv(&rNum1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Turn %d, Process-%d: %s , Process-%d: %s \n", i+1,my_rank,selectedName[rNum1],(my_rank==0)?1:0,selectedName[rNum2]);
		

			// Compare to process-0 with process-1
				
			if(rNum1 == rNum2) {
				printf("Drav, Score: %d - %d \n",score1,score2);	
			} else if (rNum1 == 0 && rNum2 == 1) {
				score2++;
				printf("Child Win, Score: %d - %d \n",score1,score2);
					
			} else if (rNum1 == 0 && rNum2 == 2) {
				score1++;
				printf("Parent Win, Score: %d - %d \n",score1,score2);
					
			} else if (rNum1 == 1 && rNum2 == 0) {
				score1++;
				printf("Parent Win, Score: %d - %d \n",score1,score2);
					
			} else if (rNum1 == 1 && rNum2 == 2) {
				score2++;
				printf("Child Win, Score: %d - %d \n",score1,score2);
					
			} else if (rNum1 == 2 && rNum2 == 0) {
				score1++;
				printf("Parent Win, Score: %d - %d \n",score1,score2);
					
			} else if (rNum1 == 2 && rNum2 == 1) {
				score2++;
				printf("Child Win, Score: %d - %d \n",score1,score2);
					
			}
				
			printf("-- \n");
			
			
			//if process gains 5 points, end game
			if(score2 > 4 || score1 > 4 ) { 
				TurnNumber = i; 
				i = maxTurnNumber; 
			}
		
		}		
					
	}

	//Tells MPI we're done, so clean up anything allocated for this program
	MPI_Finalize(); 
		

	//Game over and determination of the winner
	if (my_rank == 0) {
		if(score1 == score2) { 
			printf("Process-0 amd Process-1 have drawn the game with score: %d - %d in %d Turns. \n",score1,score2,TurnNumber);	
		} else if(score1 > score2) { 
			printf("Process-0 has won the game with score: %d - %d in %d Turns. \n",score1,score2,TurnNumber);	
		} else {
			printf("Process-1 has won the game with score: %d - %d in %d Turns. \n",score1,score2,TurnNumber);	
		}
		
		printf("\nThe game ends\n\n");
	}

	
	return 0;	

}
