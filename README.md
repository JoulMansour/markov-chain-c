# markov-chain-c

Generic Markov chain library in C (stores `void*` states and uses function pointers for compare/copy/free/print and “is last” checks).

Includes two example programs:
- **tweets_generator**: learns word transitions from a text file and generates random “tweets”.
- **snakes_and_ladders**: generates random walks on a Snakes & Ladders board using the Markov chain.

## Build
```bash
make tweets_generator
make snakes_and_ladders

./tweets_generator <seed> <num_tweets> <file_path> [max_words_to_read]
./snakes_and_ladders <seed> <num_walks>
