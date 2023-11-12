#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread_function(void* thread_num) {
    sleep(*(int*)thread_num);
    printf("thread %d completed\n",*(int*)thread_num);
    return NULL;
}

int main() {
    pthread_t my_thread[10];
    for (int i=0; i<10; i++){
        int j=i;
        pthread_create(&my_thread[i], NULL, thread_function, (void*)&j);

    }

    // 主线程继续执行其他代码


    for(int i=0; i<10; i++){
        printf("waiting for thread %d\n",i);
        pthread_join(my_thread[i], NULL); // 等待线程结束

    }

    printf("thread 收束");
    getchar();
    return 0;
}
