#include <pthread.h>
#include <stdio.h>

void *inc_x(void *x_void_ptr)
{

int *x_ptr = (int *)x_void_ptr;
while(++(*x_ptr) < 100);

printf("x increment finished\n");

return NULL;
}

int main()
{

int x = 0, y = 0;

/* show the initial values of x and y */
printf("x: %d, y: %d\n", x, y);

/* this variable is our reference to the second thread */
pthread_t inc_x_thread;

/* create a second thread which executes inc_x(&x) */
if(pthread_create(&inc_x_thread, NULL, inc_x, &x)) {

fprintf(stderr, "Error creating thread\n");
return 1;

}
/* increment y to 100 in the first thread */
while(++y < 100);

printf("y increment finished\n");

/* wait for the second thread to finish */
if(pthread_join(inc_x_thread, NULL)) {

fprintf(stderr, "Error joining thread\n");
return 2;

}

/* show the results - x is now 100 thanks to the second thread */
printf("x: %d, y: %d\n", x, y);

return 0;

}