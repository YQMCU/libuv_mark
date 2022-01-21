
/*
 * Copyright (C) Igor Sysoev
 */
// YQ_MARK: 此文档是nginx的队列结构实现
// 可参考 nginx源码分析—队列结构ngx_queue_t_学习，思考，记录，分享。-CSDN博客
// https://blog.csdn.net/livelylittlefish/article/details/6607324

#ifndef NGX_QUEUE_H_INCLUDED_
#define NGX_QUEUE_H_INCLUDED_


// YQ_MARK: 首先 ngx_queue_s 是一个具有头节点的双向循环链表的数据结构
typedef struct ngx_queue_s  ngx_queue_t;

struct ngx_queue_s {
    ngx_queue_t  *prev;
    ngx_queue_t  *next;
};


#define ngx_queue_init(q)                                                     \
  do {                                                                        \
    (q)->prev = q;                                                            \
    (q)->next = q;                                                            \
  }                                                                           \
  while (0)


#define ngx_queue_empty(h)                                                    \
    (h == (h)->prev)


#define ngx_queue_insert_head(h, x)                                           \
  do {                                                                        \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x;                                                            \
  }                                                                           \
  while (0)


#define ngx_queue_insert_after   ngx_queue_insert_head


#define ngx_queue_insert_tail(h, x)                                           \
  do {                                                                        \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x;                                                            \
  }                                                                           \
  while (0)


#define ngx_queue_head(h)                                                     \
    (h)->next


#define ngx_queue_last(h)                                                     \
    (h)->prev


#define ngx_queue_sentinel(h)                                                 \
    (h)


#define ngx_queue_next(q)                                                     \
    (q)->next


#define ngx_queue_prev(q)                                                     \
    (q)->prev


#if defined(NGX_DEBUG)

#define ngx_queue_remove(x)                                                   \
  do {                                                                        \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL;                                                         \
  }                                                                           \
  while (0)

#else

#define ngx_queue_remove(x)                                                   \
  do {                                                                        \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
  }                                                                           \
  while (0)

#endif


#define ngx_queue_split(h, q, n)                                              \
  do {                                                                        \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;                                                            \
  }                                                                           \
  while (0)


#define ngx_queue_add(h, n)                                                   \
  do {                                                                        \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;                                                      \
  }                                                                           \
  while (0)


#define ngx_queue_data(q, type, link)                                         \
    ((type *) ((unsigned char *) q - offsetof(type, link)))


#define ngx_queue_foreach(q, h)                                               \
    for ((q) = ngx_queue_head(h);                                             \
         (q) != ngx_queue_sentinel(h) && !ngx_queue_empty(h);                 \
         (q) = ngx_queue_next(q))


#endif /* NGX_QUEUE_H_INCLUDED_ */
