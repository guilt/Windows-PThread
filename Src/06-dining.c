/*
 *	File	: dining.c
 *
 *	Title	: Dining Philosophers.
 *
 *	Short	: Solution to the dining philosophers problem.
 *
 *	Long	: Sets up a fifo fifo of threads, and activates threads
 *			as forks become avaliable.
 *
 *	Author	: Andrae Muys
 *
 *	Date	: 18 September 1997
 *
 *	Revised	:
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#pragma warning(disable: 4312)
#endif

#ifndef WINVER
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#ifndef EBUSY
#define EBUSY 16
#endif
#endif

#define TABLE 7
#define DELAY ((rand()%TABLE+1))
#define DISHES 3

typedef struct node {
	struct node *next;
	pthread_cond_t sleep;
} node;

typedef struct {
	node *head, *tail;
	pthread_mutex_t *mut;
	int runmax;
	int size;
} fifo;

fifo *fifoInit (int runmax);
void fifoDelete (fifo *q);
void fifoLock (fifo *q, pthread_mutex_t *mut, int id);
void fifoUnlock (fifo *q, int id);

void *philosopher (void *id);

pthread_mutex_t forks[TABLE];
pthread_cond_t newfork;
fifo *queue;

int main (int argc, char **argv)
{
	pthread_t p[TABLE];
	int i;
        srand(time(NULL));
	queue = fifoInit (TABLE / 2);
	printf ("Begin %d-diners\n", TABLE );
	pthread_cond_init (&newfork, NULL);

	for (i = 0; i < TABLE; i++)
		pthread_mutex_init (&(forks[i]), NULL);
	for (i = 0; i < TABLE; i++)
		pthread_create (&p[i], NULL, philosopher, (void *)i);
	for (i = 0; i < TABLE; i++)
		pthread_join (p[i], NULL);
	printf ("End %d-diners\n",TABLE/2);
	fifoDelete (queue);
	return 0;
}

void *philosopher (void *num)
{
	long long id;
	int i;
	pthread_mutex_t dummy = PTHREAD_MUTEX_INITIALIZER;

	id = (long long)num;
	pthread_mutex_lock (&dummy);
	printf ("P-%lld sit\n", id);
	for (i = 0; i < 2; i++) {
		fifoLock (queue, &dummy, id);
		Sleep (DELAY);
		fifoUnlock (queue, id);
	}
	for (i = 0; i < DISHES; i++) {
                printf ("P-%lld Start\n", id);
		while (1) {
			//printf ("P-%d Start%lld\n", id, i);
			fifoLock (queue, &dummy, id);
			if (pthread_mutex_trylock (&(forks[(id+1)%TABLE]))
			    == EBUSY) {
				//printf ("P-%lld reqfork%lld\n"
				//    , id, (id+1)%TABLE);
				fifoUnlock (queue, id);
				continue;
			}
			printf ("P-%lld gotfork%lld\n", id,
			    (id+1)%TABLE);
                        Sleep(DELAY);
			if (pthread_mutex_trylock (&(forks[id])) == EBUSY) {
				//printf ("P-%lld reqfork%lld\n"
				//    , id, id);
				pthread_mutex_unlock (&(forks[(id+1)%TABLE]));
				printf ("P-%lld dropfork%lld\n"
				    , id, (id+1)%TABLE);
				fifoUnlock (queue, id);
				continue;
			}
			printf ("P-%lld gotfork%lld\n", id, id);
			printf ("P-%lld eating\n", id);
			Sleep (DELAY * 3);
			pthread_mutex_unlock (&(forks[id]));
			pthread_mutex_unlock (&(forks[(id+1)%TABLE]));
			printf ("P-%lld retforks%lld-%lld\n", id, id, (id+1)%TABLE);
			fifoUnlock (queue, id);
			break;
		}
		printf ("P-%lld finished\n", id);
	}
	printf ("P-%lld finishedMEAL\n", id);
	pthread_mutex_unlock (&dummy);
	pthread_mutex_destroy (&dummy);

	return (NULL);
}

fifo *fifoInit (int runmax)
{
	fifo *q;

	q = (fifo *)malloc (sizeof (fifo));
	if (q == NULL) return (NULL);
	q->mut = (pthread_mutex_t *)malloc (sizeof (pthread_mutex_t));
	if (q->mut == NULL) { free (q); return (NULL); }
	pthread_mutex_init (q->mut, NULL);

	q->runmax = runmax;
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;

	return (q);
}

void fifoDelete (fifo *q)
{
	if (q->head != NULL) {
		//printf ("fifoDelete: Things that make you say mmmmm.\n");
		exit (1);
	}
	pthread_mutex_destroy (q->mut);
	free (q->mut);
	free (q);

	return;
}

void fifoLock (fifo *q, pthread_mutex_t *mut, int id)
{
	node *new;

	pthread_mutex_lock (q->mut);
	q->size++;
        //fprintf (stderr, "Lock %d : size = %d\n", id, q->size);
	if (q->size > q->runmax) {
		new = (node *)malloc (sizeof (node));
		if (new == NULL) {
			//printf ("fifoLock: malloc failed.\n");
			exit (1);
		}
		pthread_cond_init (&(new->sleep), NULL);
		new->next = NULL;
		if (q->tail == NULL) {
			q->head = q->tail = new;
		} else {
			q->tail->next = new;
			q->tail = new;
		}
		pthread_mutex_unlock (q->mut);
                //fprintf (stderr, "%d Blocked.\n", id);
		pthread_cond_wait (&(new->sleep), mut);
                //fprintf (stderr, "%d Woken.\n", id);
	} else {
		pthread_mutex_unlock (q->mut);
	}

	return;
}

void fifoUnlock (fifo *q, int id)
{
	node *old;

	pthread_mutex_lock (q->mut);
	q->size--;
        //fprintf (stderr, "Unlock : %d size = %d\n", id, q->size);
	if (q->head != NULL) {
		old = q->head;
		q->head = old->next;
		if (q->head == NULL) {
			q->tail = NULL;
		}
		old->next = NULL;
                //fprintf (stderr, "%d Waking head.\n", id);
		pthread_cond_signal (&(old->sleep));
	}
	pthread_mutex_unlock (q->mut);

	return;
}

