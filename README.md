```
 _ __________ _            _____ _  _   _
| |___ /___ /| |_ ___ _ __|___ /| || | | | __
| | |_ \ |_ \| __/ __| '_ \ |_ \| || |_| |/ /
| |___) |__) | |_\__ \ |_) |__) |__   _|   <
|_|____/____/ \__|___/ .__/____/   |_| |_|\_\
                     |_|
```
Convert text into leet-speak text.


# Benchmarking
| Method | Speed |
| :--- | ---: |
| Naive Implementation | 17.29MB/second |
| Naive with -O3 | 18.49MB/second |
| AVX2 with -O3 | 403.96MB/second |
| AVX2 with -O3 and big write buffer | 1.63GB/second |
| Multithreaded AVX2 with 4MB buffer | 5.54GB/second |
