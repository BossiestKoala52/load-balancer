/**
 * Program to simulate multiple users simultaneously requesting work (a "job")
 * to be carried by a load balancing server and returned to the user. Job is to
 * compute the square of a number.
 *
 * Completion time: 1 hour
 *
 * @author Nicholas Jones, Khan, Acuna
 * @version 13Sep23
 */

#include "LoadBalancer.h"
#include "InstanceHost.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//struct for representing the host
struct host{
    struct job_node* batch;
    pthread_mutex_t host_lock;
};

struct job_node; //defined in LoadBalancer

//forward declarations for (public) functions

/**
* Initializes the host environment.
*/
host* host_create(){
    host* host1 = (struct host*)malloc(sizeof(struct host));
    host1->hostID++;
    return host1;
}

/**
* Shuts down the host environment. Ensures any outstanding batches have
* completed.
*/
void host_destroy(host** host){
    if(host[0]->batch != NULL){
        struct job_node* current = host[0]->batch;
        while(current != NULL){
            struct job_node* next = current->next;
            *current->data_result = (current->data * current->data);
            current = next;
        }
    }
    pthread_mutex_destroy(&host[0]->host_lock);
}

/**
* Creates a new server instance (i.e., thread) to handle processing the items
* contained in a batch (i.e., a listed list of job_node). InstanceHost will
* maintain a list of active instances, and if the host is requested to
* shutdown, ensures that all jobs are completed.
*
* @param job_batch_list A list containing the jobs in a batch to process.
*/
void host_request_instance(host* h, struct job_node* batch){
    printf("LoadBalancer: Received batch and spinning up new instance. \n");
    struct job_node* current = batch;
    while(current != NULL){
        struct job_node* next = current->next;
        *current->data_result = (current->data * current->data);
        //free(current);
        current = next;
    }
   // free(current);
   // host_destroy(& h);
}

