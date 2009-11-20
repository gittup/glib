#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "glib.h"

void *dlopen(const char *filename, int flag)
{
	return NULL;
}

char *dlerror(void)
{
	return "marf sez: dlopen is for suckers\n";
}

int dlclose(void)
{
	return 0;
}

void *dlsym(void *handle, const char *symbol)
{
	return NULL;
}

gchar *g_strdup(const gchar *str)
{
	return strdup(str);
}

gchar *g_strndup(const gchar *str, int n)
{
	return strndup(str, n);
}

gchar *g_strdup_printf(const gchar *format, ...)
{
	char *ptr = NULL;
	va_list ap;

	va_start(ap, format);
	vasprintf(&ptr, format, ap);
	va_end(ap);
	return ptr;
}

void *g_malloc(size_t size)
{
	return malloc(size);
}

void g_free(gpointer mem)
{
	free(mem);
}
