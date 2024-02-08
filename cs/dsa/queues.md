---
title: Queues
author: Alexander Neville
date: 2023-01-12
---

Unlike a [stack](./stacks.md), queue items are removed in the order they were
originally inserted, called a _First-In-First-Out (FIFO)_ or
_Last-In-Last-Out (LILO)_ data structure. Queues share a very similar
inductive definition to stacks, though their implementations differ.

- `emptyqueue() -> Q`
- `push(E,Q) -> Q`
- `isemptyqueue(S) -> T|F`
- `top(Q) -> E`
- `pop(Q) -> Q`

The role of `top` and `pop` are achieved through the mutator `dequeue`,
while `enqueue` performs an operation analogous to `push`, manipulating
an existing queue.

- `enqueue(E,Q)`
- `dequeue(Q) -> E`

# Implementation

For an efficient queue implementation, start and rear pointers must be
maintained. With these two references, items can be enqueued at either
the start or rear of the linked list in constant time. Items can only be
dequeued from the start of a linked list in constant time. To dequeue
from the rear, the rear pointer must be updated to point to the
penultimate (new rear) element. With a singly linked list, this requires
iteration from the start, $O(n)$ complexity. Therefore, the most
effective way to use a linked list to implement a queue is enqueue at
the rear and dequeue from the front, illustrated in figure
[@fig:queue_enqueue_dequeue].

![Queue operations, enqueue and
dequeue](../../res/queue_enqueue_dequeue.svg){#fig:queue_enqueue_dequeue}

A queue can be implemented as an array, with three additional variables:
`front`, `size` and `capacity`. So that the bounds of the array are not
exceeded, `front + size - 1 < capacity` must hold. As items are
dequeued, the front pointer is incremented and the number of available
slots decreases. It is possible that `front + size - 1` is equal to the
maximum capacity of the array, but most of the array is empty. The
simple solution to this problem is moving the occupied cells to the
beginning of the array, either when it is necessary or after each
dequeue operation. A slightly different implementation is preferable.

As successive enqueue and dequeue operations are conducted the occupied
portion of the queue shifts along the allocated space of the array. When
the rear element is at index `capacity -1`, adding an element to the
queue places it at index `0`, the queue wraps on the boundary. Now the
array only becomes full when the size of the queue is equal to the
capacity of the array. In a circular array, a queue occupies the
indices:

- `front, ..., front + size - 1` if `front + size - 1 < capacity`
- `front, ..., capacity - 1` and `0, .., front + size - capacity - 1` if
  `front + size > capacity`

In figure [@fig:circular_queue], a queue of size three occupies
different portions of the array. The front pointer is indicated with an
arrow.

![Queue implemented as a circular
array](../../res/circular_array.svg){#fig:circular_queue}

An example implementation of a queue with a circular array contains four
functions, two conditionals `isemptyqueue` and `isfullqueue`, a
constructor `enqueue` (emptyqueue is omitted here) and a selector
`dequeue`.

```{=html}
<div>
```

[Circular queue implementation]{.label}

```text
record E { ... };
record Q {
    int size;
    int capacity;
    E[] arr;
};
isemptyqueue(Q:q) -> T|F {
    return q.size == 0;
}
isfullqueue(Q:q) -> T|F {
    return q.size == q.capacity;
}
enqueue(E:e, Q:q) {
    if (isfullqueue(q)) THROW ERROR;
    q.arr[(q.front + q.size++) mod q.capacity] = e;
}
dequeue(Q:q) -> E:e {
    if (isemptyqueue(Q)) THROW ERROR;
    E e = q.arr[q.front];
    q.front = q.front + 1 mod q.capacity;
    q.size--;
    return e;
}
```
