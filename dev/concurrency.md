---
title: Asynchronous & Concurrent Programming
author: Alexander Neville
date: 2024-02-26
image-meta: semaphore-signal.png
image-meta-credit: https://commons.wikimedia.org/wiki/File:Signal_Home_Semaphore_R_%26_G.svg
description: A guide to writing asynchronous and concurrent programs.
---

Typically, a program follows a single thread of execution. This is
described as _synchronous_ code execution, in which instructions are
executed sequentially upon the completion of the previous operation.
There are two common scenarios in which multiple threads of execution
can be of benefit:

- A program must wait for the completion of some external process, such
  as an IO operation or the response from an API request.

- An expensive, time-consuming computation needs processor time. In this
  case, the processing is not offloaded from the processor.

In the first case, the CPU is left idle while it waits for an external
procedure to occur. This idle time could be spent initiating another
operation in the absence of the result of the long-running operation,
instead of spinning and wasting processor cycles. This is the default
behaviour of JavaScript, which is sometimes referred to as
_asynchronous_ or _non-blocking_.

In the second case, non-blocking program flow is of no benefit as the
expensive operations are not offloaded from the processor. Computation
must be distributed across many processor cores which can do processing
_concurrently_ or in _parallel_.

# Asynchronous Programming

In this example, the `delay` function simulates a time-consuming
external process. Very little processor time is required to service the
function call.

```py
import time


def delay():
    time.sleep(1)


def main():
    for _ in range(20):
        delay()


if __name__ == "__main__":
    main()
```

Timing this program reveals that the apparent execution time is
dramatically longer than the combined user and kernel space CPU
utilisation.

```text
$ time python sync.py

real    0m20.024s
user    0m0.021s
sys     0m0.000s
$ time python async.py
```

This is a good indication that the running time of the program can be
improved with _multitasking_, starting the _threads_ and sending them to
the background, periodically servicing them with processor time as
required. In Python these threads are not concurrent; no two threads are
executed simultaneously on different cores. This is enforced by Python's
Global Interpreter Lock. The illusion of concurrent execution is created
by repeated context switching.

```py
import time
from threading import Thread


def delay():
    time.sleep(1)


def main():
    threads = []
    for _ in range(20):
        thread = Thread(target=delay)
        threads.append(thread)
        thread.start()
    for thread in threads:
        thread.join()


if __name__ == "__main__":
    main()
```

The invocation of this improved program has a similar CPU run time, but
much lower effective running time as the remote operations simulated by
the `delay` function happen simultaneously.

```text
$ time python async.py

real    0m1.029s
user    0m0.022s
sys     0m0.007s
```

Python does support concurrent multithreading by creating multiple
python processes to service a CPU-intensive operation, using the
`Process` class from the `multiprocessing` module.

## JavaScript Callbacks & Promises

In the previous example, the `join` method of each `Thread` object is
called to ensure that the program only progresses once all threads have
completed. It is essential that the result of an asynchronous function
is available when it is used. One way to achieve this in JavaScript is
the use of _callback_ functions, which are passed as an argument to a
long-running function.

```js
function longFunction(callback) {
  callback(37);
}

longFunction((value) => {
  console.log(value);
});
```

Predictably, this program outputs `37`.

```
37
```

The value of the previous function is certain to be available in the
callback, unless an error or exception has occurred. Managing the
success and failure conditions for many nested callbacks can become very
difficult. _Promises_ admit two callback functions _resolve_ and
_reject_ to which code can subscribe with `.then` and `.catch` methods.

```js
function longFunction(condition) {
  return new Promise((resolve, reject) => {
    if (condition == true) {
      resolve("success");
    } else {
      reject("failure");
    }
  });
}

longFunction(true)
  .then((message) => {
    // success
    console.log(message);
  })
  .catch((message) => {
    // failiure
    console.log(message);
  });

longFunction(false)
  .then((message) => {
    // success
    console.log(message);
  })
  .catch((message) => {
    // failure
    console.log(message);
  });
```

This example outputs:

```text
success
failure
```

This pattern can be simplified further with the `async` and `await`
keywords. A promise can only be handled with `await` within a function
marked as `async`. If the promise _rejects_, the condition is handled by
the `catch` statement.

```js
async function main() {
  try {
    let message = await longFunction(true);
    console.log("resolve");
    console.log(message);
  } catch (message) {
    console.log("reject");
    console.log(message);
  } finally {
    console.log("finally");
  }
}

main();
```

This example outputs:

```text
resolve
success
finally
```

# POSIX Threads

The previous examples do little to accelerate a CPU-intensive operation.
In the case that a large volume of work must be completed on a machine
with multiple cores, parallel threads can be of benefit. Unlike
_multitasking_, concurrent multithreading does entail multiple threads
being executed simultaneously. A _thread_ is similar to a process.
Threads have their own context much like a process, but differ by
sharing the same address space with one or more other threads of
execution. Each thread has its own stack and data stored in this area of
memory is sometimes described as _thread-local_, illustrated in figure
[@fig:address_space].

<style>
figure {
max-width: calc(0.6*70ch);
}
</style>

![An Address Space with Multiple
Threads](/res/address_space.png){#fig:address_space}

# Critical Section

Individual threads can access heap-allocated memory. A _critical
section_ is an area of code in which a thread attempts to access a
shared resource. The danger of a critical sections is inducing a _race
condition_. A race condition occurs when two or more threads enter a
critical section to access a common piece of memory at the same time. If
data is shared amongst concurrent threads, access to it must be
carefully managed to avoid compromising its integrity. Data structures
and algorithms designed to cope with multiple threads are described as
_thread-safe_.

# Synchronisation Primitives

Used to prevent these harmful race conditions are a set of low-level
mechanisms called _synchronisation primitives_. Examples include _mutex
locks_, _condition variables_ and _semaphores_. These are often used to
deliver mutual-exclusion or _atomicity_, guaranteeing that a series of
instructions are executed sequentially, without interruption. The
improper use of these synchronisation techniques can lead to equally
sinister problems, ranging from delayed code execution to _deadlock_ and
_starvation_.

# Race Condition

This program demonstrates a typical race condition. The program is
designed to increment the `counter` variable two million times. It
creates two concurrent threads.

```c
#include <pthread.h>
#include <stdio.h>

static int counter = 0;

void *inc_counter() {
    for (int i = 0; i < 1000000; i++)
        counter++;
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, inc_counter, NULL);
    pthread_create(&t2, NULL, inc_counter, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("%d\n", counter);
    return 0;
}
```

The program should output $2000000$, although it always produces a
slightly different, always smaller number. This situation is caused by
the race condition in the code. The offending line is `counter++`. While
this appears to be a single operation, it is in fact three. The assembly
language below illustrates this.

```{.asm .gnuassembler .s}
mov rcx, [address]
inc rcx
mov [address], rcx
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

| Thread 1                    | Thread 2                    | Counter |
| --------------------------- | --------------------------- | ------: |
| Read 37 from memory         |                             |      37 |
| Increment to 38 in register |                             |      37 |
| Interrupted                 |                             |      37 |
|                             | Read 37 from memory         |      37 |
|                             | Increment to 38 in register |      37 |
|                             | Write 38 to memory          |  **38** |
| Resumed                     |                             |      38 |
| Write 38 to memory          |                             |  **38** |

:::

In this example, the value of the counter is only updated once, despite
the fact that two threads have attempted to incremented its value.

# Atomicity

The trace table below illustrates the desired behaviour of the two
threads.

:::{.wrap-table}

| Thread 1                    | Thread 2                    | Counter |
| --------------------------- | --------------------------- | ------: |
| Read 37 from memory         |                             |      37 |
| Increment to 38 in register | Waiting                     |      37 |
| Write 38 to memory          | Waiting                     |  **38** |
|                             | Read 38 from memory         |      38 |
|                             | Increment to 39 in register |      38 |
|                             | Write 39 to memory          |  **39** |

:::

The first advantage of this approach is the inability of the second
thread to enter the critical section. The critical section is also
executed atomically without interruption. The delay to thread 2 is an
example of thread _synchronisation_ , which maintains the state of the
shared resource. Here is an updated version of the program demonstrated
above, with the race condition addressed by the addition of a _mutex
lock_.

```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock; // +
static int counter = 0;

void *inc_counter() {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock); // +
        counter++;
        pthread_mutex_unlock(&lock); // +
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, inc_counter, NULL);
    pthread_create(&t2, NULL, inc_counter, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("%d\n", counter);
    return 0;
}
```

A mutex lock is used to manage access to the critical region that was
addressed previously. The term _mutex_ is short for _mutual exclusion_,
which prevents multiple threads accessing a region of code. The call to
_lock_ or _get_ the lock does on of two things:

- Obtain the lock if it is not held already, gaining exclusive access to
  the critical section.
- Spin, waiting for the lock to be free if it is already held.

Once the thread exits the critical section, the lock is _released_ or
_unlocked_. If this is not done at the appropriate time, it is possible
that other threads are permanently locked out or _deadlock_ occurs.

# Semaphores

The semaphore is another example of a synchronisation primitive.
Semaphores serve the dual purpose of delivering atomicity along with the
ability to communicate between and synchronise threads. These two roles
are otherwise achieved using a combination of locks and _condition
variables_. This is sometimes referred to as an ordering primitive and
lends the programmer greater control of the path threads take through
the program.

The semaphore is a single number, the value of which is shared amongst
threads and used to send signals to one another. The value that the
semaphore is initially set to dictates its behaviour. Once initialised,
a semaphore is interacted with through two main methods: `sem_post(3)` &
`sem_wait(3)`.

When `sem_wait` is encountered, the value of the specified semaphore is
_decremented_. If this value is subsequently greater than or equal to 0,
the thread proceeds into the critical section. If the value after being
decremented is less than 0, the resource is considered busy and the
calling thread waits. The other main function is `sem_post`. Once a
thread has completed work in a critical section, it sends a signal to
other waiting threads by _incrementing_ the semaphore value. This may
restore the value to 0 in which case a waiting thread would be able to
run.

Here is an example of a semaphore used to order the execution of two
threads, the main program thread and one called `child_thread`,
demonstrating the proper initialisation of the semaphore. The main
thread should start the child thread and then wait until the thread has
completed. The main thread is _signalled_ to by the child. To achieve
this behaviour, the semaphore value is set to 0.

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

Consider the result of setting the value to 1. The main thread would be
able to progress past the condition, regardless of the child thread's
state. See the trace table below.

:::{.wrap-table}

| Main Thread               | Child Thread | Semaphore Value |
| ------------------------- | ------------ | --------------: |
| Wait on semaphore         |              |               1 |
| Decrement semaphore value |              |           **0** |
| Sempahore value is zero   |              |               0 |
| Proceeed                  |              |               0 |
| Exit                      |              |               0 |

:::

In order to delay the main thread's exit until the child thread has run,
the semaphore should be given an initial value of 0. Decrementing the
semaphore value will yield a negative value, and the main thread will be
unable to progress. The child thread restores the semaphore value to
zero and unblocks the main thread.

:::{.wrap-table}

| Main Thread                 | Child Thread                 | Semaphore Value |
| --------------------------- | ---------------------------- | --------------: |
| Wait on semaphore           |                              |               0 |
| Decrement semaphore value   |                              |          **-1** |
| Sempahore value is negative |                              |              -1 |
| Sleep                       |                              |              -1 |
|                             | Child thread runs            |              -1 |
|                             | Child thread posts sempahore |              -1 |
|                             | Increment semaphore value    |           **0** |
| Main thread resumed         |                              |               0 |
| Exit                        |                              |               0 |

:::

It is possible that the child thread runs and increments the semaphore
before the main thread begins waiting. It may seem that the signal sent
by the child is not received, however a record of it is preserved in the
semaphore, ready for the main thread to read.

:::{.wrap-table}

| Main Thread               | Child Thread                 | Semaphore Value |
| ------------------------- | ---------------------------- | --------------: |
|                           | Child thread runs            |               0 |
|                           | Child thread posts sempahore |               0 |
|                           | Increment semaphore value    |           **1** |
| Wait on semaphore         |                              |               1 |
| Decrement semaphore value |                              |           **0** |
| Sempahore value is zero   |                              |               0 |
| Proceeed                  |                              |               0 |
| Exit                      |                              |               0 |

:::

With the correct semaphore initialisation, the desired behaviour is
achieved.

```text
parent entering
child entering
child exiting
parent exiting
```
