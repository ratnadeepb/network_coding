/*
 * Created on Tue Jan 07 2020:15:05:12
 * Created by Ratnadeep Bhattacharya
 */

#ifndef __GLUETHREAD_H__
#define __GLUETHREAD_H__

typedef struct _glthread {
        struct _glthread *prev;
        struct _glthread *next;
} glthread_t;

void
glthread_add_next(glthread_t *base_glthread, glthread_t *new_glthread);

void
glthread_add_before(glthread_t *base_glthread, glthread_t *new_glthread);

void
remove_glthread(glthread_t *glthread);

void
init_glthread(glthread_t *glthread);

void
glthread_add_last(glthread_t *base_glthread, glthread_t *new_glthread);

#define IS_GLTHREAD_LIST_EMPTY(glthreadptr)                                    \
        ((glthreadptr)->next == 0 && (glthreadptr)->prev == 0)

/* return a pointer to the parent structure given the glthread pointer */
#define GLTHREAD_TO_STRUCT(fn_name, structure_name, field_name)                \
        static inline structure_name *fn_name(glthread_t *glthreadptr)         \
        {                                                                      \
                return (structure_name *)((char *)(glthreadptr) -              \
                                          (char *)&((structure_name *)0)       \
                                              ->field_name);                   \
        }

/* delete safe loop */
#define BASE(glthreadptr) ((glthreadptr)->next)

#define ITERATE_GLTHREAD_BEGIN(glthreadptrstart, glthreadptr)                  \
        {                                                                      \
                glthread_t *_glthread_ptr = NULL;                              \
                glthreadptr = BASE(glthreadptrstart);                          \
                for (; glthreadptr != NULL; glthreadptr = _glthread_ptr) {     \
                        _glthread_ptr = (glthreadptr)->next;

#define ITERATE_GLTHREAD_END(glthreadptrstart, glthreadptr)                    \
        }                                                                      \
        }

#define offset(struct_name, fld_name) (long int)&(((struct_name *)0)->fld_name)

#define GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthreadptr, offset)                \
        (void *)((char *)(glthreadptr)-offset)

void
delete_glthread_list(glthread_t *base_glthread);

unsigned int
get_glthread_list_count(glthread_t *base_glthread);

void
glthread_priority_insert(glthread_t *base_glthread, glthread_t *glthread,
                         int (*cmp_fn)(void *, void *), int offset);

#if 0
void *
gl_thread_search(glthread_t *base_glthread,
                 void *(*thread_to_struct_fn)(glthread_t *), void *key,
                 int (*comparison_fn)(void *, void *s));
#endif

#endif