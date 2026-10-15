// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2026 Lime Microsystems

#ifndef LIME_DMA_TCD_FIFO_H
#define LIME_DMA_TCD_FIFO_H

#include <stdint.h>
#include <stdbool.h>

#define TCD_FIFO_SIZE 32 // must be power of 2
#define TCD_FIFO_SIZE_MASK (TCD_FIFO_SIZE - 1)

typedef volatile struct DMA_TCD {
    uint32_t timestamp_lsb; // user facing time, samples count
    uint32_t timestamp_msb; // user facing time, samples count
    uint32_t la9310_mem_address;
    uint32_t size;
    uint32_t flags;
} dma_tcd_t;

typedef volatile struct TCD_FIFO {
    dma_tcd_t items[TCD_FIFO_SIZE];
    uint32_t head;
    uint32_t tail;
    uint32_t done;
} dma_tcd_fifo_t;

static inline void dma_tcd_reset(dma_tcd_t *tcd) {
    tcd->timestamp_lsb = 0;
    tcd->timestamp_msb = 0;
    tcd->la9310_mem_address = 0;
    tcd->size = 0;
    tcd->flags = 0;
}

static inline void tcd_fifo_reset(dma_tcd_fifo_t *fifo) {
    fifo->head = 0;
    fifo->tail = 0;
    fifo->done = 0;
}

static inline bool tcd_is_valid(const dma_tcd_t *tcd) {
    // using only the size as validity indicator
    return tcd->size > 0;
}

static inline void tcd_fifo_push(dma_tcd_fifo_t *fifo) { ++fifo->tail; }

static inline void tcd_fifo_pop(dma_tcd_fifo_t *fifo) { ++fifo->head; }

static inline dma_tcd_t *tcd_fifo_front(dma_tcd_fifo_t *fifo) { return &fifo->items[fifo->head & TCD_FIFO_SIZE_MASK]; }
static inline dma_tcd_t *tcd_fifo_back(dma_tcd_fifo_t *fifo) { return &fifo->items[fifo->tail & TCD_FIFO_SIZE_MASK]; }

static inline uint16_t tcd_fifo_size(const dma_tcd_fifo_t *fifo) { return fifo->tail - fifo->head; }

static inline bool tcd_fifo_isfull(const dma_tcd_fifo_t *fifo) { return tcd_fifo_size(fifo) == TCD_FIFO_SIZE; }

static inline bool tcd_fifo_isempty(const dma_tcd_fifo_t *fifo) { return tcd_fifo_size(fifo) == 0; }

#endif // LIME_DMA_TCD_FIFO_H
