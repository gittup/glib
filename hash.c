#include <stdio.h>
#include <stdint.h>
#include "glib.h"

struct id_tree {
	struct rb_node rbn;
	uint32_t id;
	void *ptr;
};

static inline void id_tree_rm(struct rb_root *root, struct id_tree *it)
{
	rb_erase(&it->rbn, root);
}

struct id_tree *id_tree_search(struct rb_root *root, uint32_t id)
{
	struct rb_node *node = root->rb_node;

	while (node) {
		struct id_tree *data = rb_entry(node, struct id_tree, rbn);
		int result;

		result = id - data->id;

		if (result < 0)
			node = node->rb_left;
		else if (result > 0)
			node = node->rb_right;
		else
			return data;
	}
	return NULL;
}

int id_tree_insert(struct rb_root *root, struct id_tree *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;

	/* Figure out where to put new node */
	while (*new) {
		struct id_tree *this = rb_entry(*new, struct id_tree, rbn);
		int result = data->id - this->id;

		parent = *new;
		if (result < 0)
			new = &((*new)->rb_left);
		else if (result > 0)
			new = &((*new)->rb_right);
		else
			return -1;
	}

	/* Add new node and rebalance tree. */
	rb_link_node(&data->rbn, parent, new);
	rb_insert_color(&data->rbn, root);

	return 0;
}

int id_tree_add(struct rb_root *root, uint32_t id, void *ptr)
{
	struct id_tree *it;

	it = malloc(sizeof *it);
	if(!it) {
		perror("malloc");
		return -1;
	}
	it->id = id;
	it->ptr = ptr;
	if(id_tree_insert(root, it) < 0) {
		fprintf(stderr, "Error: Unable to insert duplicate id %lli\n", id);
		return -1;
	}
	return 0;
}

void id_tree_remove(struct rb_root *root, uint32_t id)
{
	struct id_tree *it;

	it = id_tree_search(root, id);
	if(!it) {
		return;
	}
	id_tree_rm(root, it);
	free(it);
}

GHashTable *g_hash_table_new(GHashFunc hash_func, GEqualFunc key_equal_func)
{
	struct rb_root *tree;

	tree = malloc(sizeof *tree);
	return tree;
}

GHashTable* g_hash_table_new_full(GHashFunc hash_func,
				  GEqualFunc key_equal_func,
				  GDestroyNotify key_destroy_func,
				  GDestroyNotify value_destroy_func)
{
	return NULL;
}

gpointer g_hash_table_lookup(GHashTable	*hash_table, gconstpointer key)
{
	struct id_tree *idt;

	idt = id_tree_search(hash_table, (uint32_t)key);
	if(idt)
		return idt->ptr;
	return NULL;
}

void g_hash_table_insert(GHashTable *hash_table, gpointer key, gpointer value)
{
	id_tree_add(hash_table, (uint32_t)key, value);
}

gboolean g_hash_table_remove(GHashTable	*hash_table, gconstpointer key)
{
	id_tree_remove(hash_table, (uint32_t)key);
	return 1;
}

guint g_hash_table_foreach_remove(GHashTable *hash_table, GHRFunc func,
				  gpointer user_data)
{
	struct rb_node *rbn;
	struct rb_node *next;

	for(rbn = rb_first(hash_table), next = rb_next(rbn);
	    rbn;
	    rbn = next, next = rb_next(rbn)) {
		struct id_tree *idt = rb_entry(rbn, struct id_tree, rbn);
		if(func((gpointer)idt->id, idt->ptr, user_data)) {
			id_tree_rm(hash_table, idt);
			free(idt);
		}
	}
	return 0;
}
