#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include "sys/time.h"

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 10000
#define CHUNK_SIZE 17900000
#define NUM_THREADS 4

typedef struct {
    char word[MAX_WORD_LENGTH];
    int repetition;
} Word;

typedef struct {
    int start;
    int end;
    const char *name;
    FILE *sharedFile;
} ThreadData;

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

int partition(Word[],int, int);
void quick(Word[],int,int);
int find(Word[], int, char *);
void insert(Word[], int *, char *);
void process(const char *, int, int, FILE *);
void *thread(void *arg);

int main() {

    struct timeval start, end;
    double elapsedTime;

    // Start timer
    gettimeofday(&start, NULL);

    // Create a temporary file for storing shared results
    FILE *shared_file = fopen("shared_output.txt", "w+");
    if (!shared_file) {
        perror("Failed to create shared file");
        return 1;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int chunk_size = CHUNK_SIZE / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i + 1) * chunk_size;
        thread_data[i].name = "text8.txt";
        thread_data[i].sharedFile = shared_file;

        pthread_create(&threads[i], NULL, thread, (void *)&thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Reopen the shared file to combine results
    fclose(shared_file);
    shared_file = fopen("shared_output.txt", "r");
    if (!shared_file) {
        printf("Failed to reopen shared file");
        return 1;
    }

    Word final_words[MAX_WORDS];
    int final_size = 0;
    char word[MAX_WORD_LENGTH];
    int count;

    while (fscanf(shared_file, "%s %d", word, &count) != EOF) {
        for (int i = 0; i < count; i++) {
            insert(final_words, &final_size, word);
        }
    }

    fclose(shared_file);


    // Sort and print the top 15 most frequent words
    quick(final_words,0,final_size-1);

    printf("Top 10 most frequent words:\n");
    printf("Multithreading approach: (at 2 threads) \n");
    for (int i = 0; i < 15 && i < final_size; i++) {
        if(i==0){
            printf("%d- %s: %d\n",i+1,final_words[i].word, final_words[i].repetition);

        }
        else if(i>5){
            printf("%d- %s: %d\n",i-4,final_words[i].word, final_words[i].repetition);;
        }
    }

    // Stop timer
    gettimeofday(&end, NULL);
    elapsedTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("\nElapsed time: %f seconds\n", elapsedTime);

    // Remove temporary file
    remove("shared_output.txt");

    return 0;
}
int partition(Word words[], int low, int high) {
    int pivot = words[high].repetition ; // Choose the last element as pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (words[j].repetition >= pivot) { // Sort in descending order
            i++;
            // Swap words[i] and words[j]
            Word temp = words[i];
            words[i] = words[j];
            words[j] = temp;
        }
    }

    // Place the pivot in the correct position
    Word temp = words[i + 1];
    words[i + 1] = words[high];
    words[high] = temp;

    return i + 1;
}
void quick(Word words[], int low, int high) {
    if (low < high) {
        int pivot = partition(words, low, high);

        // Recursively sort the two halves
        quick(words, low, pivot - 1);
        quick(words, pivot + 1, high);
    }
}
int find(Word words[], int size, char *word) {
    for (int i = 0; i < size; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}
void insert(Word words[], int *size, char *word) {
    int index = find(words, *size, word);
    if (index >= 0) {
        words[index].repetition++;
    } else {
        strcpy(words[*size].word, word);
        words[*size].repetition = 1;
        (*size)++;
    }
}
void process(const char *name, int start, int end, FILE *sharedFile) {
    FILE *input = fopen(name, "r");
    if (!input) {
        printf("Failed to open file");
        exit(1);
    }

    Word words[MAX_WORDS];
    int size = 0;
    char word[MAX_WORD_LENGTH];
    int count = 0;

    while (fscanf(input, "%s", word) != EOF && count < end) {
        if (count >= start) {
            insert(words, &size, word);
        }
        count++;
    }

    fclose(input);

    quick(words,0,size-1);

    // Lock before writing to the shared file
    pthread_mutex_lock(&file_mutex);
    for (int i = 0; i < size; i++) {
        fprintf(sharedFile, "%s %d\n", words[i].word, words[i].repetition);
    }
    pthread_mutex_unlock(&file_mutex);
}
void *thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    process(data->name, data->start, data->end, data->sharedFile);
    return NULL;
}