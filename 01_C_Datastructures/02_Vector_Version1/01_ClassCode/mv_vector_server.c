
// Headers
#include	<stdio.h>
#include	<stdlib.h>
#include	<assert.h>

#include 	"mv_vector.h"

// Function declaration
void error_checking( const int 	 assert_condition,
					 const char* assert_condition_str,
					 const char* file_name,
					 const int 	 line_no,
					 const int   error_condition,
					 const char* error_str);

// Macros
#define 	ERROR_CHECKING(	assert_condition, error_condition, error_str)	{											\
																				error_checking( assert_condition,		\
																								#assert_condition, 		\
																								__FILE__,				\
																								__LINE__,				\
																								error_condition,		\
																								error_str);				\
																			}	

// Vector Helper Functions 		( only for developer)
static void* Xmalloc(size_t nr_of_bytes)
{
	// Code

	void*  p = malloc(nr_of_bytes);

	ERROR_CHECKING( 1, NULL == p, "ERROR : Out Of Memory.\n" );

	return(p);
} 

static void* Xcalloc(size_t nr_of_elements, size_t nr_of_bytes)
{
	// Code

	void*  p = calloc(nr_of_elements, nr_of_bytes);

	ERROR_CHECKING( 1, NULL == p, "ERROR : Out Of Memory.\n" );

	return(p); 
}

static void* Xrealloc(void* p, size_t nr_of_bytes)
{
	// Code

	void* rp = realloc(p, nr_of_bytes);

	ERROR_CHECKING( 1, NULL == p, "ERROR : Out Of Memory.\n" );

	return(rp);
}


/*
  Function
  	identifier	:	error_checking
  	input		:	
  					1) assertion condition
  					2) assertion condition string
  					3) file name
  					4) line number
  					5) error checking
  					6) error string
*/
void  error_checking( const int   assert_condition,
					  const char* assert_condition_str,
					  const char* file_name,
					  const int   line_no,
					  const int   error_condition,
					  const char* error_str)
{
	// Code
	#ifdef DEBUG
		// Check Assertion
		if( !assert_condition )
		{
			fprintf(stderr, "Assertion failed : %s, File %s, Line No %d\n", assert_condition_str, file_name, line_no);
		}
	#else	
		// Error Checking
		if( error_condition )
		{
			if( NULL != error_str )
			{
				fprintf(stderr, error_str);
				exit(FAILURE);
			}
		}
	#endif
}		


// Vector Interface Functions			( given to user to use vector)

/*
  Function
  	identifier	:	create_vector
  	input		:	void
  	output		:	pointer to struct
*/
extern p_mv_vector_t  create_vector(void)
{
	// Code

	p_mv_vector_t  pvec = (p_mv_vector_t)Xmalloc(SIZE_MV_VECTOR);

	ERROR_CHECKING( NULL != pvec,
					1,
					NULL);

	pvec->capacity = 2;
	pvec->nr_of_elements = 0;
	pvec->array = (p_data_t)Xcalloc(pvec->capacity, SIZE_DATA);

	ERROR_CHECKING( NULL != pvec->array,
					1,
					NULL);

	return(pvec);
}


/*
  Function
	identifier	:	mv_vector_push_back
	input		:	1) vector
					2) data
	output		:	status(success or failure)
*/
extern ret_t mv_vector_push_back(p_mv_vector_t  pvector, data_t data)
{
	// Code

	ERROR_CHECKING( NULL != pvector,
					NULL == pvector,
					"ERROR : Vector Not Found.\n");

	// Check for vectors capacity
	if( pvector->nr_of_elements == pvector->capacity )
	{
		// Double the capacity
		pvector->capacity *= 2;

		pvector->array = (p_data_t)Xrealloc(pvector->array, pvector->capacity * SIZE_DATA);
		ERROR_CHECKING( NULL != pvector->array,
						1,
						NULL);
	}

	pvector->array[ pvector->nr_of_elements ] = data;
	pvector->nr_of_elements++;

	return(SUCCESS);
}	


/*
  Function
  	identifier	:	mv_vector_pop_back
  	input		:	vector
  	output		:	return popped data
*/
extern data_t mv_vector_pop_back(p_mv_vector_t  pvector)
{
	// Code

	ERROR_CHECKING( NULL != pvector,
					NULL == pvector ||
					0 == pvector->nr_of_elements,
					"ERROR : Vector Not Found.\n");

	// Remove the data
	pvector->nr_of_elements--;
	data_t to_return_data = pvector->array[ pvector->nr_of_elements ];

	// Reduce the capacity
	if( pvector->nr_of_elements == pvector->capacity / 2 )
	{
		pvector->capacity /= 2;
		pvector->array = (p_data_t)Xrealloc(pvector->array, pvector->capacity * SIZE_DATA);
		ERROR_CHECKING( NULL != pvector->array,
						1,
						NULL);
	}

	return(to_return_data);
}							


/*
  Function
  	identifier	:	mv_vector_data_at
  	input		:	1) vector
  					2) index
  	output		:	returns data at given index 
*/
extern data_t  mv_vector_data_at(p_mv_vector_t  pvector, size_t index)
{
	// Code

	ERROR_CHECKING( NULL != pvector,
					NULL == pvector ||
					0 == pvector->nr_of_elements,
					"ERROR : Vector Not Found.\n");

	ERROR_CHECKING( 1,
					index >= pvector->nr_of_elements,
					"ERROR : Index out of bound.\n");

	return(pvector->array[ index ]);
}								


/*
  Function
 	identifier	:	mv_vector_size
 	input		:	vector
 	output		:	size of vector
*/
extern size_t  mv_vector_size(p_mv_vector_t  pvector)
{
	// Code

	ERROR_CHECKING( NULL != pvector,
					NULL == pvector,
					"ERROR : Vector Not Found.\n");

	return(pvector->nr_of_elements);
}


/*
  Function
	identifier	:	mv_vector_capacity
	input		:	vector
	output		:	capacity of vector
*/
extern size_t  mv_vector_capacity(p_mv_vector_t  pvector)
{
	// Code

	ERROR_CHECKING( NULL != pvector,
					NULL == pvector,
					"ERROR : Vector Not Found.\n");

	return(pvector->capacity);
}


/*
  Function
 	identifier	:	mv_vector_destroy 
 	input		:	double pointer to vector
 	output		:	status(success or failure)
*/
extern ret_t  mv_vector_destroy(pp_mv_vector_t  ppvector)
{
	// Code
	ERROR_CHECKING( NULL != ppvector,
					NULL == ppvector,
					"ERROR : Vector Not Found.\n");

	p_mv_vector_t pvector = *ppvector;

	ERROR_CHECKING( NULL != pvector,
					NULL == pvector,
					"ERROR : Vector Not Found.\n");

	if( pvector->array )
	{
		free(pvector->array);
		pvector->array = NULL;
		pvector->nr_of_elements = 0;
		pvector->capacity = 0;
	}

	if( pvector )
	{
		free(pvector);
		pvector = NULL;
	}

	*ppvector = NULL;

	return(SUCCESS);
}


