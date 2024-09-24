#include "include/utility.h"

void print_status(){

     //TODO: write the status of each executable file to autograder.out. Your output should align with expected.out 
}


int main(int argc, char *argv[]) {


    if (argc < 2) {
        printf("Usage: %s <batch> <p1> <p2> ... <pn>\n", argv[0]);
        return 1;
    }

    // Convert the first command-line argument to an integer to determine the batch size
    int batch_size = atoi(argv[1]);


    // write the file paths from the "solutions" directory into the submissions.txt file
    write_filepath_to_submissions("solutions", "submissions.txt");

    //TODO: read the executable filename from submissions.txt
    FILE *file; 
    file = fopen("submissions.txt", "r");

    //hard coded but we can change this later
    int total_executables = 20;

    int L = argc - 2; //L = length of [p1,p2,...,pn]
    //Loop 1 time for each parameter input by user
    for(int i = 0; i<L; i++){
        int done_executables = 0;
        while(done_executables<total_executables){
            pid_t *pid_list = malloc(batch_size * sizeof(pid_t)); 
            for(int j = 0; j<batch_size; j++){
                pid_list[j] = fork();
                if (pid_list[j]<0){
                    perror("Fork Failed");
                    exit(EXIT_FAILURE);
                }else if (pid_list[j] == 0){
                    //TODO: For each parameter, run all executables in batch size chunks          
                    // This will run once for each name
                    char* filename;
                    fgets(filename, 100, file);
                    printf("Child process: I would run filename '%s'.\n",filename);
                    exit(0); 
                }
            }

            int finished = 0;
            //ensure all from this batch are done
            while(finished<batch_size){
                int status; //we will need to store this somewhere, and check exit statuses below
                waitpid(pid_list[finished], &status, 0);
                printf("Child exited\n");
                finished++;
            }
            //We finished a batch, so add that many to done executables
            done_executables += batch_size;
            free(pid_list);
        }
    }
    fclose(file);
    
    //TODO: Write the status of each executable file from "submissions.txt" to autograder.out. For testing purposes, you can compare the results with the provided expected.out file
    print_status();    


    return 0;
}     
