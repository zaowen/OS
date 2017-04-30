#ifndef CUSTOMER_H
#define CUSTOMER_H


int request_resources ( int customer_num, int request[]);
void release_resources ( int customer_num, int release[]);

int can_finish( int *r, int *n);
void make_need( int n, int * need );
int safety_test( );
void print_state();
void alloc_req( int n , int *arr );
void free_req( int n , int  *arr );
int isqrt( int n );
 
void add_arr( int *a, int *b );
void * customer ( void * );

#endif 
