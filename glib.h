#include <stdlib.h>
typedef char gchar;
typedef void *gpointer;
typedef const void *gconstpointer;
typedef int gboolean;
typedef unsigned int guint;
typedef int gint;

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

/* String/memory functions */
gchar *g_strdup(const gchar *str);
gchar *g_strndup(const gchar *str, int n);
gchar *g_strdup_printf(const gchar *format, ...) __attribute__((__format__ (__printf__, 1, 2)));
void *g_malloc(size_t size);
void g_free(gpointer mem);
#define g_new0(a, b) calloc(b, sizeof(a))

#define g_hash_table_size(...) 0
#define g_str_equal 0
#define g_str_hash 0
#define g_ptr_array_new(...) 0
#define g_ptr_array_add(...) do { } while(0)
#define g_ptr_array_free(...) do { } while(0)
#define g_strfreev(...) do { } while(0)

/* Hash functions */
#include "rbtree.h"

typedef guint (*GHashFunc)(gconstpointer key);
typedef gboolean (*GEqualFunc)(gconstpointer a, gconstpointer b);
typedef gboolean (*GHRFunc)(gpointer key, gpointer value, gpointer user_data);
typedef struct rb_root GHashTable;
typedef void (*GDestroyNotify)(gpointer data);

#define g_thread_init(...)

#define GUINT_TO_POINTER(a) ((void*)a)

GHashTable *g_hash_table_new(GHashFunc hash_func, GEqualFunc key_equal_func);
GHashTable* g_hash_table_new_full(GHashFunc hash_func,
				  GEqualFunc key_equal_func,
				  GDestroyNotify key_destroy_func,
				  GDestroyNotify value_destroy_func);
gpointer g_hash_table_lookup(GHashTable *hash_table, gconstpointer key);
void g_hash_table_insert(GHashTable *hash_table, gpointer key, gpointer value);
gboolean g_hash_table_remove(GHashTable *hash_table, gconstpointer key);
guint g_hash_table_foreach_remove(GHashTable *hash_table, GHRFunc func,
				  gpointer user_data);

/* Ptr Array functions */
struct pointer_array {
	void **pdata;
	int len;
};

typedef struct pointer_array GPtrArray;
