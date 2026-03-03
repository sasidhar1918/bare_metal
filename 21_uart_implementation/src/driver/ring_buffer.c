#include "ring_buffer.h"

void RingBuffer_Init(RingBuffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->overflow = false;
}

/**
 * @brief Adds a byte to the buffer.
 * @return true if successful, false if buffer was full (Overflow).
 */
bool RingBuffer_Put(RingBuffer_t *rb, uint8_t data) {
    if (rb->count >= RING_BUFFER_SIZE) {
        rb->overflow = true; // Set the error flag
        return false;        // Data lost
    }

    rb->storage[rb->head] = data;
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
    rb->count++;
    return true;
}

/**
 * @brief Retrieves a byte from the buffer.
 */
bool RingBuffer_Get(RingBuffer_t *rb, uint8_t *data) {
    if (rb->count == 0) {
        return false; // Buffer empty
    }

    *data = rb->storage[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
    rb->count--;
    return true;
}

bool RingBuffer_IsEmpty(const RingBuffer_t *rb) {
    return (rb->count == 0);
}

bool RingBuffer_IsFull(const RingBuffer_t *rb) {
    return (rb->count >= RING_BUFFER_SIZE);
}

void RingBuffer_ClearOverflow(RingBuffer_t *rb) {
    rb->overflow = false;
}
