#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define RING_BUFFER_SIZE 256

typedef struct {
    uint8_t storage[RING_BUFFER_SIZE];
    uint16_t head;     /* Write index */
    uint16_t tail;     /* Read index */
    uint16_t count;    /* Current number of bytes in buffer */
    bool overflow;     /* Skill: Flag for overflow handling */
} RingBuffer_t;

/* API Functions */
void RingBuffer_Init(RingBuffer_t *rb);
bool RingBuffer_Put(RingBuffer_t *rb, uint8_t data);
bool RingBuffer_Get(RingBuffer_t *rb, uint8_t *data);
bool RingBuffer_IsEmpty(const RingBuffer_t *rb);
bool RingBuffer_IsFull(const RingBuffer_t *rb);
void RingBuffer_ClearOverflow(RingBuffer_t *rb);

#endif
