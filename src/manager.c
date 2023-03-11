//
// Created by Maxwell Babey on 10/30/22.
//

#include "../include/manager.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * mm_free_recurse
 * <p>
 * Recursively free memory in the linked list of memory addresses.
 * </p>
 * @param ma the memory address to free
 * @return the number of addresses freed
 */
static int mm_free_recurse(struct memory_address *ma);

/**
 * mm_find_in_list
 * <p>
 * Find the memory address struct holding mem in a memory manager.
 * </p>
 * @param mem_manager the memory manager in which to search
 * @param mem the memory for which to search
 * @return the memory address struct holding mem; NULL if not found
 */
static struct memory_address *mm_find_in_list(struct memory_manager *mem_manager, void *mem);

/**
 * struct memory_address
 * <p>
 * A node in the list of memory addresses.
 * </p>
 */
struct memory_address
{
    void                  *addr;
    struct memory_address *next;
};

struct memory_manager *init_mem_manager(void)
{
    struct memory_manager *mm;
    
    mm = (struct memory_manager *) malloc(sizeof(struct memory_manager));
    
    if (mm)
    {
        mm->head = NULL;
        
        mm->mm_add      = mm_add;
        mm->mm_free     = mm_free;
        mm->mm_free_all = mm_free_all;
    }
    
    return mm;
}

int free_mem_manager(struct memory_manager *mem_manager)
{
    if (!mem_manager)
    {
        return -1;
    }
    
    mem_manager->mm_free_all(mem_manager);
    free(mem_manager);
    
    return 0;
}

void *mm_add(struct memory_manager *mem_manager, void *mem)
{
    struct memory_address *ma     = NULL;
    struct memory_address *ma_cur = NULL;
    
    ma = (struct memory_address *) malloc(sizeof(struct memory_address));
    if (!ma)
    {
        return NULL;
    }
    
    ma->addr = mem;
    ma->next = NULL;
    
    if (mem_manager->head == NULL)
    {
        mem_manager->head = ma;
        return ma->addr;
    }
    
    ma_cur = mem_manager->head;
    while (ma_cur->next != NULL)
    {
        ma_cur = ma_cur->next;
    }
    ma_cur->next = ma;
    
    return ma->addr;
}


int mm_free(struct memory_manager *mem_manager, void *mem)
{
    struct memory_address *ma      = NULL;
    struct memory_address *ma_prev = NULL;
    
    ma = mem_manager->head;
    while (ma && ma->addr != mem) // Find ma in list, track ma_prev
    {
        ma_prev = ma;
        ma      = ma->next;
    }
    
    if (!ma) // If not found.
    {
        return -1;
    }
    
    if (ma == mem_manager->head) // Remove the memory from the manager.
    {
        mem_manager->head = ma->next;
    } else
    {
        ma_prev->next = ma->next;
    }
    
    free(ma->addr); // Free the memory.
    free(ma);
    
    return 0;
}

int mm_free_all(struct memory_manager *mem_manager)
{
    int m_freed;
    
    m_freed = mm_free_recurse(mem_manager->head);
    
    return m_freed;
}

static int mm_free_recurse(struct memory_address *ma)
{
    if (ma == NULL)
    {
        return 0;
    }
    
    int m_freed;
    
    m_freed = 1 + mm_free_recurse(ma->next);
    
    free(ma->addr);
    free(ma);
    
    return m_freed;
}

void *mm_malloc(size_t size, struct memory_manager *mem_manager,
                const char *file, const char *func, size_t line)
{
    void *mem = NULL;
    
    mem = malloc(size);
    if (!mem)
    {
        return NULL;
    }
    
    if (mem_manager)
    {
        mem_manager->mm_add(mem_manager, mem);
    }
    
    return mem;
}

void *mm_calloc(size_t count, size_t size, struct memory_manager *mem_manager,
                const char *file, const char *func, size_t line)
{
    void *mem = NULL;
    
    mem = calloc(count, size);
    if (!mem)
    {
        return NULL;
    }
    
    if (mem_manager)
    {
        mem_manager->mm_add(mem_manager, mem);
    }
    
    return mem;
}

void *mm_realloc(void *ptr, size_t size, struct memory_manager *mem_manager,
                 const char *file, const char *func, size_t line)
{
    struct memory_address *ma  = NULL;
    void                  *mem = NULL;
    
    mem = realloc(ptr, size);
    if (!mem)
    {
        return NULL;
    }
    
    // Update the pointer in the ma
    if (mem_manager)
    {
        ma = mm_find_in_list(mem_manager, ptr);
        if (ma != NULL)
        {
            ma->addr = mem;
        }
    }
    
    return mem;
}

static struct memory_address *mm_find_in_list(struct memory_manager *mem_manager, void *mem)
{
    struct memory_address *ma = NULL;
    
    ma = mem_manager->head;
    while (ma && ma->addr != mem)
    {
        ma = ma->next;
    }
    
    return ma;
}
