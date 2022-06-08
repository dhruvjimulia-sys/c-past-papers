#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "exam.h"

/*
 * Private function prototypes.
 */

static void _print_huffman_tree(const huffman_tree_t *, int);
static void _print_huffman_tree_codes(const huffman_tree_t *, char *, char *);

/*
 * Prints the given Huffman tree.
 */
void print_huffman_tree(const huffman_tree_t *t) {
  printf("Huffman tree:\n");
  _print_huffman_tree(t, 0);
}

/*
 * Private helper function for print_huffman_tree.
 */
static void _print_huffman_tree(const huffman_tree_t *t, int level) {
  int i;
  for (i = 0; i <= level; i++) {
    printf("  ");
  }

  if (t->left == NULL && t->right == NULL) {
    printf("Leaf: '%c' with count %d\n", t->letter, t->count);
  } else {
    printf("Node: accumulated count %d\n", t->count);

    if (t->left != NULL) {
      _print_huffman_tree(t->left, level + 1);
    }

    if (t->right != NULL) {
      _print_huffman_tree(t->right, level + 1);
    }
  }
}

/*
 * Prints the codes contained in the given Huffman tree.
 */
void print_huffman_tree_codes(const huffman_tree_t *t) {
  printf("Huffman tree codes:\n");

  char *code = calloc(MAX_CODE_LENGTH, sizeof(char)), *code_position = code;
  if (code == NULL) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  _print_huffman_tree_codes(t, code, code_position);
  free(code);
}

/*
 * Private helper function for print_huffman_tree_codes.
 */
static void _print_huffman_tree_codes(const huffman_tree_t *t,
                                      char *code, char *code_position) {

  if (t->left == NULL && t->right == NULL) {
    printf("'%c' has code \"%s\"\n", t->letter, code);
  } else if (t->left != NULL) {
    *code_position++ = 'L';
    *code_position = '\0';

    _print_huffman_tree_codes(t->left, code, code_position--);
  }

  if (t->right != NULL) {
    *code_position++ = 'R';
    *code_position = '\0';

    _print_huffman_tree_codes(t->right, code, code_position--);
  }
}

/*
 * Prints a list of Huffman trees.
 */
void print_huffman_tree_list(const huffman_tree_list_t *l) {
  printf("Huffman tree list:\n");

  for (; l != NULL; l = l->next) {
    print_huffman_tree(l->tree);
  }
}

/*
 * Frees a Huffman tree.
 */
void huffman_tree_free(huffman_tree_t *t) {
  if (t != NULL) {
    huffman_tree_free(t->left);
    huffman_tree_free(t->right);
    free(t);
  }
}

/*
 * Frees a list of Huffman trees.
 */
void huffman_tree_list_free(huffman_tree_list_t *l) {
  if (l != NULL) {
    huffman_tree_list_free(l->next);
    huffman_tree_free(l->tree);
    free(l);
  }
}

/*
 * Returns 1 if the string s contains the character c and 0 if it does not.
 */
int contains(char *s, char c) {
	assert(s);
	for (uint32_t i = 0; s[i] != '\0'; i++) {
		if (s[i] == c) {
			return 1;
		}
	}
	return 0;
}

/*
 * Returns the number of occurrences of c in s.
 */
int frequency(char *s, char c) {
	assert(s);
	uint32_t freq = 0;
	for (uint32_t i = 0; s[i] != '\0'; i++) {
		if (s[i] == c) {
			freq++;
		}
	}
	return freq;
}

/*
 * Takes a string s and returns a new heap-allocated string containing only the
 * unique characters of s.
 *
 * Pre: all strings will have fewer than or equal to MAX_STRING_LENGTH - 1
 *      characters.
 */
char *nub(char *s) {
	assert(s);
	char* nub_string = malloc(MAX_STRING_LENGTH - 1);
	assert(nub_string);
	uint32_t j = 0;
	for (uint32_t i = 0; s[i] != '\0'; i++) {
		if (!contains(nub_string, s[i])) {
			nub_string[j] = s[i];
			j++;
		}
	}
	return nub_string;
}

/*
 * Adds the Huffman tree t to the list l, returning a pointer to the new list.
 *
 * Pre:   The list l is sorted according to the frequency counts of the trees
 *        it contains.
 *
 * Post:  The list l is sorted according to the frequency counts of the trees
 *        it contains.
 */
huffman_tree_list_t *huffman_tree_list_add(huffman_tree_list_t *l, huffman_tree_t *t) {
	assert(t);
	huffman_tree_list_t *new_list = malloc(sizeof(huffman_tree_list_t));
	assert(new_list);

	if (l == NULL) {
		*new_list = (huffman_tree_list_t) {t, NULL};
	} else if (t->count < l->tree->count) {
		*new_list = (huffman_tree_list_t) {t, l};
	} else {
		*new_list = (huffman_tree_list_t) {l->tree, huffman_tree_list_add(l->next, t)};
		free(l);
	}
	return new_list;
}

/*
 * Takes a string s and a lookup table and builds a list of Huffman trees
 * containing leaf nodes for the characters contained in the lookup table. The
 * leaf nodes' frequency counts are derived from the string s.
 *
 * Pre:   t is a duplicate-free version of s.
 *
 * Post:  The resulting list is sorted according to the frequency counts of the
 *        trees it contains.
 */
huffman_tree_list_t *huffman_tree_list_build(char *s, char *t) {
	assert(s);
	assert(t);
	huffman_tree_list_t* list = NULL;
	for (uint32_t i = 0; t[i] != '\0'; i++) {
		huffman_tree_t *tree = malloc(sizeof(huffman_tree_t));
		assert(tree);
		*tree = (huffman_tree_t) {frequency(s, t[i]), t[i], NULL, NULL};
		list = huffman_tree_list_add(list, tree);
	}
	return list;
}

/*
 * Reduces a sorted list of Huffman trees to a single element.
 *
 * Pre:   The list l is non-empty and sorted according to the frequency counts
 *        of the trees it contains.
 *
 * Post:  The resuling list contains a single, correctly-formed Huffman tree.
 */
huffman_tree_list_t *huffman_tree_list_reduce(huffman_tree_list_t *l) {
	assert(l);
	assert(l->tree);
	if (l->next == NULL) {
		return l;
	}
	huffman_tree_t* new_tree = malloc(sizeof(huffman_tree_t));
	assert(new_tree);
	*new_tree = (huffman_tree_t) {l->tree->count + l->next->tree->count, '\0', l->tree, l->next->tree};
	huffman_tree_list_t *new_list = huffman_tree_list_add(l->next->next, new_tree);
	free(l->next);
	free(l);
	return huffman_tree_list_reduce(new_list);
}

char *build_encoding(huffman_tree_t *t, uint32_t encoding_position, char current) {
	if (t->left == NULL && t->right == NULL) {
		if (t->letter == current) {

		}
	}
	if (t->left != NULL) {
		char *left_pos = build_encoding(t->left, encoding_position, current);
		if (strlen(left_pos) > 0) {

		}
	}
	if (t->right != NULL) {
		build_encoding(t->right, encoding_position, current);
	}
	return NULL; 
}

/*
 * Accepts a Huffman tree t and a string s and returns a new heap-allocated
 * string containing the encoding of s as per the tree t.
 *
 * Pre: s only contains characters present in the tree t.
 */
char *huffman_tree_encode(huffman_tree_t *t, char *s) {
	char *encoding; // = malloc(MAX_CODE_LENGTH - 1);
	// uint32_t curr_position = 0;
	assert(encoding);
	for (uint32_t i = 0; s[i] != '\0'; i++) {
		build_encoding(t, 0, s[i]);
	}
	return NULL;
}

/*
 * Accepts a Huffman tree t and an encoded string and returns a new
 * heap-allocated string contained the decoding of the code as per the tree t.
 *
 * Pre: the code given is decodable using the supplied tree t.
 */
char *huffman_tree_decode(huffman_tree_t *t, char *code) {
  return NULL;
}
