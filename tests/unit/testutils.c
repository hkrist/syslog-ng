#include "testutils.h"

#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

struct timeval start_time_val;

static void
print_failure(const gchar *custom_template, va_list custom_args, gchar *assertion_failure_template, ...)
{
  va_list assertion_failure_args;
  fprintf(stderr, "\n  ###########################################################################\n  #\n");
  fprintf(stderr,   "  # FAIL: ASSERTION FAILED: ");
  vfprintf(stderr, custom_template, custom_args);
  fprintf(stderr, "; ");

  va_start(assertion_failure_args, assertion_failure_template);
  vfprintf(stderr, assertion_failure_template, assertion_failure_args);
  va_end(assertion_failure_args);

  fprintf(stderr, "\n  #\n  ###########################################################################\n\n");
}

void
start_stopwatch()
{
  gettimeofday(&start_time_val, NULL);
}

void
stop_stopwatch_and_display_result(gchar *message_template, ...)
{
  va_list args;
  guint64 diff;
  struct timeval end_time_val;
  gettimeofday(&end_time_val, NULL);

  va_start(args, message_template);
  vprintf(message_template, args);
  va_end(args);

  diff = (end_time_val.tv_sec - start_time_val.tv_sec) * 1000000 + end_time_val.tv_usec - start_time_val.tv_usec;
  printf("; runtime=%lu.%06lu s\n", diff / 1000000, diff % 1000000);
}

gchar **
fill_string_array(gint number_of_elements, ...)
{
  va_list args;
  gint i;
  gchar **result;

  result = g_new(gchar *, number_of_elements);

  va_start(args, number_of_elements);
  for (i = 0; i < number_of_elements; ++i)
    {
      result[i] = va_arg(args, char *);
    }

  va_end(args);
  return result;
}

gboolean
assert_gint64_non_fatal(gint64 actual, gint64 expected, const gchar *error_message, ...)
{
  va_list args;

  if (actual == expected)
    return TRUE;

  va_start(args, error_message);
  print_failure(error_message, args, "actual=%lld, expected=%lld", actual, expected);
  va_end(args);

  return FALSE;
}

gboolean
assert_guint64_non_fatal(guint64 actual, guint64 expected, const gchar *error_message, ...)
{
  va_list args;

  if (actual == expected)
    return TRUE;

  va_start(args, error_message);
  print_failure(error_message, args, "actual=%llu, expected=%llu", actual, expected);
  va_end(args);

  return FALSE;
}

gboolean
assert_string_non_fatal(const gchar *actual, const gchar *expected, const gchar *error_message, ...)
{
  va_list args;

  if (expected == NULL && actual == NULL)
    return TRUE;

  if (actual != NULL && expected != NULL && strcmp(actual, expected) == 0)
    return TRUE;

  va_start(args, error_message);
  print_failure(error_message, args, "actual=" PRETTY_STRING_FORMAT ", expected=" PRETTY_STRING_FORMAT "",
                                     PRETTY_STRING(actual), PRETTY_STRING(expected));
  va_end(args);

  return FALSE;
}

static gboolean
compare_arrays_trivially(void *actual, guint32 actual_length,
                               void *expected, guint32 expected_length,
                               const gchar *error_message_template, va_list error_message_args)
{
  if (expected_length != actual_length)
    {
      print_failure(error_message_template, error_message_args, "actual_length=%u, expected_length=%u", actual_length, expected_length);
      return FALSE;
    }

  if (expected_length > 0 && actual == NULL)
    {
      print_failure(error_message_template, error_message_args, "actual=NULL, expected_length=%u", expected_length);
      return FALSE;
    }

  return TRUE;
}

gboolean
assert_guint32_array_non_fatal(guint32 *actual, guint32 actual_length, guint32 *expected, guint32 expected_length, const gchar *error_message, ...)
{
  va_list args;
  gboolean assertion_ok = TRUE;
  guint32 i;

  va_start(args, error_message);

  assertion_ok = compare_arrays_trivially((void *)actual, actual_length, (void *)expected, expected_length, error_message, args);
  if (assertion_ok)
    {
      for (i = 0; i < expected_length; ++i)
        {
          if (expected[i] != actual[i])
            {
              print_failure(error_message, args, "actual=%u, expected=%u, index=%u", actual[i], expected[i], i);
              assertion_ok = FALSE;
              break;
            }
        }
    }

  va_end(args);

  return assertion_ok;
}

/* NOTE: this does the same as g_strcmp0(), but we use an older glib, which lacks this function */
static gboolean
are_strings_equal(gchar *a, gchar *b)
{
  if (a == NULL && b == NULL)
    return TRUE;

  if (a == NULL || b == NULL)
    return FALSE;

  return strcmp(a, b) == 0;
}

gboolean
assert_string_array_non_fatal(gchar **actual, guint32 actual_length, gchar **expected, guint32 expected_length, const gchar *error_message, ...)
{
  va_list args;
  gboolean assertion_ok = TRUE;
  guint32 i;

  va_start(args, error_message);

  assertion_ok = compare_arrays_trivially((void *)actual, actual_length, (void *)expected, expected_length, error_message, args);
  if (assertion_ok)
    {
      for (i = 0; i < expected_length; ++i)
        {
          if (!are_strings_equal(actual[i], expected[i]))
            {
              print_failure(error_message, args, "actual=" PRETTY_STRING_FORMAT ", expected=" PRETTY_STRING_FORMAT ", index=%u",
                                                 PRETTY_STRING(actual[i]), PRETTY_STRING(expected[i]), i);
              assertion_ok = FALSE;
              break;
            }
        }
    }

  va_end(args);

  return assertion_ok;
}

gboolean
assert_gboolean_non_fatal(gboolean actual, gboolean expected, const gchar *error_message, ...)
{
  va_list args;

  if (actual == expected)
    return TRUE;

  va_start(args, error_message);
  print_failure(error_message, args, "actual=%s, expected=%s", gboolean_to_string(actual), gboolean_to_string(expected));
  va_end(args);

  return FALSE;
}

gboolean
assert_null_non_fatal(void *pointer, const gchar *error_message, ...)
{
  va_list args;

  if (pointer == NULL)
    return TRUE;

  va_start(args, error_message);
  print_failure(error_message, args, "Pointer expected to be NULL; pointer=%llx", (guint64)pointer);
  va_end(args);

  return FALSE;
}

gboolean
assert_not_null_non_fatal(void *pointer, const gchar *error_message, ...)
{
  va_list args;

  if (pointer != NULL)
    return TRUE;

  va_start(args, error_message);
  print_failure(error_message, args, "Unexpected NULL pointer");
  va_end(args);

  return FALSE;
}

gboolean
assert_no_error_non_fatal(GError *error, const gchar *error_message, ...)
{
  va_list args;

  if (error == NULL)
    return TRUE;

  va_start(args, error_message);
  print_failure(error_message, args, "GError expected to be NULL; message='%s'", error->message);
  va_end(args);

  return FALSE;
}

static int
cmp_guint32(const void *a, const void *b)
{
  return (*(guint32 *)a - *(guint32 *)b);
}

gboolean
assert_guint32_set_non_fatal(guint32 *actual, guint32 actual_length, guint32 *expected, guint32 expected_length, const gchar *error_message, ...)
{
  va_list args;
  gboolean ret;

  if (actual_length != expected_length)
    {
      va_start(args, error_message);
      print_failure(error_message, args, "actual_length='%d', expected_length='%d'", actual_length, expected_length);
      va_end(args);
      return FALSE;
    }

  qsort(actual, actual_length, sizeof(guint32), cmp_guint32);
  qsort(expected, expected_length, sizeof(guint32), cmp_guint32);

  va_start(args, error_message);
  ret = assert_guint32_array(actual, actual_length, expected, expected_length, error_message, args);
  va_end(args);

  return ret;
}

gboolean
assert_gpointer_non_fatal(gpointer actual, gpointer expected, const gchar *error_message, ...)
{
  va_list args;

  if (actual == expected)
    return TRUE;

  va_start(args, error_message);
  print_failure(error_message, args, "actual=%x, expected=%x", actual, expected);
  va_end(args);

  return FALSE;
}