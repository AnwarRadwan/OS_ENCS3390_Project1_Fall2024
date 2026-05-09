# Top 10 Frequent Words - Linux Parallel Processing (C Project)

## 📌 Description
This project is implemented in C under Linux to analyze the performance of different execution models for finding the top 10 most frequent words in the enwik8 dataset.

It compares three approaches:
- Sequential (Naive) execution
- Multiprocessing using fork()
- Multithreading using pthreads

The program measures execution time for each approach and compares performance under different numbers of processes and threads.

Dataset:
https://huggingface.co/datasets/LTCB/enwik8

---

## ⚙️ Implemented Approaches

### 1️⃣ Sequential (Naive)
- Single process
- No parallelism
- Baseline execution time

---

### 2️⃣ Multiprocessing (fork)
- Uses Linux fork() system call
- Splits dataset across child processes
- Tested with:
  - 2 processes
  - 4 processes
  - 6 processes
  - 8 processes

---

### 3️⃣ Multithreading (pthreads)
- Uses POSIX threads (pthread library)
- Shared memory model
- Tested with:
  - 2 threads
  - 4 threads
  - 6 threads
  - 8 threads

---

## ⏱️ Performance Measurement
- Execution time measured using:
  - `gettimeofday()` or `clock_gettime()`
- Comparison between all approaches
- Speedup analysis

---

## 🧠 Concepts Used
- Process creation using fork()
- Thread creation using pthreads
- Synchronization techniques (mutex if used)
- Parallel computation
- File processing in C
- Word frequency counting
- CPU performance analysis

---

## 🖥️ System Requirements
- Linux operating system
- GCC compiler
- Minimum 4 CPU cores (recommended)
- POSIX threads support

---

## 🚀 
How to Compile & Run

### Compile
```bash id="compile_os_c"
gcc naive.c -o naive
gcc fork_version.c -o fork_version
gcc pthread_version.c -o pthread_version -lpthread

Run Sequential version
./naive

Run Multiprocessing version
./fork_version

Run Multithreading version
./pthread_version

📊 Expected Output
Top 10 most frequent words
Execution time for each approach
Performance comparison table
📈 Amdahl’s Law Analysis

The report includes:

Serial vs parallel portion of code
Maximum theoretical speedup
Effect of number of cores
Optimal number of threads/processes
📚 Learning Outcomes
Linux process management (fork)
POSIX threads programming
Parallel performance optimization
System-level programming in C
CPU utilization and benchmarking
Real-world application of Amdahl’s Law
