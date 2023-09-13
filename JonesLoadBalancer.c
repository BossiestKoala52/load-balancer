/**
 * Program to simulate multiple users simultaneously requesting work (a "job")
 * to be carried by a load balancing server and returned to the user. Job is to
 * compute the square of a number.
 *
 * Completion time: 2hours
 *
 * @author Nicholas Jones, Khan, Acuna
 * @version 13Sep23
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "InstanceHost.h"
#include "LoadBalancer.h"

//struct for representing the load balancer
struct balancer{
    int batchSize, jobCount;
    pthread_mutex_t list_lock;
    struct job_node* batchHead;
};

//structure to track jobs as they are created. serves as list node.
struct job_node;

//forward declarations for (public) functions

/**
 * Initializes the load balancer. Takes batch size as parameter.
 */
balancer* balancer_create(int batch_size){
    balancer *balancer1 = (struct balancer*)malloc(sizeof(balancer));
    balancer1->batchSize = batch_size;
    balancer1->jobCount = 0;
    balancer1->batchHead = NULL;

    return balancer1;
}

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 */
void balancer_destroy(balancer** lb){
    if(lb[0]->batchHead != NULL){
        host_request_instance(host_create(), lb[0]->batchHead);
    }
    struct job_node* current = lb[0]->batchHead;
    while(current != NULL){
        struct job_node* next = current->next;
        free(current);
        current = next;
    }
    pthread_mutex_destroy(&lb[0]->list_lock);
}

/**
 * Adds a job to the load balancer. If enough jobs have been added to fill a
 * batch, will request a new instance from InstanceHost. When job is complete,
 * *data_return will be updated with the result.
 *
 * @param user_id the id of the user making the request.
 * @param data the data the user wants to process.
 * @param data_return a pointer to a location to store the result of processing.
 */
void balancer_add_job(balancer* lb, int user_id, int data, int* data_return){
    pthread_mutex_lock(&lb->list_lock);
    printf("LoadBalancer: Received new job from user #%d to process data=%d and store it at %p. \n", user_id, data, data_return);
    struct job_node* newJob = (struct job_node*)malloc(sizeof(struct job_node));
    newJob->user_id = user_id;
    newJob->data = data;
    newJob->data_result = data_return;
    newJob->next = NULL;
        if(lb->jobCount == 0){
            lb->batchHead = newJob;
        }
        else{
            struct job_node* current = lb->batchHead;
            while(current->next != NULL){
                current = current->next;
            }
            current->next = newJob;
        }
        lb->jobCount++;
    pthread_mutex_unlock(&lb->list_lock);
    if(lb->jobCount == lb->batchSize) {
        host *host1 = host_create();
        host_request_instance(host1, lb->batchHead);
        host_destroy(&host1);
    }
}

