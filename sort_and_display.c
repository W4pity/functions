/*** submitted by petroc_m on 2016-11-07 16:10:37.956526+00:00 from 10.224.32.77 ***/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

unsigned int_width(int i)
{
  unsigned size = 0;
  //test the sighe
  if (i < 0)
  {
    size++;
    // i = -i;
  }
  size += log10(fabs(i));
  size += 1;
  return size;
}

unsigned ints_width(const int* tab, unsigned count)
{
  unsigned max = 0;
  for (unsigned a = 0; a < count; a++)
  {
    unsigned i = int_width(tab[a]);
    if (i > max)
      max = i;
  }
  return max;
}

void print_int_array(FILE* out, const int* tab, unsigned count)
{
  unsigned aligned = ints_width(tab, count);
  unsigned col = 0;
  fprintf(out, "%*s[%d]", int_width(count - 1) - 1, "", col);
  col += 2 + 1 + int_width(count - 1) - 1;
  for (unsigned i = 0; i < count; i++)
  {
    if (col + aligned -1 <= 80)
    {
      fprintf(out, " %*d", aligned, tab[i]);
      col += aligned + 1;
    }
    else
    {
      fprintf(out, "\n");
      col = 0;
      fprintf(out, "%*s[%d]", int_width(count -1) - int_width(i), "", i);
      col += 2 + int_width(count - 1) + int_width(i);
      fprintf(out, " %*d", aligned, tab[i]);
      col += aligned + 1;
    }
  }
  fprintf(out, "\n");
}

void insert_sort_cmp(int* tab, unsigned count, int (*cmp)(int a, int b))
{
  for (unsigned i = 1; i < count; i++)
  {
    int key = tab[i];
    int j = i - 1;
    while (j >= 0 && cmp(tab[j], key) > 0)
    {
      tab[j + 1] = tab[j];
      j = j - 1;
    }
    tab[j + 1] = key;
  }
}

/*********************************************
  DAY 2
 **********************************************/

unsigned linear_search(const int* tab, unsigned count, int val, int (*cmp)(int
      a, int b))
{
  for (unsigned i = 0; i < count; i++)
  {
    if (cmp(tab[i], val) >= 0)
    {
      if (tab[i] == val)
        return i;
      else
        return i;
    }
  }
  return count;
}

unsigned binary_search(const int* tab, unsigned count, int val, int (*cmp)(int
      a, int b))
{
  unsigned b = 0;
  while (b < count)
  {
    unsigned m = (b + count) / 2;
    int cmpval =  cmp(val, tab[m]);
    if (cmpval == 0)
      return m;
    else
    {
      if (cmpval > 0)
        b = m + 1;
      else
        count = m;
    }
  }
  return b;
}

/*
   unsigned long cmp_count;

   int increasing(int a, int b)
   {
   ++cmp_count;
   if (a < b)
   return -1;
   return a > b;
   }
 */
void bs_insert_sort_cmp(int *tab, unsigned count, int (*cmp)(int a, int b))
{
  for (unsigned i = 1; i < count; i++)
  {
    int key = tab[i];
    int j = binary_search(tab, i, key, cmp);
    //memmove(tab + j + 1, tab + j, i - j);
    for (int k = i; k > j; k--)
      tab[k] = tab[k - 1];
    tab[j] = key;
  }
}

unsigned interpolate(int *tab, unsigned b, unsigned e, int val,
    int (*cmp)(int, int))
{
  //unsigned interpol = (e - b) / (tab[e] + tab[b]);
  //int cp;
  while (tab[e] != tab[b] && cmp(val, tab[b]) >= 0 && cmp(val, tab[e]) <= 0)
  {
    double inter1 = b;
    double inter2 = (val - tab[b]);
    double inter3 = e - b;
    double inter4 = (tab[e] - tab[b]);
    double interpol = inter1 + (inter2 * (inter3 / inter4));
    if ((int)(interpol + 0.5f) > (int)interpol)
      interpol++;
    return interpol;
  }
  if (cmp(val, tab[b]) <= 0)
    return b;
  else
    return e;
}


unsigned interpolation_search(int *tab, unsigned count, int val,
    int (*cmp)(int a, int b))
{
  unsigned b = 0;
  while (b < count)
  {
    unsigned m = interpolate(tab, b, count-1, val, cmp);
    int cmpval =  cmp(val, tab[m]);
    if (cmpval == 0)
      return m;
    else
    {
      if (cmpval > 0)
        b = m + 1;
      else
        count = m;
    }
  }
  return b;
}

/*******************************************************
 *                       DAY 3
 *******************************************************/

void heapify_cmp(int* v, unsigned i, unsigned n, int (*cmp)(int a, int b));
void make_heap_cmp(int* v, unsigned n, int (*cmp)(int a, int b));
bool check_heap_cmp(const int* v, unsigned n, int (*cmp)(int a, int b));
int pop_heap_cmp(int* heap, unsigned* n, int (*cmp)(int a, int b));
void heap_sort_cmp(int* tab, unsigned n, int (*cmp)(int a, int b));

int leftChild(int i)
{
  return 2 * i + 1;
}

int rightChild(int i)
{
  return 2 * i + 2;
}

void swap(int tab[], int a, int b)
{
  int c = tab[a];
  tab[a] = tab[b];
  tab[b] = c;
}

void swap1(int *a, int *b)
{
  int c = *a;
  *a = *b;
  *b = c;
}

void heapify(int* tab, unsigned pos, unsigned n)
{
  unsigned l = leftChild(pos);
  unsigned r = rightChild(pos);
  unsigned largest;
  if (l < n && tab[l] > tab[pos])
    largest = l;
  else
    largest = pos;
  if (r < n && tab[r] > tab[largest])
  {
    largest = r;
  }
  if (pos != largest)
  {
    swap(tab, pos, largest);
    heapify(tab, largest, n);
  }
}

void make_heap(int* tab, unsigned n)
{
  for (int i = (n / 2) - 1; i >= 0; i--)
  {
    heapify(tab, i, n);
  }
}

bool check_heap(int* heap, unsigned n)
{
  for (unsigned i = 0; i < n/2; i++)
  {
    if (heap[i] < heap[2 * i + 1])
      return false;
    if (i < n && heap[i] < heap[2 * i + 2])
      return false;
  }
  return true;
}

void pretty_print_heap(FILE* out, const int* v, unsigned n)
{
  unsigned w = ints_width(v, n);
  unsigned h = log2(n);
  unsigned t = (w + 1) * pow(2, h);
  unsigned c = 1;
  unsigned index = 0;
  for (unsigned i = 0; i <= h; i++)
  {
    unsigned s = (t / c) - w;
    fprintf(out, "%*s%*d", s/2, "",w, v[index++]);
    for (unsigned j = 1; index < n && j < c ; j++)
    {
      fprintf(out, "%*s%*d", s, "", w, v[index++]);
    }
    fprintf(out, "\n");
    c = 2 * c;
  }
}


int pop_heap(int* heap, unsigned* n)
{
  int v = *heap;
  *heap = *(heap + (*n) - 1);
  (*n)--;
  make_heap(heap, *n);
  return v;
}


void heap_sort(int* tab, unsigned n)
{
  make_heap(tab, n);
  for (unsigned i = n - 1; i > 0; i--)
  {
    swap(tab, i, 0);
    heapify(tab, 0, i);
  }
}


void heapify_cmp(int* v, unsigned i, unsigned n, int (*cmp)(int a, int b))
{
  unsigned l = leftChild(i);
  unsigned r = rightChild(i);
  unsigned largest;
  if (l < n && cmp(v[l], v[i]) > 0)
    largest = l;
  else
    largest = i;
  if (r < n && cmp(v[r], v[largest]) > 0)
  {
    largest = r;
  }
  if (i != largest)
  {
    swap(v, i, largest);
    heapify_cmp(v, largest, n, cmp);
  }

}

void heap_sort_cmp(int* tab, unsigned n, int (*cmp)(int a, int b))
{
  make_heap_cmp(tab, n, cmp);
  for (unsigned i = n - 1; i > 0; i--)
  {
    swap(tab, i, 0);
    heapify_cmp(tab, 0, i, cmp);
  }

}


void make_heap_cmp(int* v, unsigned n, int (*cmp)(int a, int b))
{
  for (int i = (n / 2) - 1; i >= 0; i--)
  {
    heapify_cmp(v, i, n, cmp);
  }

}


bool check_heap_cmp(const int* v, unsigned n, int (*cmp)(int a, int b))
{
  for (unsigned i = 0; i < n/2; i++)
  {
    if (cmp(v[i], v[2 * i + 1]) < 0)
      return false;
    if (i < n && cmp(v[i], v[2 * i + 2]) < 0)
      return false;
  }
  return true;
}

int pop_heap_cmp(int* heap, unsigned* n, int (*cmp)(int a, int b))
{
  int v = *heap;
  *heap = *(heap + (*n) - 1);
  (*n)--;
  make_heap_cmp(heap, *n, cmp);
  return v;

}

/********************************************************************
  DAY 4
 ********************************************************************/
int increasing(int a, int b)
{
  if (a < b)
    return -1;
  return a > b;
}

unsigned partition_cmp(int* tab, unsigned b, unsigned e,
    unsigned p, int (*cmp)(int a, int b))
{
  int x = tab[p];
  unsigned i = b - 1;
  unsigned j = e;
  while (1) {
    do  i++; while (cmp(tab[i], x) < 0);
    do  j--; while (cmp(tab[j], x) > 0);
    if  (i < j)
      swap(tab, i, j);
    else
      return i + (i == b);
  }
}


unsigned pivot_first(const int* tab, unsigned l, unsigned r,
    int (*cmp)(int a, int b))
{
  (void) tab;
  (void) r;
  (void) cmp;
  return l;
}

unsigned pivot_rand(const int* tab, unsigned l,
    unsigned r,
    int (*cmp)(int a, int b))
{
  (void) tab;
  (void) cmp;
  return l + (rand() % (r - l));
}

unsigned pivot_median3(const int* tab, unsigned l, unsigned r,
    int (*cmp)(int a, int b))
{
  int center = l + (r - l) / 2;

  if ((cmp(tab[l], tab[r-1]) < 0 && cmp(tab[r - 1], tab[center]) < 0)
      || (cmp(tab[l], tab[r-1]) > 0 && cmp(tab[r - 1], tab[center]) > 0))
    return r - 1;
  if ((cmp(tab[r - 1], tab[l]) < 0 && cmp(tab[l], tab[center]) < 0)
      || (cmp(tab[r - 1], tab[l]) > 0 && cmp(tab[l], tab[center]) > 0))
    return l;
  return center;
}


void quick_sort_cmp(int* tab, unsigned count,
    unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
      int (*cmp)(int a, int b)), int (*cmp)(int a, int b))
{
  if (count > 1)
  {
    unsigned p = partition_cmp(tab, 0, count, pivot(tab, 0, count, cmp), cmp);
    quick_sort_cmp(tab, p, pivot, cmp);
    quick_sort_cmp(tab + p, count - p, pivot, cmp);
  }
}

uintmax_t cmp_count;
int decreasing(int a, int b)
{
  if (a > b)
    return -1;
  return a < b;
}
/*************************************************************************/
int my_decreasing(int a, int b)
{
  ++cmp_count;
  if (a < b)
    return -1;
  return a > b;
}

void quick_sort_cmp_loop(int* tab, unsigned count,
    unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
      int (*cmp)(int a, int b)), int (*cmp)(int a, int b))
{
  while (count > 1)
  {
    unsigned p = partition_cmp(tab, 0, count, pivot(tab, 0, count, cmp), cmp);
    if (p < count - p)
    {
      quick_sort_cmp_loop(tab, p, pivot, cmp);
      count = count - p;
      tab = tab + p;
    }
    else
    {
      quick_sort_cmp_loop(tab + p, count - p, pivot, cmp);
      count = p;
    }
  }
}



int my_increasing(int a, int b)
{
  ++cmp_count;
  if (a < b)
    return -1;
  return a > b;
}
/*
#define mycheck(fun, tab)                                  \
{                                                        \
  cmp_count = 0;                                         \
  clock_t before = clock();                              \
  fun(tab, tsize, pivot_median3, my_increasing);         \
  clock_t after = clock();                               \
  printf(#fun " used %ju comparisons and %ju ticks\n",   \
      cmp_count, (uintmax_t) (after - before)); \
}

int main()
{
  srand(0);
  const unsigned tsize = 500000;
  int* tab1 = malloc(tsize *
      sizeof(int));
  int* tab2 = malloc(tsize *
      sizeof(int));
  for (unsigned i = 0; i < tsize;
      ++i)
    tab2[i] = tab1[i] = rand(); //= i;

  mycheck(quick_sort_cmp,
      tab1);
  mycheck(quick_sort_cmp_loop,
      tab2);
  return 0;
}*/
unsigned small_threshold = 10;

void quick_sort_cmp_adapt(int* tab, unsigned count,
    unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
      int (*cmp)(int a, int b)), int (*cmp)(int a, int b))
{
   while (count > small_threshold)
  {
    unsigned p = partition_cmp(tab, 0, count, pivot(tab, 0, count, cmp), cmp);
    if (p < count - p)
    {
      quick_sort_cmp_adapt(tab, p, pivot, cmp);
      count = count - p;
      tab = tab + p;
    }
    else
    {
      quick_sort_cmp_adapt(tab + p, count - p, pivot, cmp);
      count = p;
    }
  }
  insert_sort_cmp(tab, count, cmp);
 }




void intro_sort_cmp_rec(int* tab, unsigned count, int (*cmp)(int a, int b), int
log, int current)
{
  while (count > small_threshold)
  {
    if (current > log)
    {
      heap_sort_cmp(tab, count, cmp);
      return;
    }
    unsigned p = partition_cmp(tab, 0, count, pivot_median3(tab, 0, count, cmp), cmp);
    if (p < count - p)
    {
      intro_sort_cmp_rec(tab, p, cmp, log, ++current);
      count = count - p;
      tab = tab + p;
    }
    else
    {
      intro_sort_cmp_rec(tab + p, count - p, cmp, log, ++current);
      count = p;
    }
  }
  insert_sort_cmp(tab, count, cmp);

}

void intro_sort_cmp(int* tab, unsigned count, int (*cmp)(int a, int b))
{
  int log = 2 * log2(count);
  intro_sort_cmp_rec(tab, count, cmp, log, 0);

}



void median_of_3_killer(int* tab, unsigned count)
{
  assert((count & 1) == 0);
  unsigned k = count / 2;
  for (unsigned i = 1; i <= k; ++i)
  {
    tab[i - 1] = (i & 1) ? i : k + i - 1;
    tab[k + i - 1] = 2 * i;
  }
}

unsigned int coin_combo_rec(const unsigned int* values, unsigned count, unsigned
int amount)
{
  unsigned res = 0;
  if (amount ==  0)
    return 1;
  for (unsigned i = 0; i < count; i++ )
  {
    if (values[i] <= amount)
      res += coin_combo_rec(values + i, count - i, amount - values[i]);
    else
      break;
  }
  return res;
}


unsigned int
coin_combo(const unsigned int* values, unsigned count, unsigned int amount)
{
  intro_sort_cmp(values, count, increasing);
  return coin_combo_rec(values, count, amount);
}

