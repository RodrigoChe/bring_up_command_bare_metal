/*
 * ring_buffer.c
 *
 *  Created on: Sep 24, 2025
 *      Author: rodrigo-che
 */

#include "ring_buffer.h"
#include <string.h>
#include <stdbool.h>

ReturnCode RingBufferInit(RingBuffer* rb) {
  if(rb == NULL) {
	  /* check your buffer parameter */
	  return kInvalidArgument;
  }

  rb->head = 0;
  rb->tail = 0;
  rb->items = 0;

  return kOk;
}

ReturnCode RingBufferIsFull(const RingBuffer* rb) {
  if(rb == NULL) {
	  /* check your buffer parameter */
	  return kInvalidArgument;
  }
  /*For overwrite method use this (rb->head == rb->tail) approach */
  if(rb->items >= (RING_BUFFER_SIZE - 1)) {
	  return kFull; // Buffer is full
  } else {
	  return kOk;
  }
}

ReturnCode RingBufferIsEmpty(const RingBuffer* rb) {
  if(rb == NULL) {
	  /* check your buffer parameter */
	  return kInvalidArgument;
  }

  /*For overwrite method use this approach */
  /*(((rb->head == rb->tail) || (rb->items == 0)) == true)*/
  if(rb->items <= 0) {
    return kEmpty;
  } else {
    return kOk;
  }
}

ReturnCode RingBufferPush(RingBuffer* rb, uint8_t data) {
  if(rb == NULL) {
	  /* check your buffer parameter */
	  return kInvalidArgument;
  }
  // Lock buffer
  // For Thread safety use this condition
  if (RingBufferIsFull(rb) == kFull) {
    return kFull;
  }

  rb->buffer[rb->head] = data;
  // Circular operator
  //rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
  //circular operator less overhead
  if((++rb->head) >= RING_BUFFER_SIZE) {
	  rb->head = 0;
  }

  rb->items++;

//  // Overwrite method! (No safety)
//  if (rb->items < RING_BUFFER_SIZE) {
//    rb->items++;
//  } else {
//    // Buffer is full, move read index forward (overwrite)
//    // THIS CONDITION IS NOT THREAD SAFE
//    //rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
//    if((++rb->tail) >= RING_BUFFER_SIZE) {
//  	  rb->tail = 0;
//    }
//  }

  return kOk;
}

ReturnCode RingBufferPop(RingBuffer* rb, uint8_t* data) {
  if((rb == NULL) || (data == NULL)) {
	  // check your buffer and data parameter
	  return kInvalidArgument;
  }

  if (RingBufferIsEmpty(rb) == kEmpty) {
    return kEmpty;  // Buffer is empty
  }

  *data = rb->buffer[rb->tail];

  //rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;

  if((++rb->tail) >= RING_BUFFER_SIZE) {
	  rb->tail = 0;
  }

  rb->items--;

  return kOk;
}

ReturnCode RingBufferFlush(RingBuffer* rb) {
  if(rb == NULL) {
	  // check your buffer parameter
	  return kInvalidArgument;
  }

  rb->items = 0;
  rb->head = 0;
  rb->tail = 0;

  memset(rb->buffer, 0, sizeof(rb->buffer));

  return kOk;
}

ReturnCode RingBufferWillFull(RingBuffer* rb, uint16_t new_items) {
  if(rb == NULL) {
	  // check your buffer parameter
	  return kInvalidArgument;
  }
  // For overwrite method just use the ring buffer size
  if (rb->items  + new_items > (RING_BUFFER_SIZE - 1)) {
	  return kFull;
  } else {
	  return kOk;
  }
}

ReturnCode RingBufferFreeItems(RingBuffer* rb, uint16_t* free_items) {
  if(rb == NULL) {
	  // check your buffer parameter
	  return kInvalidArgument;
  }

  *free_items = (RING_BUFFER_SIZE -1) - rb->items;

  return kOk;
}

ReturnCode RingBufferCurrentItems(RingBuffer* rb, uint16_t* num) {
  if(rb == NULL) {
	  // check your buffer parameter
	  return kInvalidArgument;
  }

  *num = rb->items;

  return kOk;
}

ReturnCode RingBufferCurrentSize(RingBuffer* rb, uint16_t* num){
  if(rb == NULL) {
	  // check your buffer parameter
	  return kInvalidArgument;
  }

  if(rb->head >= rb->tail) {
    *num = (rb->head - rb->tail);
  } else {
	*num = (RING_BUFFER_SIZE + rb->head - rb->tail); // (overwrote)
  }

  return kOk;
}

ReturnCode RingBufferStreamPush(RingBuffer* rb, uint8_t* data, uint16_t items) {
  if((rb == NULL) || (data == NULL) || (items >= RING_BUFFER_SIZE)) {
	  // check your buffer parameter
	  return kInvalidArgument;
  }

  if(RingBufferIsFull(rb)) {
    return kFull;
  }

  if(RingBufferWillFull(rb, items) == kFull) {
    return kFull;
  } else {
	for(uint16_t i = 0; i < items; i++) {
	  ReturnCode result = RingBufferPush(rb, data[i]);
	  if(result != kOk) {
	    return result;
	  }
	}
  }
  return kOk;
}

ReturnCode RingBufferStreamPop(RingBuffer* rb, uint8_t* data, uint16_t items) {
  if((rb == NULL) || (data == NULL) || (items <= 0)) {
    // check your buffer parameter
    return kInvalidArgument;
  }

  if(RingBufferIsEmpty(rb)) {
    return kEmpty;
  }

  for(uint16_t i = 0; i < items; i++) {
    ReturnCode result = RingBufferPop(rb, &data[i]);
    if(result != kOk) {
      return result;
    }
  }

  return kOk;
}
/*** end of file ***/
