#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;

#ifndef WINVER
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#endif

#define NUM_THREADS 50

struct ThreadData {
   int  threadId;
   string message;
};

class Printer {
	public:
	Printer(int threadId, const string & message) {
   		Sleep(rand()&4095);
   		cout << "Thread ID : " << threadId
		     << " Message : " << message << endl;
	}
};

void *PrintHello(void *threadArg) {
   struct ThreadData *threadData = (struct ThreadData *)threadArg;
     
   Printer(threadData->threadId, threadData->message);

   return NULL;
}

int main () {
   pthread_t threads[NUM_THREADS];
   struct ThreadData threadData[NUM_THREADS];
   srand(time(NULL));

   for(int i = 0; i < NUM_THREADS; i++ ) {
      cout <<"main(): Creating Thread: " << i << endl;
      threadData[i].threadId = i;
      threadData[i].message = "This is message";
      int rc = pthread_create(&threads[i], NULL, PrintHello, (void *)&threadData[i]);
      pthread_detach(threads[i]);
      if (rc) {
         cerr << "Error: Unable to Create Thread: " << rc << endl;
      }
   }

   Sleep(10000);

   return 0;
}
