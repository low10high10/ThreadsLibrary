#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

using namespace std;

/// This code was given to us from the lab manual

// Tests thread_libinit, thread_create, and thread_yield

int g=0;

void loop(void *a) {
  char *id;
  int i;

  id = (char *) a;
  cout <<"loop called with id " << (char *) id << endl;

  for (i=0; i<5; i++, g++) {
    cout << id << ":\t" << i << "\t" << g << endl;
    if (thread_yield()) {
      cout << "thread_yield failed\n";
      exit(1);
    }
  }
}

void parent(void *a) {
  int arg;
  arg = (long int) a;

  if(thread_libinit( (thread_startfunc_t) loop, (void *) 100) != -1);{
    cout << "ERROR: Tried to call thread_libinit twice" << endl;
  }

  cout << "parent called with arg " << arg << endl;
  if (thread_create((thread_startfunc_t) loop, (void *) "child thread")) {
    cout << "thread_create failed\n";
    exit(1);
  }

  loop( (void *) "parent thread");
}

int main() {
  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}
