/*
Week 2b Lab Activity - Synchronization Primitives
This file demonstrates various synchronization primitives and their differences
*/

#include <atomic>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <semaphore>
#include <thread>


// 1. LOCK vs MUTEX
/*
Lock (General Concept):
- A lock is a general synchronization primitive that prevents multiple threads
from accessing a resource simultaneously
- Can be implemented in various ways (mutex, semaphore, etc.)
- Basic concept of preventing concurrent access

Mutex (Mutual Exclusion):
- A specific implementation of a lock
- Ensures only one thread can access a resource at a time
- Has two states: locked and unlocked
- Thread that locks must be the one to unlock
*/

// Example 1: Lock vs Mutex
void lockVsMutexExample() {
  std::cout << "\n=== Lock vs Mutex Example ===\n";

  // Simple lock implementation using atomic
  std::atomic<bool> simpleLock(false);

  // Mutex implementation
  std::mutex mutex;

  // Function to demonstrate simple lock
  auto simpleLockFunction = [&simpleLock]() {
    while (simpleLock.exchange(true)) {
      // Busy waiting (spin lock)
    }
    std::cout << "Simple lock acquired\n";
    // Do work
    simpleLock.store(false);
  };

  // Function to demonstrate mutex
  auto mutexFunction = [&mutex]() {
    mutex.lock();
    std::cout << "Mutex acquired\n";
    // Do work
    mutex.unlock();
  };

  std::thread t1(simpleLockFunction);
  std::thread t2(simpleLockFunction);
  t1.join();
  t2.join();

  std::thread t3(mutexFunction);
  std::thread t4(mutexFunction);
  t3.join();
  t4.join();
}

// 2. BINARY vs COUNTING SEMAPHORES
/*
Binary Semaphore:
- Can only have two values: 0 and 1
- Similar to a mutex but doesn't require the same thread to release
- Used for signaling between threads

Counting Semaphore:
- Can have any non-negative integer value
- Allows multiple threads to access a resource
- Useful for managing a pool of resources
*/

// Example 2: Binary vs Counting Semaphores
void semaphoreExample() {
  std::cout << "\n=== Binary vs Counting Semaphore Example ===\n";

  // Binary Semaphore (using std::binary_semaphore)
  std::binary_semaphore binarySem(1);

  // Counting Semaphore (using std::counting_semaphore)
  std::counting_semaphore<3> countingSem(3); // Allows 3 concurrent accesses

  // Function to demonstrate binary semaphore
  auto binarySemFunction = [&binarySem](int id) {
    binarySem.acquire();
    std::cout << "Binary Semaphore: Thread " << id << " acquired\n";
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Binary Semaphore: Thread " << id << " released\n";
    binarySem.release();
  };

  // Function to demonstrate counting semaphore
  auto countingSemFunction = [&countingSem](int id) {
    countingSem.acquire();
    std::cout << "Counting Semaphore: Thread " << id << " acquired\n";
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Counting Semaphore: Thread " << id << " released\n";
    countingSem.release();
  };

  // Create threads for binary semaphore
  std::thread t1(binarySemFunction, 1);
  std::thread t2(binarySemFunction, 2);
  t1.join();
  t2.join();

  // Create threads for counting semaphore
  std::thread t3(countingSemFunction, 1);
  std::thread t4(countingSemFunction, 2);
  std::thread t5(countingSemFunction, 3);
  std::thread t6(countingSemFunction, 4);
  t3.join();
  t4.join();
  t5.join();
  t6.join();
}

int main() {
  std::cout << "Demonstrating different synchronization primitives\n";

  // Run examples
  lockVsMutexExample();
  semaphoreExample();

  return 0;
}

/*
Additional Concepts Explained:

3. pthread_mutex_t:
- POSIX thread mutex implementation
- Platform-specific implementation of mutex
- Used in C/C++ on Unix-like systems
- More low-level than std::mutex

4. Reentrant Lock:
- Allows the same thread to acquire the lock multiple times
- Prevents deadlocks when a thread needs to acquire the same lock multiple times
- Keeps track of lock count and owner

5. Spin Locks:
- Busy waiting implementation of a lock
- Thread actively checks if lock is available
- Useful for very short critical sections
- Can waste CPU cycles

6. Sleep Locks:
- When lock is not available, thread goes to sleep
- More efficient than spin locks for longer wait times
- Involves context switching
- std::mutex is an example of a sleep lock
*/