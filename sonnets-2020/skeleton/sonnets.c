#include "sonnets.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Checks that files were opened correctly.
#define FILECHECK(ptr, msg) if (!(ptr)) perror(msg), exit(EXIT_FAILURE)

// Returns a pointer to the last word in line, or NULL if there are no words.
// This function adds a sentinel '\0' after the word.
char *lastwordtok(char *line) {
  assert(line);
  int pos = -1;
  for (int i = 0; line[i];  ++i) if (isalpha(line[i])) pos = i;
  if (pos == -1) return NULL;
  line[pos + 1] = '\0';
  while(pos >= 0 && line[--pos] != ' ');
  while(!isalpha(line[++pos]));
  return line + pos;
}

char *uppercase(char *str) {
  assert(str);
  return NULL;
}

#define VOWELS "AEIOU"

const char *strrhyme(const char *phonemes) {
  assert(phonemes);
  return NULL;
}

#define MAX_DICT_LINE_LENGTH 120

// Assume the given file is formatted correctly and formed only of characters
// that are supported by the alphabet from PART A.
dict_t *load_rhyme_mappings_from_file(const char *phonetic_dict_filename) {
  return NULL;
}

#define MAX_SONNET_LINE_LENGTH 80

bool next_rhyme_scheme(FILE *sonnets_file,
                       const dict_t *rhyme_mappings, char *out) {
  return false;
}

#define MAX_NUM_SONNET_LINES 20

void most_common_rhyme_scheme(FILE *sonnets_file,
                              const dict_t *rhyme_mappings, char *out) {
}


#ifdef SONNETS_MAIN

#define PHONETIC_DICT_FILE "dictionary.txt"

/* TODO: DO NOT MODIFY THIS FUNCTION!!! Remember to run:
 *  valgrind --leak-check=full --show-leak-kinds=all ./sonnets_map or
 *  valgrind --leak-check=full --show-leak-kinds=all ./sonnets_trie if your
 *  PART A is correct and want to see how much faster it is
 */
int main (void) {
  dict_t *rhyme_mappings = load_rhyme_mappings_from_file(PHONETIC_DICT_FILE);

  char *sonnets_files[3] = {"shakespeare.txt", "spenser.txt", "petrarch.txt"};
  for (int i = 0; i < 3; ++i) {
    FILE *f = fopen(sonnets_files[i], "r");
    FILECHECK(f, sonnets_files[i]);

    char rhyme_scheme[MAX_NUM_SONNET_LINES];
    most_common_rhyme_scheme(f, rhyme_mappings, rhyme_scheme);
    printf("The most common rhyme scheme of sonnets from %s is: %s\n",
           sonnets_files[i], rhyme_scheme);
    fclose(f);
  }

  dict_free(rhyme_mappings);
  return EXIT_SUCCESS;
}

#endif
