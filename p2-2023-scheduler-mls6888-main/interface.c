#include "interface.h"
#include "scheduler.h"

// Interface implementation

fifo_queue_t *queue;
int global_time;
int global_thread_count;
int threads_to_complete;
pthread_mutex_t lock;
pthread_cond_t cond;

io_queue_1_t *io_queue_1;
int global_io_count;
int global_io_time;
pthread_cond_t io_cond;

//Figure out how to make sure that we can lock other threads from using the same io device thats already being upsed by a thread.


void init_scheduler(int thread_count) {
  // TODO: Implement this
  // initialize list of size thread_count?
  queue = (fifo_queue_t *)malloc(sizeof(fifo_queue_t));
  init_fifo(queue);
  pthread_mutex_init(&lock, NULL);           // where to destroy? endme?
  pthread_cond_init(&cond, NULL);
  global_thread_count = thread_count;
  threads_to_complete = thread_count;
  global_time = 0;


  io_queue_1 = (io_queue_1_t *)malloc(sizeof(io_queue_1_t));
  init_io_1(io_queue_1);
  pthread_cond_init(&io_cond, NULL);


}

int cpu_me(float current_time, int tid, int remaining_time) {
  // TODO: Implement this
  // enqueue
  pthread_mutex_lock(&lock);
  
  if (find(queue, tid) == 0) {
    enqueue(queue, tid, current_time, remaining_time);
    global_thread_count -= 1;
  }
  
  while (global_thread_count != 0) {
    pthread_cond_wait(&cond, &lock);
  }
  
  
  
  
  
  // check if start time is 0 or not
  if (global_time == 0) {
    global_time = ceil(find_min_arrival(queue) -> arrival_time);        // does not work with consistency
  }
  
  // another while loop, set current to one passed in rn
  
  fifo_queue_entry_t *next;
  
  while (1) {
    next = find_srtf(queue, global_time);        // find_srtf(queue, global_time);           // find_fifo(queue);
    //printf("Next: %d",next -> arrival_time);
    if (next -> tid == tid) {
      break;
    }
    pthread_cond_signal(&cond);
    pthread_cond_wait(&cond, &lock);
  }
  
  //next -> remaining_time -= 1;               
  
  // find min arrival time in queue
  /*fifo_queue_entry_t *shortest;
  shortest=find_fifo(queue);

  int rounded;
  
  rounded=ceil(shortest->arrival_time);
  */
  // set global time based on initial arrival time, might not be 0
  /*if (global_time == 0 & next -> arrival_time != 0) {
    global_time = ceil(next -> arrival_time);                                   // not working every time
  } */
  
  //printf("Remaining Time: %d", next -> remaining_time);
  
  /*if (find_io(io_queue_1, tid) == 1) {
    //pthread_cond_wait(&cond, &lock);
    global_time += 2;
  }*/
  
  if (remaining_time != 0) {
    global_time += 1;
    //next -> remaining_time -= 1;
  }
  
  
  
  else {
    dequeue(queue, tid);
    global_thread_count += 1;            // not always +1, gets stuck in deadlock because of this
  }
  
  
  
  next -> remaining_time -= 1;
  //printf("Next remaining time: %d\n", next -> remaining_time); 
  
  // set current time to start time initially? If two threads have same arrival time, pick lowest tid
  // make find function to see if already enqueued
  // update remaining time after each loop in queue (task 2)
  
  //pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&lock);
  
  return global_time;
}

int io_me(float current_time, int tid, int device_id) {
  // TODO: Implement this
  
  // make separate queue for each device
  // only service one at a time per device, FCFS basis 
  pthread_mutex_lock(&lock);


  int io_complete;

  if (device_id == 1){

    io_complete=global_time + 2;
  }
  /*
  else{

    io_complete=global_time + 5;

  }*/
  enqueue_io_1(io_queue_1, tid, current_time, device_id);
  global_io_count+=1;//maybe get rid of it

  io_queue_entry_1_t *next;

  while (1) {
    next = find_first(io_queue_1);        // find_srtf(queue, global_time);           // find_fifo(queue);
    if (next -> tid == tid) {
      break;
    }
    //pthread_cond_signal(&io_cond);
    //pthread_cond_wait(&io_cond, &lock);
  }

  //next -> remaining_time = 0;
  /*if (global_time==io_complete){

  }*/

  /*if (find(queue, tid)) {
    global_time += 2;
  }*/
    //deadlock, we need to force the thread wait so that another thread can use cpu me, and this thread doesnt use cpu me


    //using 2 condition variables, signal the condition variable for cpu and wait on the conduituon variable for io me.
  
  

  dequeue_io_1(io_queue_1, tid);
  global_io_count-=1;

  
  pthread_mutex_unlock(&lock);
  
  return io_complete;
}

void end_me(int tid) {
  // TODO: Implement this
  // dequeue                        ??
  pthread_mutex_lock(&lock);
  global_thread_count -= 1;
  //dequeue(queue, tid);
  threads_to_complete -= 1;
  if (threads_to_complete == 0) {
    pthread_mutex_unlock(&lock);
    //pthread_mutex_destroy(&lock);          
    //pthread_cond_destroy(&cond);         // DESTROY ONCE ALL THREADS ARE DONE
  }
  else {
    pthread_cond_broadcast(&cond);  // need this?
    pthread_mutex_unlock(&lock);
  }
}
