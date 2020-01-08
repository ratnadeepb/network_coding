/*
 * Created on Tue Jan 07 2020:16:12:36
 * Created by Ratnadeep Bhattacharya
 */

#include "glthread.h"
#include <stdlib.h>

void
init_glthread(glthread_t *glthread)
{
        glthread->prev = NULL;
        glthread->next = NULL;
}

void
glthread_add_next(glthread_t *curr_glthread, glthread_t *new_glthread)
{
        if (!curr_glthread->next) {
                curr_glthread->next = new_glthread;
                new_glthread->prev = curr_glthread;
                return;
        }

        glthread_t *temp = curr_glthread->next;
        curr_glthread->next = new_glthread;
        new_glthread->prev = curr_glthread;
        new_glthread->next = temp;
        temp->prev = new_glthread;
}

void
glthread_add_before(glthread_t *curr_glthread, glthread_t *new_glthread)
{
        if (!curr_glthread->prev) {
                curr_glthread->prev = new_glthread;
                new_glthread->next = curr_glthread;
                return;
        }

        glthread_t *temp = curr_glthread->prev;
        curr_glthread->prev = new_glthread;
        new_glthread->next = curr_glthread;
        new_glthread->prev = temp;
        temp->next = new_glthread;
}

void
remove_glthread(glthread_t *curr_glthread)
{
        if (!curr_glthread->prev) {
                if (curr_glthread->next) {
                        curr_glthread->next->prev = NULL;
                        curr_glthread->next = NULL;
                        return;
                }
                return;
        }
        if (!curr_glthread->next) {
                curr_glthread->prev->next = NULL;
                curr_glthread->prev = NULL;
                return;
        }

        curr_glthread->prev->next = curr_glthread->next;
        curr_glthread->next->prev = curr_glthread->prev;
        curr_glthread->next = NULL;
        curr_glthread->prev = NULL;
}

void
delete_glthread_list(glthread_t *base_glthread)
{
        glthread_t *glthreadptr = NULL;
        ITERATE_GLTHREAD_BEGIN(base_glthread, glthreadptr)
        {
                remove_glthread(glthreadptr);
        }
        ITERATE_GLTHREAD_END(base_glthread, glthread_ptr);
}

void
glthread_add_last(glthread_t *base_glthread, glthread_t *new_glthread)
{
        glthread_t *glthreadptr = NULL, *prevglthreadptr = NULL;

        ITERATE_GLTHREAD_BEGIN(base_glthread, glthreadptr)
        {
                prevglthreadptr = glthreadptr;
        }
        ITERATE_GLTHREAD_END(base_glthread, glthreadptr);

        if (prevglthreadptr)
                glthread_add_next(prevglthreadptr, new_glthread);
        else
                glthread_add_next(base_glthread, new_glthread);
}

unsigned int
get_glthread_list_count(glthread_t *base_glthread)
{
        unsigned int count = 0;
        glthread_t *glthreadptr = NULL;

        ITERATE_GLTHREAD_BEGIN(base_glthread, glthreadptr) { count++; }
        ITERATE_GLTHREAD_END(base_glthread, glthreadptr);

        return count;
}

void
glthread_priority_insert(glthread_t *base_glthread, glthread_t *glthread,
                         int (*cmp_fn)(void *, void *), int offset)
{
        glthread_t *cur = NULL, *prev = NULL;
        init_glthread(glthread);

        if (IS_GLTHREAD_LIST_EMPTY(base_glthread)) {
                glthread_add_next(base_glthread, glthread);
                return;
        }

        /* only one node */
        if (base_glthread->next && !base_glthread->next->next) {
                if (cmp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(
                               base_glthread->next, offset),
                           GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread,
                                                              offset)) == -1)
                        glthread_add_next(base_glthread->next, glthread);
                else
                        glthread_add_next(base_glthread, glthread);
                return;
        }

        if (cmp_fn(
                GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread->next, offset),
                GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread->next,
                                                   offset)) == -1) {
                glthread_add_next(base_glthread->next, glthread);
                return;
        }

        ITERATE_GLTHREAD_BEGIN(base_glthread, cur)
        {
                if (cmp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread, offset),
                           GLTHREAD_GET_USER_DATA_FROM_OFFSET(cur, offset)) !=
                    -1) {
                        prev = cur;
                        continue;
                }
        }
        ITERATE_GLTHREAD_END(base_glthread, cur);

        glthread_add_next(prev, glthread);
}

#if 0
void *
gl_thread_search(glthread_t *base_glthread,
                 void *(*thread_to_struct_fn)(glthread_t *), void *key,
                 int (*comparison_fn)(void *, void *s))
{
        return NULL;
}
#endif