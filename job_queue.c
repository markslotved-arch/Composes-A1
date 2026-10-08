#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>

#include "job_queue.h"



int job_queue_init(struct job_queue *job_queue, int capacity) {
  job_queue->head = 0;
  job_queue->tail = 0;
  job_queue->capacity = capacity;
  job_queue->queue = calloc(capacity, sizeof(struct entry*));
  pthread_mutex_init(&job_queue->mutex, NULL);

  job_queue->is_destroyed = 0;
  job_queue->is_full = 0;
  job_queue->is_empty = 0;

  return 0;
}


int job_queue_destroy(struct job_queue *job_queue) {
  bool is_destroyed = 1;
  for (int i=0; i<job_queue->tail; i++) {
        free(job_queue->queue[i]);
    }
  return 0;
}

// https://www.geeksforgeeks.org/c/queue-in-c/ Reference for queue implementation in C
int job_queue_push(struct job_queue *job_queue, void *data) {
  pthread_mutex_lock(&job_queue->mutex);
  
  if (job_queue->is_destroyed) {
    pthread_mutex_unlock(&job_queue->mutex);
    return -1;
  }

  while (job_queue->tail == job_queue->capacity)
    {
      pthread_cond_wait(&job_queue->mutex);
    }
    job_queue->queue[job_queue->tail] = data;
    job_queue->tail++;

    pthread_mutex_unlock(&job_queue->mutex);
  return 0;
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  pthread_mutex_lock(&job_queue->mutex);

  if (job_queue->is_destroyed) {
    pthread_mutex_unlock(&job_queue->mutex);
    return -1;
  }

    while (job_queue->head == job_queue->tail)
    {
      pthread_cond_wait(&job_queue->mutex);
    }
    *data = job_queue->queue[job_queue->head];
    job_queue->head++;

    pthread_mutex_unlock(&job_queue->mutex);
  return 0;
}

