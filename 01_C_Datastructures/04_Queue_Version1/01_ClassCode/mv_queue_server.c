
//	Headers

#include <stdio.h>
#include <stdlib.h>

#include "mv_queue.h"

//	Function Declaration

static ret_t error_checking(const int assert_condition,
							const char *assert_condition_str,
							const char *file_name,
							const long line_no,
							const int error_condition,
							const char *error_str,
							void *(*perror_handler)(void *),
							void *error_handler_params);

//	Constant Literal / MACROS

#define QUEUE_DEFAULT_SIZE 512

#define ERROR_CHECKING(assert_condition, error_condition, error_str, error_handler, error_handler_params) ( \
	error_checking(assert_condition,                                                                        \
				   #assert_condition,                                                                       \
				   __FILE__,                                                                                \
				   __LINE__,                                                                                \
				   error_condition,                                                                         \
				   error_str,                                                                               \
				   error_handler,                                                                           \
				   error_handler_params))

//	Queue Auxillary Functions
static void *Xmalloc(size_t nr_of_bytes)
{
	// Code
	void *p = malloc(nr_of_bytes);

	ret_t status = ERROR_CHECKING(1,
								  NULL == p,
								  "ERROR: Out of memory.\n",
								  NULL,
								  NULL);

	return (p);
}

static void *Xcalloc(size_t nr_of_elements, size_t size_of_element)
{
	// Code
	void *p = calloc(nr_of_elements, size_of_element);

	ret_t status = ERROR_CHECKING(1,
								  NULL == p,
								  "ERROR: Out Of Memory",
								  NULL,
								  NULL);

	return (p);
}

static ret_t error_checking(const int assert_condition,
							const char *assert_condition_str,
							const char *file_name,
							const long line_no,
							const int error_condition,
							const char *error_str,
							void *(*perror_handler)(void *),
							void *error_handler_params)
{
	//	Code
	ret_t status = SUCCESS;

#ifdef MV_DEBUG
	// Assertion
	if (!assert_condition)
	{
		fprintf(stderr,
				"Assertion Failed: %s, File %s, Line %ld.\n",
				assert_condition_str,
				file_name,
				line_no);
		status = FAILURE;
	}
#else
	// Error Checking
	if (error_condition)
	{
		fprintf(stderr, error_str);
		status = FAILURE;
	}
#endif

	if (FAILURE == status)
	{
		if (NULL != perror_handler)
		{
			perror_handler(error_handler_params);
		}
	}

	return (status);
}

void *deallocate_pqueue(void *param)
{
	//	Code
	pp_mv_queue_t ppqueue = (pp_mv_queue_t)param;
	p_mv_queue_t pqueue = *ppqueue;

	if (pqueue)
	{
		free(pqueue);
		pqueue = NULL;
	}

	*ppqueue = NULL;

	exit(FAILURE);

	return (NULL);
}

//	Queue Interface Functions

/*
  Fuction
  	identifier	:	create_default_queue
  	input		:	void
  	output		:	pointer to queue
*/
extern p_mv_queue_t create_default_queue(void)
{
	//	Code
	p_mv_queue_t pqueue = (p_mv_queue_t)Xmalloc(SIZE_QUEUE);

	ret_t status = ERROR_CHECKING(NULL != pqueue,
								  0,
								  NULL,
								  NULL,
								  NULL);
	
	// Allocate Array
	pqueue->queue = (p_data_t)Xcalloc(QUEUE_DEFAULT_SIZE, SIZE_DATA);

	status = ERROR_CHECKING(NULL != pqueue->queue,
							0,
							NULL,
							deallocate_pqueue,
							(void *)&pqueue);

	pqueue->head = 0;
	pqueue->tail = 0;
	pqueue->no_of_elements = 0;
	pqueue->length = QUEUE_DEFAULT_SIZE;

	return (pqueue);
	
}


/*
	no of elements = 3
	length = 10
	Queue
	[12	|13	|14	|111|	|	|	|	|	|	]
	0	1	2	3	4	5	6	7	8	9
	|				|
	HEAD			TAIL

	Queue
	[	|	|14	|111|112|113|14	|15	|16	|17	]
	0	1	2	3	4	5	6	7	8	9
	|		|
	TAIL	HEAD

	Enqueue <==> Push Back

*/


/*
  Function
  	identifier	:	create_custom_queue
  	input		:	number of elements
  	output		:	pointer to queue
*/
extern p_mv_queue_t create_custom_queue(size_t nr_of_elements)
{
	//	Code
	p_mv_queue_t pqueue = (p_mv_queue_t)Xmalloc(SIZE_QUEUE);

	ret_t status = ERROR_CHECKING(NULL != pqueue,
								  1,
								  NULL,
								  NULL,
								  NULL);

	// Allocate Array
	pqueue->queue = (p_data_t)Xcalloc(nr_of_elements, SIZE_DATA);

	status = ERROR_CHECKING(NULL != pqueue->queue,
							1,
							NULL,
							deallocate_pqueue,
							(void *)&pqueue);

	pqueue->head = 0;
	pqueue->tail = 0;
	pqueue->no_of_elements = 0;
	pqueue->length = nr_of_elements;

	return (pqueue);

}


/*
 Function
  	identifier	:	mv_queue_enqueue
  	input		:	1) queue
  					2) data
  	output		:	status(success or failure)
*/
extern ret_t mv_queue_enqueue(p_mv_queue_t pqueue, data_t data)
{
	//	Code
	ret_t status = SUCCESS;
	status = ERROR_CHECKING(NULL != pqueue,
							NULL == pqueue,
							"ERROR: Queue not found.\n",
							NULL,
							NULL);

	if (FAILURE == status)
		return (status);

	status = ERROR_CHECKING(NULL != pqueue->queue,
							NULL == pqueue->queue,
							"ERROR: Queue not found.\n",
							deallocate_pqueue,
							(void *)&pqueue);

	if (FAILURE == status)
		return (status);

	status = ERROR_CHECKING(pqueue->no_of_elements != pqueue->length,
							pqueue->no_of_elements == pqueue->length,
							"ERROR: Queue is full.\n",
							NULL,
							NULL);
	if (FAILURE == status)
		return (status);

	// Insert Data
	pqueue->queue[pqueue->tail] = data;
	pqueue->tail++;

	if (pqueue->tail == pqueue->length)
	{
		pqueue->tail = 0;
	}

	pqueue->no_of_elements++;

	return (SUCCESS);
}


/*
	Dequeue <==> PopFront
	Length = 10

	Queue
	[	|	|	|	|	|11	|12	|13	|14	|	]
	0	1	2	3	4	5	6	7	8	9
						|				|
						HEAD			TAIL

	Dequeue(Q);

	Queue
	[11	|12	|13	|14	|15	|	|	|	|	|	]
	0	1	2	3	4	5	6	7	8	9
	|					|
	HEAD				TAIL
	Dequeue(Q);
*/


/*
  Function 
  	identifier	:	mv_queue_dequeue
  	input		:	queue
  	output		:	returning popped data
*/
extern data_t mv_queue_dequeue(p_mv_queue_t pqueue)
{
	// Code
	ret_t status = SUCCESS;

	status = ERROR_CHECKING(NULL != pqueue,
							NULL == pqueue,
							"ERROR: Queue not found.\n",
							NULL,
							NULL);

	if (FAILURE == status)
		return (NULL);

	status = ERROR_CHECKING(NULL != pqueue->queue,
							NULL == pqueue->queue,
							"ERROR: Queue not found.\n",
							deallocate_pqueue,
							(void *)&pqueue);

	if (FAILURE == status)
		return (NULL);

	status = ERROR_CHECKING(0 != pqueue->no_of_elements,
							0 == pqueue->no_of_elements,
							"ERROR: No Data to delete",
							NULL,
							NULL);
	if (FAILURE == status)
		return (NULL);

	data_t to_return_data = pqueue->queue[pqueue->head];
	pqueue->head++;

	if (pqueue->head == pqueue->length)
	{
		pqueue->head = 0;
	}

	pqueue->no_of_elements--;

	return (to_return_data);
	
}


/*
  Function
  	identifier	:	mv_queue_print_data
  	input		:	1) queue
  					2) pointer to function 
  	output		:	status(success or failure)
*/
extern ret_t mv_queue_print_data(p_mv_queue_t pqueue, PRINTDATAPROC pprintfunc)
{
	
	// Code
	ret_t status = SUCCESS;
	status = ERROR_CHECKING(NULL != pqueue,
							NULL == pqueue,
							"ERROR: Queue not found.\n",
							NULL,
							NULL);

	if (FAILURE == status)
		return (status);

	status = ERROR_CHECKING(NULL != pqueue->queue,
							NULL == pqueue->queue,
							"ERROR: Queue not found.\n",
							deallocate_pqueue,
							(void *)&pqueue);

	if (FAILURE == status)
		return (status);

	fprintf(stdout, "\nQUEUE:\n");
	fprintf(stdout, "\n{START}-");
	for (long le = pqueue->head; le != pqueue->tail; ++le)
	{
		if (le == pqueue->length)
		{
			le = 0;
		}
		pprintfunc(pqueue->queue[le]);
	}
	fprintf(stdout, "{END}\n");

	return (status);
	
}


/*
  Function
  	identifier	:	mv_queue_length
  	input		:	queue
  	output		:	returning size of queue
*/
extern size_t mv_queue_length(p_mv_queue_t pqueue)
{
	// Code
	ret_t status = SUCCESS;
	status = ERROR_CHECKING(NULL != pqueue,
							NULL == pqueue,
							"ERROR: Queue not found.\n",
							NULL,
							NULL);

	if (FAILURE == status)
		return (status);

	status = ERROR_CHECKING(NULL != pqueue->queue,
							NULL == pqueue->queue,
							"ERROR: Queue not found.\n",
							deallocate_pqueue,
							(void *)&pqueue);

	if (FAILURE == status)
		return (status);

	return (pqueue->length);

}


/*
  Function
  	identifier	:	mv_queue_destroy
  	input		:	1) double pointer to queue
  	output		:	pointer to function
*/
extern ret_t mv_queue_destroy(pp_mv_queue_t ppqueue, DELETEDATAPROC pdeletefunc)
{
	//	Code
	ret_t status = SUCCESS;

	status = ERROR_CHECKING(NULL != ppqueue,
							NULL == ppqueue,
							"ERROR: Queue not found.\n",
							NULL,
							NULL);

	if (FAILURE == status)
		return (status);

	p_mv_queue_t pqueue = *ppqueue;

	status = ERROR_CHECKING(NULL != pqueue,
							NULL == pqueue,
							"ERROR: Queue not found.\n",
							NULL,
							NULL);

	if (FAILURE == status)
		return (status);

	status = ERROR_CHECKING(NULL != pqueue->queue,
							NULL == pqueue->queue,
							"ERROR: Queue not found.\n",
							deallocate_pqueue,
							(void *)&pqueue);

	if (FAILURE == status)
		return (status);

	fprintf(stdout, "\nDestroying QUEUE:\n");
	fprintf(stdout, "\n{START}\n");
	for (long le = pqueue->head; le != pqueue->tail; ++le)
	{
		if (le == pqueue->length)
		{
			le = 0;
		}
		pdeletefunc(pqueue->queue[le]);
	}
	fprintf(stdout, "{END}\n");

	if (pqueue->queue)
	{
		free(pqueue->queue);
		pqueue->queue = NULL;
	}

	if (pqueue)
	{
		free(pqueue);
		pqueue = NULL;
	}

	*ppqueue = NULL;

	return (status);
	
}

