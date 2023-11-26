---
title: Concurrency
author: Alexander Neville
date: 2023-11-18
---

# Critical Section

Individual threads can access heap allocated memory. A critical section
is an area of code in which a thread attempts to access a shared
resource . The danger of a critical sections is inducing a _race
condition_. A race condition occurs when two or more threads enter a
critical section to access a common piece of memory at the same time. If
data is shared amongst concurrent threads, access to it must be
carefully managed to avoid compromising its integrity. Data structures
and algorithms which are designed to cope with multiple threads are
described as _thread-safe_.

Used to prevent these harmful race conditions are a set of low-level
mechanisms called _synchronisation primitives_. Examples include _mutex
locks_, _condition variables_ and _semaphores_. These are often used to
deliver mutual-exclusion or _atomicity_, guaranteeing that a series of
instructions are executed sequentially, without interruption.

Frustratingly, the improper use of these synchronisation techniques can
lead to equally sinister problems, ranging from delayed code execution
to _deadlock_ and _starvation_.

## Race Condition

This pseudocode demonstrates a typical race condition. The program is
designed to increment the `shared_counter` variable 2 million times. It
creates two threads, which work concurrently.

```language-plaintext
GLOBAL INTEGER shared_counter <- 0

SUB inc_counter

    FOR (local_counter <- 0 TO 1000000 STEP 1)
        GLOBAL shared_counter += 1
    END FOR

END_SUB

SUB main

    OUTPUT shared_counter

    NEW THREAD t1 DOING inc_counter
    NEW THREAD t2 DOING inc_counter

    thread_start(t1)
    thread_start(t2)

    thread_join(t1)
    thread_join(t2)

    OUTPUT shared_counter

END SUB
```

The function `thread_start` is a stand-in for a language-specific
concrete function to start a thread. Similarly, the function
`thread_join` waits for a thread to finish working.

There is a problem with the algorithm shown above. Running the program
should return 2000000, although it always produces a slightly different,
smaller number. This situation is caused by the race condition in the
code. The offending line is `GLOBAL shared_counter +=1`. While this
appears to be one operation, it is in fact three. The assembly language
below illustrates this.

```language-plaintext
LDA rax MEM_ADDR
ADD rax, 1
STO MEM_ADDR, rax
```

Each thread in a program has its own state, which is managed by the
operating system. During execution, it is possible that a thread is
interrupted and its operation is suspended. If this occurs while the
thread is in the critical region, a synchronisation error can occur.
Below is a demonstration of the scenario that could arise in the above
program. Thread `t1` is interrupted while it is in the critical region,
leaving `t2` free to access the shared resource. When execution of `t1`
resumes, it overwrites the updated value.

:::{.wrap-table}

| Thread 1                    | Thread 2                    | Shared Counter |
| :-------------------------- | --------------------------- | -------------: |
| read 37 from memory         |                             |             37 |
| increment to 38 in register |                             |             37 |
| interrupted                 |                             |             37 |
|                             | read 37 from memory         |             37 |
|                             | increment to 38 in register |             37 |
|                             | write 38 to memory          |         **38** |
| resumed                     |                             |             38 |
| write 38 to memory          |                             |         **38** |

:::

In this example, the value of `shared_counter` is only updated once,
despite the fact that two threads have attempted to incremented its
value.

## Atomicity

The trace table below illustrates the desired behaviour of the two
threads.

:::{.wrap-table}

| Thread 1                    | Thread 2                    | Shared Counter |
| :-------------------------- | --------------------------- | -------------: |
| read 37 from memory         |                             |             37 |
| increment to 38 in register | waiting ...                 |             37 |
| write 38 to memory          | waiting ...                 |         **38** |
|                             | read 38 from memory         |             38 |
|                             | increment to 39 in register |             38 |
|                             | write 39 to memory          |         **39** |

:::

The first advantage of this approach is the inability of a second thread
to enter the critical section, while the critical section is also
executed atomically, without interruption. The delay to thread `t2` is
an example of synchronisation between threads, which maintains the state
of the shared resource. Here is an updated version of the program
demonstrated above, with the race condition addressed (new lines
prefixed with a `+` sign):

```language-plaintext
GLOBAL integer shared_counter <- 0

- GLOBAL mutex_lock lock

SUB inc_counter

    FOR (local_counter <- 0 TO 1000000 STEP 1)

-       get_lock(GLOBAL lock)
        GLOBAL shared_counter += 1
-       release_lock(GLOBAL lock)

  END FOR

END_SUB

SUB main

    OUTPUT shared_counter

    thread t1
    thread t2

- init_lock(lock)

  thread_start(t1, inc_counter, NONE) thread_start(t2, inc_counter,
  NONE)

  thread_join(t1, NONE) thread_join(t2, NONE)

  OUTPUT shared_counter

END SUB
```

A _mutex lock_ is used to manage access to the critical region that was
addressed previously. The term _mutex_ is short for mutual-exclusion,
which prevents multiple threads accessing a region of code.

The call to `get_lock()` does on of two things:

- Obtain the lock if it is not held already, getting exclusive access to
  the critical section.
- Spin, waiting for the lock to be free if it is already held.

Once the thread exits the critical section, the lock is released with a
call to `release_lock()`. If this is not done at the appropriate time,
it is possible that other threads are permanently locked out, or a
deadlock might occur.

## Semaphores

The semaphore is another example of a synchronisation primitive. They
serve the dual purpose of delivering atomicity, along with the ability
to communicate between threads. These two roles are otherwise achieved
using a combination of locks and condition variables. This is sometimes
referred to as an ordering primitive and lends the programmer greater
control of the path threads take through the program.

The semaphore is a single number, whose value is shared amongst threads
and used to send signals. The value the semaphore is initially set to
dictates its behaviour. Once initialised, a semaphore is interacted with
through two main methods: `sem_post` & `sem_wait`.

When `sem_post` is encountered, the value of the specified semaphore is
_decremented_. If this value is subsequently greater than or equal to
`0`, the thread proceeds into the critical section. If the value after
being decremented is less than `0`, the resource is considered busy and
the calling thread waits. The other main function is `sem_wait`. Once a
thread has completed work in a critical section it sends a signal to
other waiting threads by _incrementing_ the semaphore value. This may
restore the value to `0` in which case another thread is able to run.

Here is an example of a semaphore used to order the execution of two
threads, the main program thread and one called `child_thread`,
demonstrating the proper initialisation of the semaphore. The main
thread should start the child thread and then wait until the thread has
completed. The main thread is _signalled_ to by the child. To achieve
this behaviour, the semaphore value is set to `0`.

```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t semaphore;

void *child_func(void *arg) {
    printf("child entering\n");
    printf("child exiting\n");
    sem_post(&semaphore);
    return NULL;
}

int main(int argc, char *argv[]) {
    sem_init(&semaphore, 0, 0);
    printf("parent entering\n");
    pthread_t child_thread;
    pthread_create(&child_thread, NULL, child_func, NULL);
    sem_wait(&semaphore);
    printf("parent exiting\n");
    return 0;
}
```

Consider the outcome of setting the value to `1`. The main thread would
be able to progress past the condition, regardless of the child thread's
state. See the trace table below.

:::{.wrap-table}

| Main Thread                          | Child Thread | Semaphore Value |
| :----------------------------------- | :----------- | --------------: |
| wait on semaphore                    |              |               1 |
| decrement semaphore value            |              |               0 |
| sempahore value remains non-negative |              |               0 |
| proceeed                             |              |               0 |
| exit                                 |              |               0 |

:::

In order to delay the main thread's exit until the child thread has run,
the semaphore should be given an initial value of `0`. Decrementing this
will leave a negative value, and the main thread will be unable to
progress. The signal to continue is sent by the child thread, restoring
the semaphore to the zero value and unblocking the main thread.

:::{.wrap-table}

| Main Thread                 | Child Thread                 | Semaphore Value |
| :-------------------------- | :--------------------------- | --------------: |
| wait on semaphore           |                              |               0 |
| decrement semaphore value   |                              |              -1 |
| sempahore value is negative |                              |              -1 |
| sleep...                    |                              |              -1 |
|                             | child thread runs            |              -1 |
|                             | child thread posts sempahore |               0 |
| main thread resumed         |                              |               0 |
| program exits               |                              |               0 |

:::

It is possible that the child thread runs and increments the semaphore
before the main thread begins waiting. It may seem that the _signal_
sent by the child is not received, however a record of it is preserved
in the semaphore, ready for the main thread to read.

:::{.wrap-table}

| Main Thread                     | Child Thread                 | Semaphore Value |
| :------------------------------ | :--------------------------- | --------------: |
|                                 | child thread runs            |               0 |
|                                 | child thread posts sempahore |               1 |
| wait on semaphore               |                              |               1 |
| decrement semaphore value       |                              |               0 |
| sempahore value is non-negative |                              |               0 |
| program exits                   |                              |               0 |

:::

With the correct semaphore setup, the desired behaviour is achieved.

```language-plaintext
parent entering
child entering
child exiting
parent exiting
```

This was a basic example, demonstrating little other than thread
ordering with the bare minimum of threads. In the technical solution, I
have used multiple semaphores in a _hand-over-hand_ fashion to control
access to a shared data structure.
