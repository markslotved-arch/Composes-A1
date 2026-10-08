#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>

#include "job_queue.h"

bool is_destroyed = 0;

int job_queue_init(struct job_queue *job_queue, int capacity) {
  job_queue->head = 0;
  job_queue->tail = 0;
  job_queue->total_capacity = capacity;
  job_queue->queue = calloc(capacity, sizeof(struct entry*));
  pthread_mutex_init(&job_queue->mutex, NULL);
}


int job_queue_destroy(struct job_queue *job_queue) {
  bool is_destroyed = 1;
  for (int i=0; i<job_queue->tail; i++) {
        free(job_queue->queue[i]);
    }
}

// https://www.geeksforgeeks.org/c/queue-in-c/ Reference for queue implementation in C
int job_queue_push(struct job_queue *job_queue, void *data) {
  if (is_destroyed) {
    return -1;
  }

  pthread_mutex_lock(&job_queue->mutex);

  if (job_queue->tail == job_queue->total_capacity)
    {
      pthread_mutex_join(&job_queue->mutex);
    }
    job_queue->queue[job_queue->tail] = data;
    job_queue->tail++;

    pthread_mutex_unlock(&job_queue->mutex);
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  if (is_destroyed) {
    return -1;
  }

  pthread_mutex_lock(&job_queue->mutex);

    if (job_queue->head == job_queue->tail)
    {
        
    }
    
    job_queue->head++;

    pthread_mutex_unlock(&job_queue->mutex);
}

