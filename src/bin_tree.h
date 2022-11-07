#ifndef _BIN_TREE_H_
#define _BIN_TREE_H_

typedef struct bin_tree_entry_s {
  struct bin_tree_entry_s* left;
  struct bin_tree_entry_s* right;
} bin_tree_entry_t;

#endif  // _BIN_TREE_H_