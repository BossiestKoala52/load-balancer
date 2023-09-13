#include "LoadBalancer.h"
#include "InstanceHost.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//struct for representing the host
struct host{
    struct job_node* batch;
};

struct job_node; //defined in LoadBalancer

//forward declarations for (public) functions

/**
* Initializes the host environment.
*/
host* host_create(){
    host* host1 = (struct host*)malloc(sizeof(struct host));
    return host1;
}

/**
* Shuts down the host environment. Ensures any outstanding batches have
* completed.
*/
void host_destroy(host** host){

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
    free(current);

}

