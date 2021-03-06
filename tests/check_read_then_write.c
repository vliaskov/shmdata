/*
 * Copyright (C) 2012-2013 Nicolas Bouillot (http://www.nicolasbouillot.net)
 * Copyright (C) 2012 Alexandre Quessy
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 */

/**
 * @file Checks if it can send/receive ASCII data when a reader is started before the writer. This is 
 * accordingly testing internal file monitoring.
 * The main() function calls check_read_then_write() which starts a writer and a reader. When on_data is called, it checks if the string received matches what has been sent, and sets the value of the success variable accordingly.
 */

#include "shmdata/any-data-reader.h"
#include "shmdata/any-data-writer.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //sleep

// types:
typedef enum _bool
{
  no = 0,
  yes = 1
} Bool;

// constants:
static const Bool VERBOSE = yes;
static const char message[21] = "helloworldhelloworld";

// variables:
static Bool success = no;
static Bool keep_going;

// Function signatures:
static void data_not_required_anymore (void *priv);
static void on_data (shmdata_any_reader_t * reader, void *shmbuf, void *data,
		     int data_size, unsigned long long timestamp,
		     const char *type_description, void *user_data);
static int check_read_write ();

// Implementations:
static void 
data_not_required_anymore (void *priv)
{
  // here you can free your buffer
}

void
on_data (shmdata_any_reader_t * reader,
	 void *shmbuf,
	 void *data,
	 int data_size,
	 unsigned long long timestamp,
	 const char *type_description, void *user_data)
{
  if (VERBOSE == yes)
    {
      printf ("data %p, data size %d, timestamp %llu, type descr %s\n",
	      data, data_size, timestamp, type_description);
      printf ("user_data: %s\n", (const char *) user_data);
    }

  if (strcmp (data, message) == 0)
    {
      if (VERBOSE == yes)
	printf ("The two strings match! Success!\n");
      success = yes;
    }
  //free the data, can also be called later
  shmdata_any_reader_free (shmbuf);
  keep_going = no;
}

int
check_read_then_write ()
{
  shmdata_any_reader_t *reader;
  shmdata_any_writer_t *writer;

  const char *my_user_data =
    "You can pass a pointer to the reader's data handler function.";
  const char *SOCKET_PATH = "/tmp/shmdata-test-check-read-then-write";

  reader = shmdata_any_reader_init (); 
   if (VERBOSE == yes) 
     shmdata_any_reader_set_debug (reader, SHMDATA_ENABLE_DEBUG); 

   shmdata_any_reader_set_on_data_handler (reader, &on_data, 
   					  (void *) my_user_data); 
   shmdata_any_reader_set_data_type (reader, "text/plain"); 
   shmdata_any_reader_start (reader, SOCKET_PATH); 

   usleep (1000); 

   writer = shmdata_any_writer_init (); 
   if (VERBOSE == yes) 
     shmdata_any_writer_set_debug (writer, SHMDATA_ENABLE_DEBUG); 
   shmdata_any_writer_set_path (writer, SOCKET_PATH); 
   shmdata_any_writer_set_data_type (writer, "text/plain"); 
   shmdata_any_writer_start (writer); 

   unsigned long long myclock = 0; 
   unsigned long long nsecPeriod = 30000000; 

   keep_going = yes; 
   while (keep_going == yes) 
     { 
       //data should be serialized if network is involved 
       // here it is not 
       shmdata_any_writer_push_data (writer, 
   				    message, 
   				    sizeof (message), 
   				    myclock, &data_not_required_anymore, message); 
       usleep (nsecPeriod / 1000); 
       myclock += nsecPeriod; 
     } 
  
   shmdata_any_writer_close (writer); 
   shmdata_any_reader_close (reader); 

   if (success == yes) 
     return 0; 
   else 
     return 1; 
  
}

int
main (int argc, char *argv)
{
  if (check_read_then_write () != 0)
    return 1;
  return 0;
}

