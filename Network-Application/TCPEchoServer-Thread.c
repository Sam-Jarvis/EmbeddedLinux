#include <pthread.h>
#include <stdio.h>
#include <stdint.h>

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"

static void * myThread (void * arg);            /* thread that does the work */

int main (int argc, char *argv[])
{
    int         servSock;     /* Socket descriptor for server */
    intptr_t         clntSock;     /* Socket descriptor for client */
    pthread_t   threadID;     /* Thread ID from pthread_create() */
    bool        to_quit = false;
    


    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);


    while (!to_quit)                /* run until someone indicates to quit... */
    {
        clntSock = AcceptTCPConnection (servSock);

        // TODO: create&start the thread myThread() te creeeren
        // use the POSIX operation pthread_create()
        //
        // make sure that clntSock and servSock are closed at the correct locations 
        // (in particular: at those places where you don't need them any more)

        printf("Hello from thread main\n");

        if(pthread_create(&threadID, NULL, myThread, (void *) clntSock) != 0)
        {
            perror("Thread ID: \n");
            to_quit = false;
        }

    }
    
    // server stops...
	return (0);
}

static void *
myThread (void * threadArgs)
{
    // TODO: write the code to handle the client data
    // use operation HandleTCPClient()
    //  
    // Hint: use the info(), info_d(), info_s() operations to trace what happens
    //
    // Note: a call of pthread_detach() is obligatory

    if(pthread_detach(pthread_self()) != 0)
    {
        perror("Thread detach failed: ");
    }

    printf("Hello from thread %lx\n", pthread_self());

    HandleTCPClient((intptr_t) threadArgs);

    return (NULL);
}
