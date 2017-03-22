/*---------------------------------------------------------
|
|   Name: Lab 4
|
|   Written By: Frank Liang 0560904 - March 2017
|
|   Purpose: To create a three signal handlers functions.  SIGINT, SIGQUIT, and SIGUSR1
|
|   Compile: gcc -o lab5 lab5.c
|
|   Usage: ./lab5
|
|   Description of Parameter:
|   cntl c: A counter is incremented
|   cntl \: Creates a child fork, sends a signal to the parent, and then gets killed
|   Any other key and it just does nothing at all
|
|   Source:
|   https://www.gnu.org/software/libc/manual/html_node/Kill-Example.html
|   http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/wait.html
|   Lecture notes were helpful as well
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include  <sys/wait.h>
#include  <string.h>
#define MAX 5   // Sets the max value to 
// i is the counter as cntl c is pressed
int i = 0, status;  // status is the value where the parent waits for the signal
void sig_usr( int signo );  //  the method of the signal user
/* handles two signals */
int main()
{
    if( signal( SIGINT,sig_usr ) == SIG_ERR )   // Checks if there is an error when pressing cntl c
        printf( "Cannot catch SIGINT\n" );
    if( signal( SIGQUIT,sig_usr ) == SIG_ERR )  /* Checks if there is an error when pressing cntl \*/
        printf("Cannot catch SIGQUIT\n");
    if( signal( SIGUSR1,sig_usr ) == SIG_ERR )  // Checks if there is an error when receiving the signal from the child
        printf("Cannot carch SIGUSR1\n");
    
    // Infinite loop while the program is waiting for the signal
    while(1)
    {
        printf("Wait for another signal ...\n");
        pause();
        /* pause until signal handler
        * has processed signal */
    }
    exit(0);
}
void sig_usr( int signo )   /* argument is signal number */
{
    pid_t childPID; // Creates a child pid when cntl \ is pressed
    // Checks to see if the user is pressing cntl c
    if( signo == SIGINT )
        // Limits the amount of times the user can press cntl c
        if (i < MAX)
        {
            i++;    // counter incremented when cntl c is pressed
            printf("This is the %d time you pressed cntl-c\n", i);
            if (i == MAX)   // Outputs the message to let the user know that they only have one attempt left
                printf("You have only one attempt left.\n");
        }
        // If the counter exceeds the limit, the program stops
        else
        {
            printf("Reached max attempt.  Have a good day.\n");
            exit(0);
        }
    // A child process is created where it sends the signal to the parent 
    else if( signo == SIGQUIT )
    {
        childPID = fork();  // A child pid is created using the fork functions
        if (childPID == 0)  // Checks if the there is only one child process created
        {
            printf("I am a child and I am sending a signal\n");
            kill(getppid(), SIGUSR1);   // The child presses terminates with no errors
            exit(0);
        }
        else
            wait(&status);  // The parent waits until a message is received from the child
    }
    // Once the message is received from the child, the entire program terminates with no errors
    // In other words, successful termination
    else if (signo == SIGUSR1)
    {
        puts("Child sent to signal so I guess you are bored, have a great day!\n");
        exit(0);
    }
    // Prints out an error if the signal is not received.
    else 
    {
        printf("Error: received signal %d\n",signo);
        exit(1); 
    }
}