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
    for(int i = 0; i<MAX_EXE;i++){
        fgets(os.paths[i],256,file);
        printf("%s", os.paths[i]);
        size_t len = strlen(os.paths[i]);
        if (os.paths[i][len - 1] == '\n') {  // Check if the last character is '\n'
            os.paths[i][len - 1] = '\0';  // Replace it with the null terminator
        }  
    }
    fclose(file);


    int L = argc - 2; //L = length of [p1,p2,...,pn]
    //Loop 1 time for each parameter input by user
    for(int i = 0; i<L; i++){
        int done_executables = 0;
        while(done_executables<MAX_EXE){
            for(int j = 0; j<batch_size; j++){
                pid_t pid = fork();
                if (pid < 0){
                    perror("Fork Failed");
                    exit(EXIT_FAILURE);
                }else if (pid == 0){
                    char* slash = strrchr(os.paths[done_executables+j], '/');
                    char* filename = slash+1;
                    if (execl(os.paths[done_executables+j],filename, argv[i+2], NULL) == -1) {
                        perror("execv failed");  
                        exit(1); 
                    }
                }
            }

            int finished = 0;
            //ensure all from this batch are done
            while(finished<batch_size){
                int status; //we will need to store this somewhere, and check exit statuses below
                waitpid(-1, &status, 0);
                if (WIFEXITED(status)){
                    printf("Child exited with status: %d\n", WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)){
                    printf("Child terminated with signal: %d\n", WTERMSIG(status));
                } 
                finished++;
            }
            //We finished a batch, so add that many to done executables
            done_executables += batch_size;
        }
    }
    
    //TODO: Write the status of each executable file from "submissions.txt" to autograder.out. For testing purposes, you can compare the results with the provided expected.out file
    print_status();    


    return 0;
}     
