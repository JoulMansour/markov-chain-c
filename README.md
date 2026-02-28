# markov-tweet-generator

A C program that builds a **Markov chain** from a text file and generates random “tweets” (sentences) based on word transition frequencies.

- Input is split into words by whitespace
- A word ending with `.` is treated as end-of-sentence
- Randomness is controlled by a user-provided seed


```bash
gcc -Wall -Wextra -std=c99 tweets_generator.c markov_chain.c linked_list.c -o tweets_generator
./tweets_generator <seed> <num_tweets> <file_path> [max_words_to_read]
