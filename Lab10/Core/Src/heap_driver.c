#include "heap_driver.h"
#include <stdint.h>
#include <string.h>
#define HEAP_START_ADDR ((uint8_t*)0x20001000)
#define HEAP_SIZE       (4 * 1024)
#define BLOCK_SIZE      16
#define BLOCK_COUNT     (HEAP_SIZE / BLOCK_SIZE)

static uint8_t allocation_table[BLOCK_COUNT];

/**
 * @brief
 */
void heap_init(void) {
    memset(allocation_table, 0, sizeof(allocation_table));
    memset(HEAP_START_ADDR, 0, HEAP_SIZE);
}

/**
 * @brief
 */
void* heap_alloc(size_t size) {
    if (size == 0 || size > HEAP_SIZE) {
        return NULL;
    }

    size_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    size_t contiguous_free = 0;
    size_t start_index = 0;

    for (size_t i = 0; i < BLOCK_COUNT; i++) {
        if (allocation_table[i] == 0) {
            if (contiguous_free == 0) start_index = i;
            contiguous_free++;

            if (contiguous_free == blocks_needed) {
                allocation_table[start_index] = (uint8_t)blocks_needed;
                
                for (size_t j = 1; j < blocks_needed; j++) {
                    allocation_table[start_index + j] = 0xFF;
                }

                return (void*)(HEAP_START_ADDR + (start_index * BLOCK_SIZE));
            }
        } else {
            contiguous_free = 0;
        }
    }

    return NULL; 
}

/**
 * @brief
 */
void heap_free(void* ptr) {
    if (ptr == NULL) return;

    uint8_t* p = (uint8_t*)ptr;
    if (p < HEAP_START_ADDR || p >= (HEAP_START_ADDR + HEAP_SIZE)) {
        return;
    }

    size_t index = (p - HEAP_START_ADDR) / BLOCK_SIZE;

    uint8_t blocks_to_free = allocation_table[index];
    if (blocks_to_free == 0 || blocks_to_free == 0xFF) {
        return;
    }

    for (size_t i = 0; i < blocks_to_free; i++) {
        allocation_table[index + i] = 0;
    }
}
