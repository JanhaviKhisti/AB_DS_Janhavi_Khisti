
#include "mv_dcll.h"

#ifndef		_MV_QUEUE_H
#define		_MV_QUEUE_H

//	Constant Literals

#ifndef		SUCCESS
#define		SUCCESS		0
#endif

#ifndef		FAILURE
#define		FAILURE		1
#endif

//	Type definitions

struct mv_queue;	//	struct declaration

typedef		struct mv_queue 		mv_queue_t;
typedef 	struct mv_queue* 		p_mv_queue_t;
typedef 	struct mv_queue**		pp_mv_queue_t;

typedef 	void*		data_t;
typedef 	data_t*		p_data_t;

#ifndef		SIZE_DATA
#define		SIZE_DATA	(sizeof(data_t))
#endif

typedef		int 		ret_t;

typedef		void (*PRINTDATAPROC) (data_t);
typedef		void (*DELETEDATAPROC) (data_t);

//	Struct Type Definitions

struct mv_queue
{
	p_mv_dcll_t pdcll;	

};

#define		SIZE_QUEUE		(sizeof(mv_queue_t))

//	Queue Interface Functions

extern	p_mv_queue_t 	mv_create_default_queue();

// extern	p_mv_queue_t	mv_create_custom_queue(size_t nr_of_elements);

extern	ret_t 	mv_queue_enqueue(p_mv_queue_t pqueue, data_t data);

extern	data_t 	mv_queue_dequeue(p_mv_queue_t pqueue);

extern	ret_t 	mv_queue_print_data(p_mv_queue_t pqueue, PRINTDATAPROC pprintfunc );

extern	size_t 	mv_queue_length(p_mv_queue_t pqueue);

extern	ret_t	mv_queue_destroy( pp_mv_queue_t, DELETEDATAPROC pdeletefunc );

#endif		/* _MV_QUEUE_H */