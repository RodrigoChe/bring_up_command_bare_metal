/*
 * ring_buffer.h
 *
 *  Created on: Sep 24, 2025
 *      Author: rodrigo-che
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Maximum buffer size for the circular buffer (valid items = buffer size - 1).
 *
 * @details We account for the space we can't use for thread safety. The buffer can
 * actually hold RING_BUFFER_SIZE - 1 elements to distinguish between full and empty states.
 */
#define RING_BUFFER_SIZE 4

/**
 * @brief Circular ring buffer structure.
 *
 * @details Implements a FIFO (First-In-First-Out) circular buffer with thread-safe
 * volatile members. The buffer uses the common technique of always keeping one
 * position empty to distinguish between full and empty states.
 */
typedef struct {
  uint8_t buffer[RING_BUFFER_SIZE]; ///< Storage array for buffer elements
  volatile uint16_t head;           ///< Write index (incremented on push operations)
  volatile uint16_t tail;           ///< Read index (incremented on pop operations)
  volatile uint16_t items;          ///< Current number of elements in the buffer
} RingBuffer;

/**
 * @brief Return codes for ring buffer operations.
 */
typedef enum {
    kOk = 0,               ///< Operation completed successfully
    kFull,                 ///< Buffer is full, operation cannot be performed
    kEmpty,                ///< Buffer is empty, operation cannot be performed
    kInvalidArgument,      ///< Invalid parameters passed to function
    kError                 ///< General error occurred
} ReturnCodes;

/**
 * @brief Initializes the ring buffer to empty state.
 *
 * @param rb Pointer to the RingBuffer instance to initialize.
 * @return kOk if initialization successful, kInvalidArgument if rb is NULL.
 */
ReturnCodes RingBufferInit(RingBuffer* rb);

/**
 * @brief Pushes a single byte into the buffer.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @param data The byte to push into the buffer.
 * @return kOk if push successful, kFull if buffer is full,
 *     kInvalidArgument if parameters invalid.
 */
ReturnCodes RingBufferPush(RingBuffer* rb, uint8_t data);

/**
 * @brief Pops a single byte from the buffer.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @param data Pointer to store the popped byte.
 * @return kOk if pop successful, kEmpty if buffer is empty,
 *     kInvalidArgument if parameters invalid.
 */
ReturnCodes RingBufferPop(RingBuffer* rb, uint8_t* data);

/**
 * @brief Checks if the buffer is empty.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @return kEmpty if buffer is empty, kOk if buffer
 *     contains data, kInvalidArgument if rb is NULL.
 */
ReturnCodes RingBufferIsEmpty(const RingBuffer* rb);

/**
 * @brief Checks if the buffer is full.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @return kFull if buffer is full, kOk if buffer  has space,
 *     kInvalidArgument if rb is NULL.
 */
ReturnCodes RingBufferIsFull(const RingBuffer* rb);

/**
 * @brief Flushes the buffer, removing all elements and resetting to empty state. *
 * @param rb Pointer to the RingBuffer instance.
 * @return kOk if flush successful, kInvalidArgument  if rb is NULL.
 */
ReturnCodes RingBufferFlush(RingBuffer* rb);

/**
 * @brief Checks if adding a specified number
 * of items would fill the buffer.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @param new_items Number of items to check for addition.
 * @return kOk if buffer would not be full, kFull if buffer would be full,
 *     kInvalidArgument if parameters invalid.
 */
ReturnCodes RingBufferWillFull(RingBuffer* rb, uint16_t new_items);

/**
 * @brief Retrieves the number of free items available in the buffer.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @param free_items Pointer to store the number of free items available.
 * @return kOk if operation successful, kInvalidArgument if parameters invalid.
 */
ReturnCodes RingBufferFreeItems(RingBuffer* rb, uint16_t* free_items);

/**
 * @brief Retrieves the current number of used items in the buffer.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @param num Pointer to store the number of used items.
 * @return kOk if operation successful, kInvalidArgument if parameters invalid.
 */
ReturnCodes RingBufferCurrentItems(RingBuffer* rb, uint16_t* num);

/**
 * @brief Retrieves the current allocated size of the buffer.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @param num Pointer to store the buffer size (always RING_BUFFER_SIZE - 1).
 * @return kOk if operation successful, kInvalidArgument if parameters invalid.
 */
ReturnCodes RingBufferCurrentSize(RingBuffer* rb, uint16_t* num);

/**
 * @brief Pushes a stream of data into the buffer.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @param data Pointer to the data stream to push into buffer.
 * @param items Number of items to push from the data stream.
 * @return kOk if all items pushed successfully, kFull if insufficient space,
 *         kInvalidArgument if parameters invalid.
 */
ReturnCodes RingBufferStreamPush(RingBuffer* rb, uint8_t* data, uint16_t items);

/**
 * @brief Pops a stream of data from the buffer.
 *
 * @param rb Pointer to the RingBuffer instance.
 * @param data Pointer to buffer where popped data will be stored.
 * @param items Number of items to pop from the buffer.
 * @return kOk if all items popped successfully, kEmpty if insufficient data,
 *         kInvalidArgument if parameters invalid.
 */
ReturnCodes RingBufferStreamPop(RingBuffer* rb, uint8_t* data, uint16_t items);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_H_ */
