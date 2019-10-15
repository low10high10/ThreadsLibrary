#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

using namespace std;

//Test when lock is called on held lock and it must transition to ready queue item
int goodbye();
int goodbye2();
bool correctDetect = false;
// Have to fix this **
int hello(){
  thread_lock(0);
  thread_create((thread_startfunc_t)goodbye, (void*)0);
  thread_create((thread_startfunc_t)goodbye2, (void*)0);

  if(thread_yield()){
    cout << "Yield Failed" << endl;
  }

  thread_unlock(0);


  //thread_unlock(0);

  return 0;
}

int goodbye(){
  //cout << "GOODBYE" << endl;
  cout << "1" << endl;
  int x;
  if(x = thread_lock(0)){
    cout << "lock Failed" << endl;

  }
  cout << "2 " << x << endl;

}
int goodbye2(){
  //cout << "GOODBYE" << endl;
  cout << "3" << endl;


}

int main(int argc, char *argv[]) {

  thread_libinit((thread_startfunc_t)hello, (void*)0);

}
