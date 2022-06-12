#include "trie.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// Returns the position of the bit that represents the existence of this symbol
// in the bitfield, or -1 if the symbol is not in the alphabet we support.
int get_bit_pos(char symbol) {
  if ('A' <= symbol && symbol <= 'Z')
    return 'Z' - symbol + 1;
  else switch (symbol) {
      case '_'  : return 'Z' - 'A' + 2;
      case '.'  : return 'Z' - 'A' + 3;
      case '-'  : return 'Z' - 'A' + 4;
      case '\'' : return 'Z' - 'A' + 5;
      case ' '  : return 'Z' - 'A' + 6;
      default   : return -1;
    }
}

trie_t *trie_new() {
  return NULL;
}

int count_set_bits(uint32_t n) {
  return 0;
}

void trie_free(trie_t *root) {
}

bool trie_get(const trie_t *root, const char *key, int *value_out) {
  assert(root);
  return false;
}

bool trie_insert(trie_t *root, const char *key, int value) {
  assert(root);
  return false;
}

#ifdef TRIE_MAIN

// TODO: DO NOT MODIFY TIHS FUNCTION!!! Remember to run:
//  valgrind --leak-check=full --show-leak-kinds=all ./trie

int main(void) {
  #define SIZE (8)
  char ks[SIZE][10] = { "GOOD", "",   "W",  "-_ _-", "123", "*",   "()",  "+}{" };
  bool bs[SIZE] =     { true,   true, true, true,    false, false, false, false };
  int vs[SIZE] =      { 12,     0,    -1,   2342,    999,   0,     2,     4 };

  printf("Mapping strings to ints.\n");
  trie_t *root = trie_new();
  for (int i = 0; i < SIZE; ++i) {
    char *status = bs[i] ? "(insert should succeed)" : "(insert should fail)";
    if (trie_insert(root, ks[i], vs[i])) {
      printf("Was able to insert %s with value %d %s\n", ks[i], vs[i], status);
    } else {
      printf("Unable to insert %s with value %d %s\n", ks[i], vs[i], status);
    }
  }
  for (int i = SIZE - 1; i >= 0; --i) {
    char *status = bs[i] ? "(get should succeed)" : "(get should fail)";
    int v;
    if (trie_get(root, ks[i], &v)) {
      printf("Was able to get %s -> %d %s\n", ks[i], v, status);
    } else {
      printf("Unable to get %s %s\n", ks[i], status);
    }
  }
  trie_free(root);
}

#endif
