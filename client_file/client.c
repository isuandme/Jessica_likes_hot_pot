#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

#include "historyList.h"

#define N_      5

struct hist_node {
    char *str;
    struct hist_node *next;
};

int main(int argv, char * argc[]){
    
    int clientSocket;   /*---- ClientScoket is this personal clients socket.  ----*/
    
    int port_number;    /*---- Port_number is the port number that is changed for this specific client  ----*/
    
    char buffer[1024];  /*---- Buffer is an input buffer  ----*/
    struct sockaddr_in serverAddr;/*---- removes the '\n' since it will  ----*/
    socklen_t addr_size;
    
    if(argv < 2){   /*---- The below checks and sets the port numbers  ----*/
        printf("argv < 1\n");
        exit(1);
    } else if (argv == 2 && strstr(argc[1],"localhost:") != NULL){
        char *str = strchr(argc[1], ':');
        str += 1;
        port_number = atoi(str);    /*---- Changes the number past the ':' to integers  ----*/
    } else {
        printf("Port number not set");
    }
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); /*---- generates the socket ----*/
    serverAddr.sin_family = AF_INET;    /*---- Specifies that we want to use TCP connection thingy ----*/
    serverAddr.sin_port = htons(port_number);   /*---- sets the port number ----*/
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");    /*---- sets the host to be local --> this is not set by the argument list above  ----*/
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  /*---- Set all bits of the padding field to 0  ----*/
    
    if(connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) <0) { /*---- Connect the socket to the server using the address struct  ----*/
        printf("Error in client connection");
        exit(1);
    }
    
    char str_long_arr[1000][20];
    int str_long_arr_index = 0;
    
    while(1) {  /*---- loop repeats the receive / send of the client ----*/
        
        printf("CLIENT: ");
        
        fgets(buffer, 1024, stdin); /*---- gets the origional user input ----*/
        
        char *token, *string, *tofree;
        
        tofree = string = strdup(buffer);
        
        // inner loop to run through string tokens
        while ((token = strsep(&string, ";")) != NULL) {
            
            int i;
            
            bzero(buffer, sizeof(buffer));
            
            //printf("token: %s\t length: %d\n", token, (int) strlen(token));

            
            for(i = 0; i < (int) strlen(token); i++)
                if(token[i] == '\n') {
                    token[i] = '\0';
                } else {
                    strcat(token, "\0");
                }
                
            strcpy(buffer, token);
            
            //printf("buffer 1: %s\t  length: %d\n", buffer, (int) strlen(buffer));
            
            int buff_size = strlen(buffer);     /* Encryption */
            
            /* gets the last characters in the history list and executes it */
            if(buffer[0] == '!' && buffer[1] == '!') {
                if(str_long_arr_index == 0) {
                    printf("Nothing in history.");
                    continue;
                } else  {
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, str_long_arr[str_long_arr_index - 1]);
                }
            }
            
            //printf("buffer after !!: %s\t  length: %d\n", buffer, (int) strlen(buffer));
            
            /* gets the Nth element in the history list and executes it */
            if(buffer[0] == '!' && buffer[1] > 47 && buffer[1] < 58) {
                int index = buffer[1] - 49;
                if(str_long_arr_index == 0) {
                    printf("Nothing in history.\n");
                    continue;
                } else if( str_long_arr_index < (buffer[1] - 48)) {
                    printf("No such command in history.\n");
                    continue;
                } else {
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, str_long_arr[index]);
                }
            }
            
            //printf("buffer after !N: %s\t  length: %d\n", buffer, (int) strlen(buffer));
            
            /*  */
            if(buffer[0] == 'H' && buffer[1] == 'i' && buffer[2] == 's' && buffer[3] == 't' && buffer[4] == 'o' && buffer[5] == 'r' && buffer[6] == 'y'){
                if(str_long_arr_index == 0) {
                    printf("No such command in history.\n");
                } else if (str_long_arr_index < 10 && str_long_arr_index > 0){
                    for(i = 0; i < str_long_arr_index; i++) {
                        printf("%s\n", str_long_arr[i]);
                    }
                } else {
                    int incr = str_long_arr_index - 10;
                    for(i = 0; i < 10; i++) {
                        printf("%s\n", str_long_arr[i + incr]);
                    }
                }
                strcpy(str_long_arr[str_long_arr_index], buffer); // copy buffer to the index of str_long_arr
                str_long_arr_index += 1; // This is where the new command will be entered into the History List.
                continue;
            }
            
            //printf("buffer after History: %s\t  length: %d\n", buffer, (int) strlen(buffer));
            
            if(buff_size > 1) {
                strcpy(str_long_arr[str_long_arr_index], buffer); // copy buffer to the index of str_long_arr
                str_long_arr_index += 1; // This is where the new command will be entered into the History List.
            }
            
            //printf("buff_size: %d\n", buff_size);
            //printf("buffer after add: %s\t  length: %d\n", buffer, (int) strlen(buffer));
            
            // encrypts the thingy n' stuff
            for(i = 0; i < buff_size - 1; i++){
                buffer[i] = buffer[i] + N_;
            }
            
            printf("arr[0]: %s\n", str_long_arr[0]);
            printf("arr[1]: %s\n", str_long_arr[1]);
            printf("arr[2]: %s\n", str_long_arr[2]);
            printf("arr[3]: %s\n", str_long_arr[3]);
            printf("arr[4]: %s\n", str_long_arr[4]);
            printf("arr[5]: %s\n", str_long_arr[5]);
            
            /* SENDS TO SERVER */
            send(clientSocket, buffer, sizeof(buffer), 0);
            
            // resets the encryption which is dumb and should change
            for(i = 0; i < buff_size - 1; i++){
                buffer[i] = buffer[i] - N_;
            }
            
            // using the information after sending quit to kill the child process for this specific client
            if(buffer[0] == 'q' && buffer[1] == 'u' && buffer[2] == 'i' && buffer[3] == 't'){
                return 0;
            }
            
            bzero(buffer, sizeof(buffer));
            
            char rec_buffer[1048576];   /*---- a buffer that is set empty to recieve the message from the client ----*/
            
            if(recv(clientSocket, rec_buffer, sizeof(rec_buffer), 0) < 0){  /*---- recieves the socket connection message and sets it to buffer ----*/
                perror("recv"); /*---- prints a standar output ----*/
                break;
            } else {
                printf("SERVER: \n%s\n", rec_buffer);
                bzero(rec_buffer, sizeof(rec_buffer));  /*---- zeros the buffer if successful ----*/
            }
            
            
        } // Inner while-loop

    }
    
    close(clientSocket);    /*---- closes the socket when client exits main loop ----*/
    return 0;
}
