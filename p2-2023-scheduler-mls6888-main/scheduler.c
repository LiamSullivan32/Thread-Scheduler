#include "scheduler.h"

// TODO: Add your function definitions here.

// add FIFO queue with enqueue, dequeue, 
// manage time with variable
// blocking threads that can't run yet
// main: create threads for each line in input
//cpu_me: arrival time for first call, schedules for 1 time unit, return time after 1 tick, goes until time remaining is 0 (done by main), threads happen at same time (concurrency), lock around global time with global time variable in interface.h, make other thread wait until others are done, signal to other thread to start when other is done
// if need to wait, add to linked list, if cannot schedule, wait
// add thread in cpu_me to list too, when remaining time is 0 signal to run next, use condition variables for across threads

// for task 2, wait for all threads to arrive before doing anything (can do for task 1 too?)
// have current thread variable for task 1 as well


// when remaining time is 0, remove from queue, reduce # threads - 1, before unlocking, signal to head


// make struct for FIFO queue, enqueue and dequeue functions need created

// go through to find earliest arrival time 

// find function!

void init_fifo(fifo_queue_t *queue) {
  queue -> head = NULL;
  queue -> tail = NULL;
} 

int isEmpty(fifo_queue_t *queue) {         
  return (queue -> head == NULL);
}

void enqueue(fifo_queue_t *queue, int tid, float arrival_time, int remaining_time) {
  // add to tail
  fifo_queue_entry_t *new_entry = (fifo_queue_entry_t *)malloc(sizeof(fifo_queue_entry_t));
  new_entry -> tid = tid;
  new_entry -> arrival_time = arrival_time;
  new_entry -> remaining_time = remaining_time;
  new_entry -> next = NULL;
  if (isEmpty(queue)) {
    queue -> head = new_entry;
    queue -> tail = new_entry;
  }
  else {
    queue -> tail -> next = new_entry;
    queue -> tail = new_entry;
  }
}

void dequeue(fifo_queue_t *queue, int tid) {
  // find entry with specific tid to kick out (one currently being worked with) 
  fifo_queue_entry_t *prev = NULL;
  fifo_queue_entry_t *current = queue -> head;
  while (current -> tid != tid) {
    prev = current;
    current = current -> next;
  }
  if (prev != NULL) {
    prev -> next = current -> next;
    if (current == queue -> tail) {
      queue -> tail = prev;
    }
  }
  else {
    queue -> head = current -> next;
  }
  free(current);
}

int find(fifo_queue_t *queue, int tid) {
  fifo_queue_entry_t *current = queue->head;
  while (current != NULL) {
    if (current->tid == tid) {
      return 1;
    }
    current = current->next;
  }
  return 0;
}

fifo_queue_entry_t *find_fifo(fifo_queue_t *queue) {

  int earliest_arrival_time;
  
  fifo_queue_entry_t *shortest = queue -> head;
  earliest_arrival_time = shortest->arrival_time;
  fifo_queue_entry_t *current = queue->head;

  //printf("Pointer: %p ThreadID: %d\n", current, current -> tid);
  
  while (current != NULL) {
    //if the earliest arrival time has not been set, or current arrived before any other seen value, 
    if (earliest_arrival_time==current->arrival_time){

      if (current->tid<shortest->tid){

        shortest=current;
        earliest_arrival_time=current->arrival_time;

      }
    }
    else if (earliest_arrival_time>current->arrival_time) {

      shortest=current;
      earliest_arrival_time=current->arrival_time;
        //set the shortest arrival time to the current value
      }

    
    //printf("Pointer: %p ThreadID: %d\n", current, current -> tid);
    current = current->next;
    }



  return shortest;
}

fifo_queue_entry_t *find_srtf(fifo_queue_t *queue, int global_time) {

  int lowest_remaining_time = 999999;
  fifo_queue_entry_t *current = queue -> head;
  fifo_queue_entry_t *shortest = queue -> head;
  
  while (current != NULL) {
    if (global_time >= current -> arrival_time & lowest_remaining_time > current -> remaining_time) {
      shortest = current;
      lowest_remaining_time = current -> remaining_time;
    }
    else if (global_time >= current -> arrival_time & lowest_remaining_time == current -> remaining_time) {        // decrement remaining time in queue?
      if (current -> tid < shortest -> tid) {
        shortest = current;
        lowest_remaining_time = current -> remaining_time;
      }
    }
    current = current -> next;
  }
  //shortest -> remaining_time -= 1;
  //printf("Shortest: %p Remaining Time: %d", shortest, shortest -> remaining_time);
  return shortest;
}


fifo_queue_entry_t *find_min_arrival(fifo_queue_t *queue) {

  float min_arrival = 999999;
  fifo_queue_entry_t *current = queue -> head;
  fifo_queue_entry_t *shortest = queue -> head;
  
  while (current != NULL) {
    if (current -> arrival_time < min_arrival) {
      shortest = current;
      min_arrival = current -> arrival_time;
    }
    current = current -> next;
  }
  return shortest;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void init_io_1(io_queue_1_t *io_queue_1) {
  io_queue_1 -> head = NULL;
  io_queue_1 -> tail = NULL;
}

int isEmpty_io_1(io_queue_1_t *io_queue_1) {
  return (io_queue_1 -> head == NULL);
}

void enqueue_io_1(io_queue_1_t *io_queue_1, int tid, float current_time, int device_id) {
  // add to tail
  io_queue_entry_1_t *new_entry = (io_queue_entry_1_t *)malloc(sizeof(io_queue_entry_1_t));
  new_entry -> tid = tid;
  new_entry -> current_time = current_time;
  new_entry -> device_id = device_id;
  new_entry -> next = NULL;
  if (isEmpty_io_1(io_queue_1)) {
    io_queue_1 -> head = new_entry;
    io_queue_1 -> tail = new_entry;
  }
  else {
    io_queue_1 -> tail -> next = new_entry;
    io_queue_1 -> tail = new_entry;
  }
}

void dequeue_io_1(io_queue_1_t *io_queue_1, int tid) {
  // find entry with specific tid to kick out (one currently being worked with) 
  io_queue_entry_1_t *prev = NULL;
  io_queue_entry_1_t *current = io_queue_1 -> head;
  while (current -> tid != tid) {
    prev = current;
    current = current -> next;
  }
  if (prev != NULL) {
    prev -> next = current -> next;
    if (current == io_queue_1 -> tail) {
      io_queue_1 -> tail = prev;
    }
  }
  else {
    io_queue_1 -> head = current -> next;
  }
  free(current);
}


int find_io(io_queue_1_t *io_queue_1, int tid) {
  io_queue_entry_1_t *current = io_queue_1->head;
  while (current != NULL) {
    if (current->tid == tid) {
      return 1;
    }
    current = current->next;
  }
  return 0;
}


io_queue_entry_1_t *find_first(io_queue_1_t *io_queue_1) {

  int earliest_arrival_time;
  
  io_queue_entry_1_t *shortest = io_queue_1 -> head;
  earliest_arrival_time = shortest->current_time;
  io_queue_entry_1_t *current = io_queue_1->head;

  //printf("Pointer: %p ThreadID: %d\n", current, current -> tid);
  
  while (current != NULL) {
    //if the earliest arrival time has not been set, or current arrived before any other seen value, 
    if (earliest_arrival_time==current->current_time){

      if (current->tid<shortest->tid){

        shortest=current;
        earliest_arrival_time=current->current_time;

      }
    }
    else if (earliest_arrival_time>current->current_time) {

      shortest=current;
      earliest_arrival_time=current->current_time;
        //set the shortest arrival time to the current value
      }

    
    //printf("Pointer: %p ThreadID: %d\n", current, current -> tid);
    current = current->next;
    }

  return shortest;
  
}





// OTHER NOTES: ADD A LOT OF STUFF TO HEADER FILE TO BE USED ACROSS FILES