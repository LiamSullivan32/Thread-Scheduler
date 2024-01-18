/*
 * Utilize "scheduler.h" and "scheduler.c" for all the utility functions students
 * intend to use for "interface.c".
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>

#include "interface.h"

typedef struct fifo_queue_entry
{
  int tid;
  float arrival_time;
  int remaining_time;
  struct fifo_queue_entry *next;
    
} fifo_queue_entry_t;

typedef struct fifo_queue
{
  fifo_queue_entry_t *head;
  fifo_queue_entry_t *tail;

} fifo_queue_t;

void init_fifo(fifo_queue_t *queue);

int isEmpty(fifo_queue_t *queue);

void enqueue(fifo_queue_t *queue, int tid, float arrival_time, int remaining_time);

void dequeue(fifo_queue_t *queue, int tid);

int find(fifo_queue_t *queue, int tid);

fifo_queue_entry_t *find_fifo(fifo_queue_t *queue);

fifo_queue_entry_t *find_srtf(fifo_queue_t *queue, int global_time);

fifo_queue_entry_t *find_min_arrival(fifo_queue_t *queue);



typedef struct io_queue_entry_1
{
  float current_time;
  int tid;
  int device_id;
  struct io_queue_entry_1 *next;
    
} io_queue_entry_1_t;

typedef struct io_queue_1
{
  io_queue_entry_1_t *head;
  io_queue_entry_1_t *tail;

} io_queue_1_t;

void init_io_1(io_queue_1_t *io_queue_1);

int isEmpty_io_1(io_queue_1_t *io_queue_1);

void enqueue_io_1(io_queue_1_t *io_queue_1, int tid, float current_time, int device_id);

void dequeue_io_1(io_queue_1_t *io_queue_1, int tid);

int find_io(io_queue_1_t *io_queue_1, int tid);

io_queue_entry_1_t *find_first(io_queue_1_t *io_queue_1);

#endif
