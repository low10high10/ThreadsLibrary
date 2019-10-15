#include "interrupt.h"
#include "thread.h"
#include <ucontext.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <utility>

using namespace std;


int thread_libinit(thread_startfunc_t func, void *arg);
int thread_create(thread_startfunc_t func, void *arg);
int thread_yield(void);
int thread_lock(unsigned int lock);
int thread_unlock(unsigned int lock);
int thread_wait(unsigned int lock, unsigned int cond);
int thread_signal(unsigned int lock, unsigned int cond);
int thread_broadcast(unsigned int lock, unsigned int cond);
int thread_exit();
struct TCB{
    ucontext* context;
    int ID;

};
map<pair<int, int>, queue<TCB> > condMap;
queue<TCB> ready_Queue; //Global variable for queue which contains all the thread's TCB which are ready to run
queue<TCB> zombie_Queue; //Global variable for queue which will contain all threads that have finished. Loop through this queue when ready_Queue is empty to clean up and delete all threads.
bool hasBeenInit = false; //Global boolean to check if thread_libinit has already been called
map<int, int> isLocked; // Key is lockNum and the value is int. If int is -1, that means lock is free. Otherwise, number is associated with TCB.id
    //True means that it is being held already, false means it is free
map<int, queue<TCB> > lockMap; //Map for each locks associated queue of blocked items.

//map<int, bool> isInit;

TCB current_thread; // Global Variable which is pointer to the TCB of the current thread running
TCB old_thread; // global variable which will be only updated correctly moments before swapcontext
int thread_counter =0;
map<pair<int, int>, bool> isInit;


// CODE GOES HERE
int start(thread_startfunc_t func, void *arg){
  //printf("hello\n");
  interrupt_enable();
  //printf("hello2\n");

  func(arg);
  interrupt_disable();
  //printf("to \n");
  thread_exit();

  //have to clean up now

  //swapcontext(initTCB->)
}
int thread_libinit(thread_startfunc_t func, void *arg){
  interrupt_disable();
  if(hasBeenInit){
    interrupt_enable();
    return -1; //Returns -1 due to error from user calling thread_libinit
  }
  thread_counter = 0;

  hasBeenInit = true;

  ucontext *libContext = new ucontext;
  TCB initTCB = {libContext,thread_counter}; //initializes initTCB with context pointer to libContext

  // Code taken from lab manual ***

  try{
    getcontext(initTCB.context);
    char *stack = new char[STACK_SIZE];

    initTCB.context->uc_stack.ss_sp = stack;
    initTCB.context->uc_stack.ss_size = STACK_SIZE;
    initTCB.context->uc_stack.ss_flags = 0;
    initTCB.context->uc_link = NULL;
  }
  catch(bad_alloc& ba){
    delete (char*) initTCB.context->uc_stack.ss_sp;
    delete initTCB.context;
    interrupt_enable();
    return -1;
  }

  makecontext(initTCB.context, (void(*)()) start, 2, func, arg);
  current_thread = initTCB;
  setcontext(current_thread.context);

  //cout << "got through end of libinit" << endl;

  //interrupt_enable();

}

//function called when program is completed/finished
int deleting(){

  while(!zombie_Queue.empty()){
    if(zombie_Queue.size()==1){
      cout << "Thread library exiting." << endl;
      interrupt_enable();
      exit(0);
    }
    //cout << "test." << endl;
    delete (char*) zombie_Queue.front().context->uc_stack.ss_sp;
    delete zombie_Queue.front().context;
    //delete zombie_Queue.front();
    //cout << "test2" << endl;
    zombie_Queue.pop();
  }



}

int thread_exit(){
//  cout << "thread has finished" << endl;
//  interrupt_disable();

  // Function should call thread_exit with interrupt_disable are set
  if(!ready_Queue.empty()){

    old_thread = current_thread;
    current_thread = ready_Queue.front();
    ready_Queue.pop();
    //current_thread = old_thread;
    zombie_Queue.push(old_thread);
    //cout << "Thread finished." << endl;

    swapcontext(old_thread.context,current_thread.context);
  }
  else{
    zombie_Queue.push(current_thread);
    deleting();
    return 0;

    //TO DO *** Have to do cleanup
  }
// interrupt_enable();
}





int thread_create(thread_startfunc_t func, void *arg){
  interrupt_disable();
  if(!hasBeenInit){
    //cout << "Havent called libinit yet!" << endl;
    interrupt_enable();
    return -1; //Returns -1 due to error from user not calling thread_libinit
  }
  thread_counter++;
  ucontext *newContext = new ucontext;
  TCB newTCB = {newContext,thread_counter};

  //cout << "ID of thread is " << thread_counter << endl;

  try{
    getcontext(newTCB.context);
    char *stack = new char[STACK_SIZE];
    newTCB.context->uc_stack.ss_sp = stack;
    newTCB.context->uc_stack.ss_size = STACK_SIZE;
    newTCB.context->uc_stack.ss_flags = 0;
    newTCB.context->uc_link = NULL;
  }
  catch(bad_alloc& ba){
    delete (char*) newTCB.context->uc_stack.ss_sp;
    delete newTCB.context;
    interrupt_enable();
    return -1;
  }


  makecontext(newTCB.context, (void(*)()) start, 2, func, arg);

  ready_Queue.push(newTCB);
  //cout << "push new thread into ready queue with size of: " << ready_Queue.size() << endl;

  interrupt_enable();
  return 0;
  //interrupt_enable();

}


int thread_yield(void){
  interrupt_disable();
  if(!hasBeenInit){
    //cout << "Havent called libinit yet!" << endl;
    interrupt_enable();
    return -1; //Returns -1 due to error from user not calling thread_libinit
  }

//  Save context of current thread
	// Put it in the back of the ready queue
	// Swap contexts with the thread in the front of the ready queue
	// pop the old thread off of the ready queue and start it running.

  //cout << "thread is trying to yield " << "Here's the size: "<< ready_Queue.size() << endl;
  if(!ready_Queue.empty()){
  	//cout << "swap threads" << endl;
    old_thread = current_thread;
    current_thread = ready_Queue.front();
    ready_Queue.push(old_thread);
    //current_thread = old_thread;
    ready_Queue.pop();


    swapcontext(old_thread.context,current_thread.context);
    interrupt_enable();
    return 0; // Why the return zero here?
    //cout << "actually getting through" << endl

  }
  else{
    //Thread yields when nothing is in ready_Queue, so it just continues on
  // cout << "Trying to yield, but it's the only thread 'alive' so nothing will happen, except this will print" << endl;
   interrupt_enable();
   return 0;
  }
}

int thread_lock(unsigned int lock){
  interrupt_disable();
  if(!hasBeenInit){
    //cout << "Havent called libinit yet!" << endl;
    interrupt_enable();
    return -1; //Returns -1 due to error from user not calling thread_libinit
  }
  //interrupt_disable(void);
  if(isLocked.count(lock)==0){ //If this is the first time using this lock, we must setup map to contain the lock key. We then set the lock to be busy as it is now held by the thread that called it.
    isLocked.insert(pair<int,int>(lock,current_thread.ID));
    //cout << "first time lock "<< lock << " is used" << endl;


  }
  else if(isLocked[lock]==-1){ //if lock is not held, the thread now holds the lock and continues.
    isLocked[lock]=current_thread.ID; //Lock is now being held by thread.
  //  cout << "lock "<< lock << " is free" << endl;
  }

  else if(isLocked[lock]==current_thread.ID){ //If lock is already held by current_thread
    //printf("holding same lock\n");
    interrupt_enable();
    return -1;
  }


  else{ //Lock is being held, put thread into lock's queue
  //  printf("lock is being held, PUT INTO LOCK Q \n");
    if(lockMap.count(lock)==0){ //If this is the first time a thread is added to this locks queue, we must initialize map.
      queue<TCB> lockQueue;
      lockMap.insert(pair<int,queue<TCB> >(lock,lockQueue));
    //  cout << "first time lockMap queue "<< lock << " is used" << endl;

    }
    lockMap[lock].push(current_thread); // Adds current_thread to lock's Queue.

  //  cout << "adding thread to Lock queue for lock "<< lock << endl;

    if(!ready_Queue.empty()){
      //old_thread = ready_Queue.front();//old thread will be from top of ready_Queue
      old_thread = current_thread;
      current_thread = ready_Queue.front();
      ready_Queue.pop(); //Update ready queue
      swapcontext(old_thread.context,current_thread.context);//Swap running thread to top of ready queue
      interrupt_enable();
      return 0;
    }
    else{ //If thread hits held lock and there is nothing in ready_Queue to switch into, this is a deadlock
      //cout << "deadlock" << endl;
      thread_exit();
      return 0;


    }
  }
  interrupt_enable();
  return 0;


  //interrupt_enable(void);
}

int lockInWait(unsigned int lock){
  //Comes into fxn with interrupts disabled and leaves disabled
  //interrupt_disable();
  if(!hasBeenInit){
    //cout << "Havent called libinit yet!" << endl;
    //interrupt_enable();
    return -1; //Returns -1 due to error from user not calling thread_libinit
  }
  //interrupt_disable(void);
  if(isLocked.count(lock)==0){ //If this is the first time using this lock, we must setup map to contain the lock key. We then set the lock to be busy as it is now held by the thread that called it.
    isLocked.insert(pair<int,int>(lock,current_thread.ID));
    //cout << "first time lock "<< lock << " is used" << endl;


  }
  else if(isLocked[lock]==-1){ //if lock is not held, the thread now holds the lock and continues.
    isLocked[lock]=current_thread.ID; //Lock is now being held by thread.
  //  cout << "lock "<< lock << " is free" << endl;
  }

  else if(isLocked[lock]==current_thread.ID){ //If lock is already held by current_thread
    //printf("holding same lock\n");
    //interrupt_enable();
    return -1;
  }


  else{ //Lock is being held, put thread into lock's queue
  //  printf("lock is being held, PUT INTO LOCK Q \n");
    if(lockMap.count(lock)==0){ //If this is the first time a thread is added to this locks queue, we must initialize map.
      queue<TCB> lockQueue;
      lockMap.insert(pair<int,queue<TCB> >(lock,lockQueue));
    //  cout << "first time lockMap queue "<< lock << " is used" << endl;

    }
    lockMap[lock].push(current_thread); // Adds current_thread to lock's Queue.

  //  cout << "adding thread to Lock queue for lock "<< lock << endl;

    if(!ready_Queue.empty()){
      //old_thread = ready_Queue.front();//old thread will be from top of ready_Queue
      old_thread = current_thread;
      current_thread = ready_Queue.front();
      ready_Queue.pop(); //Update ready queue
      swapcontext(old_thread.context,current_thread.context);//Swap running thread to top of ready queue
      //interrupt_enable();
      return 0;
    }
    else{ //If thread hits held lock and there is nothing in ready_Queue to switch into, this is a deadlock
      //cout << "deadlock" << endl;
      thread_exit();
      return 0;


    }
  }
  //interrupt_enable();
  return 0;


  //interrupt_enable(void);
}

int thread_unlock(unsigned int lock){
  interrupt_disable();
  if(!hasBeenInit){
    //cout << "Havent called libinit yet!" << endl;
    interrupt_enable();
    return -1; //Returns -1 due to error from user not calling thread_libinit
  }


  //printf("%d \n", isLocked.count(lock));
  if((isLocked.count(lock)==0)||(isLocked[lock] != current_thread.ID)){ //Thread cannot unlock lock it doesnt hold.
	//	cout << "This lock was not held, so it can't be freed" << endl;
  //  cout << "ID is " << current_thread.ID << endl;
    interrupt_enable();
    return -1;
	}

	if(isLocked.count(lock) != 0){
	//	cout << "lock is held, then released" << endl;
		if(lockMap[lock].empty()){ //Might need to change this. Not looking at if the map is empty, but if the queue corresponding to the lock is empty
	//		cout << "lock queue empty, lock is just freed" << endl;
			isLocked[lock] = -1; //Issue with this? Can't just update the map value?
		}
		else{
	//		cout << "There is something else in the lockQueue, needs to be an asynchronous (atomic) handoff of the lock" << endl;
			//ready_Queue.push(*Thing off the top of lockQueue*)
			//lockQueue.pop;
      isLocked[lock] = lockMap[lock].front().ID;
    //  cout << "ID getting THE LOCK NEXT IS " << lockMap[lock].front().ID  << endl;
			ready_Queue.push(lockMap[lock].front());
	//		cout << "Size of ready_Queue " << ready_Queue.size() << endl;
			lockMap[lock].pop();
		}
	}
  interrupt_enable();
  return 0;
	//cout << "lock " << lockMap.find(lock) << endl;
}

int unlockInWait(unsigned int lock){
  if(!hasBeenInit){
    //cout << "Havent called libinit yet!" << endl;
    //interrupt_enable(); NOT SURE ABOUT THIS. THIS TECHNICALLY ISN't POSSIBLE BC WAIT CANT CALL THIS FXN W/O LIBINT SET ALRDY
    return -1; //Returns -1 due to error from user not calling thread_libinit
  }


  //printf("%d \n", isLocked.count(lock));
  if((isLocked.count(lock)==0)||(isLocked[lock] != current_thread.ID)){ //Thread cannot unlock lock it doesnt hold.
	//	cout << "This lock was not held, so it can't be freed" << endl;
  //  cout << "ID is " << current_thread.ID << endl;
    //interrupt_enable();
    return -1;
	}

	if(isLocked.count(lock) != 0){
	//	cout << "lock is held, then released" << endl;
		if(lockMap[lock].empty()){ //Might need to change this. Not looking at if the map is empty, but if the queue corresponding to the lock is empty
	//		cout << "lock queue empty, lock is just freed" << endl;
			isLocked[lock] = -1; //Issue with this? Can't just update the map value?
		}
		else{
	//		cout << "There is something else in the lockQueue, needs to be an asynchronous (atomic) handoff of the lock" << endl;
			//ready_Queue.push(*Thing off the top of lockQueue*)
			//lockQueue.pop;
      isLocked[lock] = lockMap[lock].front().ID;
    //  cout << "ID getting THE LOCK NEXT IS " << lockMap[lock].front().ID  << endl;
			ready_Queue.push(lockMap[lock].front());
	//		cout << "Size of ready_Queue " << ready_Queue.size() << endl;
			lockMap[lock].pop();
		}
	}
  return 0;
	//cout << "lock " << lockMap.find(lock) << endl;
}


int thread_wait(unsigned int lock, unsigned int cond){ //NEED TO MAKE SURE WAIT ISNT CALLED ON LOCK IT DOESNT HOLD
  interrupt_disable();
  if(!hasBeenInit){
		//cout << "Havent called libinit yet!" << endl;
    interrupt_enable();
		return -1;
	}
  //Need to add new unlock FXN that will come in disabled and come out disabled
	if(unlockInWait(lock)){ //Without this, we assume the calling function holds the lock. No bueno...
    interrupt_enable();
    return -1;
  }
	queue<TCB> condQueue;
	if(isInit.count(make_pair(lock, cond)) == 0){	//If this is your first time seeing the tuple, add it to the map
	//	cout << "first time seeing this tuple" << endl;
		isInit.insert(make_pair(make_pair(lock, cond), true)); //This condition now exists, loop should only run once per unique tuple
		condMap.insert(make_pair(make_pair(lock, cond), condQueue));
	}

//	cout << "PUSHING into CV queue" << endl;
	condMap[make_pair(lock, cond)].push(current_thread);


//	cout << "Here's the number of threads waiting on a condition " << condMap[make_pair(lock, cond)].size() << endl;


	//Thread swapping part
	if(ready_Queue.empty()){
	//	cout << "This program is deadlocked, nothing will wake it up" << endl; //Test to check if it truly exits when deadlocked?
    thread_exit(); //Causing a seg fault?
	}
	else{
		old_thread = current_thread;
		current_thread = ready_Queue.front();
		ready_Queue.pop();
		swapcontext(old_thread.context, current_thread.context);
    //NEED TO ADD NEW LOCK FUNXTION that comes in disabled and comes out disabled
    lockInWait(lock); //Has to reaquire lock once it wakes back up...

    interrupt_enable();
    return 0;
		}
	//}
}
int thread_signal(unsigned int lock, unsigned int cond){
  interrupt_disable();

  if(!hasBeenInit){
		//cout << "Havent called libinit yet!" << endl;
    interrupt_enable();
		return -1;
	}

  if(condMap[make_pair(lock, cond)].size() == 0){
  //  cout << "No threads associated with this thread tuple" << endl;
  }
  else{
    ready_Queue.push(condMap[make_pair(lock, cond)].front());
  //  cout << "all good" << endl;
    condMap[make_pair(lock, cond)].pop();
  }
  interrupt_enable();
  return 0;

}
int thread_broadcast(unsigned int lock, unsigned int cond){
  interrupt_disable();
  if(!hasBeenInit){
		//cout << "Havent called libinit yet!" << endl;
    interrupt_enable();
		return -1;
	}

  if(condMap[make_pair(lock, cond)].size() == 0){
  //  cout << "No threads associated with this thread tuple" << endl;
  }
  else{
    while(condMap[make_pair(lock, cond)].size() > 0){
      ready_Queue.push(condMap[make_pair(lock, cond)].front());
  //    cout << "all good" << endl;
      condMap[make_pair(lock, cond)].pop();
    }
  }
  interrupt_enable();
  return 0;
}
