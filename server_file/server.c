#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <assert.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#define N_  5

void run_command_method(int filedes , char **cmd) {  /* ---- Helper function to execute ---- */
    switch (fork()) {  /* ---- fork to make a child to run the execvp method ---- */
        case 0:
            dup2(filedes, STDOUT_FILENO); /* ---- dup2 redirects the stdin to the output file to the ---- */
            dup2(filedes, STDERR_FILENO); /* ---- dup2 redirects the stderr to the output file to the ---- */
            execvp(cmd[0], cmd); /* ---- executes the cmd arg array ---- */
            perror("run_command_method execvp failed"); /* ---- prints error to stdout ---- */
            exit(1);
            
        default:
            while (wait(NULL) != -1) ; /* ---- collects all the children ---- */
            break;
            
        case -1:
            perror("fork"); /* ---- redirects the err to the stdout file stream ---- */
            
    }
    fflush(NULL); /* ---- flushes the current files ---- */
    return;
}

int main(int argv, char * argc[]) {
    
    int welcomeSock;                            /* ---- welcomSock is the main connection socket ---- */
    int greySock;                               /* ---- greySock ... he he ... is the client socket ---- */
    char buffer[1024];                          /* ---- buffer is the recieved buffer for the input message ---- */
    struct sockaddr_in serverAddr;              /* ---- Address of the socket ---- */
    struct sockaddr_storage serverStorage;      /* ---- Something to do with the storage maybe idk ---- */
    
    socklen_t addr_size;    /* ---- size of address ---- */
    
    pid_t client_pid;       /* ---- client pid ---- */
    
    int i;
    
    welcomeSock = socket(AF_INET, SOCK_STREAM, 0);  /* ---- Sets up socket n' stuff ---- */
    if(welcomeSock < 0){            /* ---- Checks for socket failure ---- */
        printf("Error in server socket\n");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;            /* ---- Standard number for TCP socket ---- */
    
    serverAddr.sin_port = htons(2505);          /* ---- port number it wants ---- */
    
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    /*---- Bind the address struct to the socket ----*/
    bind(welcomeSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSock,5) == 0)
        printf("Listening ... \n\n");
    else
        printf("Error in binding server side. \n");
    
    // SETS A CHILD ARRAY
    pid_t child_arr[20];
    int j, child_i; // child_i is the index of the child array used in the parent process.
    for(j = 0; j < 20; j++)
    {
        child_arr[j] = 0;
    }
    
    while(1){ /*---- Starts main while-loop ----*/
        /*---- Accept call creates a new socket for the incoming connection ----*/
        addr_size = sizeof serverStorage;
        greySock = accept(welcomeSock, (struct sockaddr *) &serverStorage, &addr_size); /*---- Starts the client socket ----*/
        
        if(greySock < 0){ /*---- Checks to see if the socket is  ----*/
            perror("accept call failed");
            exit(1);
        }
        
        if((client_pid = fork()) == 0) {    /*---- Starts the child ----*/
            
            close(welcomeSock); /*---- Closes the welcome socket ----*/
            
            while(1) {
                
                int i;
                
                recv(greySock, buffer, sizeof(buffer), 0);  /*---- recieves message from client ----*/
                
                char *word; /*---- pointer to word ----*/
                char *brkt;/*---- pointer to break ----*/
                char *sep = " "; /*---- how we break stuff ----*/

                int str_len = strlen(buffer); /*---- recieves message from client ----*/
                
                for(i = 0; i < str_len - 1; i++) { /* Decryption */
                    buffer[i] = buffer[i] - N_;
                }
                
                if(buffer[0] == 'q' && buffer[1] == 'u' && buffer[2] == 'i' && buffer[3] == 't'){    /*---- if the user enters "quit" as input the terminal quits ----*/
                    exit(0);
                }
                
                char test[str_len]; /*---- copy length of input message ----*/

                strcpy(test, buffer); /*---- copies string to test ----*/

                for(i = 0; i < str_len; i++){  /*---- removes the '\n' since it will  ----*/
                    if(test[i] == '\n')
                        test[i] = ' ';
                }

                int word_count;
                word_count = 0;

                /* ---- Counts the number of words which will be separated by spaces ---- */
                for(i = 0; i < str_len; i++){
                    if(buffer[i] == ' ')
                        word_count++;
                }
                word_count++;

                int bld_str_len = word_count + 1; /*---- Builds the length of the string ----*/

                char *build_str[bld_str_len]; /*---- removes the '\n' since it will  ----*/

                i = 0;
                /*---- builds the argument array that is entered in the execvp helper method  ----*/
                for (word = strtok_r(test, sep, &brkt); word; word = strtok_r(NULL, sep, &brkt)) {
                    build_str[i] = word;
                    i++;
                }

                build_str[i] = NULL;  /*---- must end with a NULL element ----*/
                
                FILE *file;

                file = fopen("test.txt", "w");

                int file_des = fileno(file);
                
                run_command_method(file_des, build_str); /*---- Calls the run command method to get the execvp to stdout ----*/

                /*---- gets the length of the file  ----*/
                int fileLen;
                fseek(file, 0, SEEK_END);
                fileLen=ftell(file);
                fseek(file, 0, SEEK_SET);
                fclose(file);

                /*---- builds the message to be sent  ----*/
                file = fopen("test.txt","r");
                char out_msg[fileLen + 1];
                fread(out_msg, fileLen, 1, file);
                out_msg[fileLen] = 0;
                fclose(file);

                send(greySock, out_msg, sizeof(out_msg), 0); /*---- sends the message  ----*/
                
                bzero(buffer, sizeof(buffer)); /*---- zeros the buffer  ----*/
                bzero(out_msg, sizeof(out_msg)); /*---- zeros the message  ----*/
                
            } // inner while-loop
            
        } else {
            
            if(waitpid(client_pid, NULL, 0) > 0){
                kill(getpid(), SIGCHLD);
            }
            
        }
        
    } // big while()
    
    close(greySock);
    return 0;
}
