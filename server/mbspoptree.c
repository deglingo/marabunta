/* mbspoptree.c -
 */

#include "server/srvprivate.h"
#include "server/mbspoptree.h"



/* #define TREE_CHECK(tree) mbs_pop_tree_check(tree) */
#define TREE_CHECK(tree)

#define TREE_ASSERT(tree, expr, args...) do {       \
    if (!(expr)) {                                  \
      CL_DEBUG("TREE ASSERT FAILED: `" #expr "'");  \
      CL_DEBUG(args);                               \
      mbs_pop_tree_dump(tree);                      \
      abort();                                      \
    }                                               \
  } while (0)



/* mbs_pop_unit_new:
 */
static MbsPopUnit *mbs_pop_unit_new ( MbPopType type,
                                      guint birthdate,
                                      gint64 count )
{
  MbsPopUnit *unit;
  unit = g_new0(MbsPopUnit, 1);
  unit->type = type;
  unit->birthdate = birthdate;
  unit->count = count;
  return unit;
}



struct dump_data
{
  int dummy;
};



static void _dump ( MbsPopUnit *unit,
                    gpointer data_ )
{
  /* struct dump_data *data = data_; */
  CL_DEBUG(" - tp=%d, birthdate=%8d, count=%16" G_GINT64_FORMAT,
           unit->type, unit->birthdate, unit->count);
}



/* mbs_pop_tree_dump:
 */
void mbs_pop_tree_dump ( MbsPopTree *tree )
{
  struct dump_data data;
  memset(&data, 0, sizeof(struct dump_data));
  CL_DEBUG("****  TREE DUMP  ****");
  mbs_pop_tree_traverse(tree, _dump, &data);
  CL_DEBUG("*********************");
}



/* mbs_pop_unit_free:
 */
static void mbs_pop_unit_free ( MbsPopUnit *unit )
{
  g_free(unit);
}



/* mbs_pop_unit_check:
 */
static inline gint mbs_pop_unit_check ( MbsPopUnit *unit,
                                        MbPopType type,
                                        guint birthdate )
{
  if (unit->type < type)
    return -1;
  else if (unit->type > type)
    return 1;
  else if (unit->birthdate < birthdate)
    return -1;
  else if (unit->birthdate > birthdate)
    return 1;
  else
    return 0;
}



/* mbs_pop_tree_new:
 */
MbsPopTree *mbs_pop_tree_new ( void )
{
  MbsPopTree *tree;
  tree = g_new0(MbsPopTree, 1);
  return tree;
}



struct check_data
{
  MbsPopTree *tree;
  gint64 pop[MB_POP_TYPE_COUNT];
};



static void _check ( MbsPopUnit *unit,
                     struct check_data *data )
{
  if (unit->type == MB_POP_NONE)
    {
      ASSERT(!unit->left);
      ASSERT(!unit->right);
    }
  else
    {
      ASSERT(unit->left);
      ASSERT(unit->left->parent == unit);
      ASSERT(unit->right);
      ASSERT(unit->right->parent == unit);
      if (unit->left->type != MB_POP_NONE)
        ASSERT(mbs_pop_unit_check(unit, unit->left->type, unit->left->birthdate) < 0);
      if (unit->right->type != MB_POP_NONE)
        ASSERT(mbs_pop_unit_check(unit, unit->right->type, unit->right->birthdate) > 0);
      ASSERT(unit->type >= 0 && unit->type < MB_POP_TYPE_COUNT);
      data->pop[unit->type] += unit->count;
      _check(unit->left, data);
      _check(unit->right, data);
    }
}



/* mbs_pop_tree_check:
 */
void mbs_pop_tree_check ( MbsPopTree *tree )
{
  struct check_data data;
  gint tp;
  if (!tree->root)
    return;
  memset(&data, 0, sizeof(struct check_data));
  data.tree = tree;
  ASSERT(!tree->root->parent);
  _check(tree->root, &data);
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
    TREE_ASSERT(tree,
                tree->pop[tp] == data.pop[tp],
                "tp=%d, tree_pop=%" G_GINT64_FORMAT ", data_pop=%" G_GINT64_FORMAT,
                tp, tree->pop[tp], data.pop[tp]);
}



static void _clear ( MbsPopUnit *unit )
{
  ASSERT(!unit->task); /* [todo] */
  if (unit->left)
    _clear(unit->left);
  if (unit->right)
    _clear(unit->right);
  mbs_pop_unit_free(unit);
}



/* mbs_pop_tree_clear:
 */
void mbs_pop_tree_clear ( MbsPopTree *tree )
{
  gint tp;
  if (tree->root) {
    _clear(tree->root);
    tree->root = NULL;
  }
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
    tree->pop[tp] = 0;
}



/* lookup_unit:
 *
 * Note: only returns NULL in one case: when tree->root is NULL (ie
 * when the tree is empty) - in other cases, a lookup failure returns
 * the a node with type = MB_POP_NONE (null node).
 */
static inline MbsPopUnit *lookup_unit ( MbsPopTree *tree,
                                        MbPopType type,
                                        guint birthdate )
{
  MbsPopUnit *unit;
  if (!tree->root)
    return NULL;
  unit = tree->root;
  while (unit->type != MB_POP_NONE)
    {
      gint d = mbs_pop_unit_check(unit, type, birthdate);
      if (d < 0)
        unit = unit->left;
      else if (d > 0)
        unit = unit->right;
      else
        return unit;
      ASSERT(unit);
    }
  return unit;
}



static void insert_root ( MbsPopTree *tree,
                          MbsPopUnit *unit )
{
  ASSERT(!tree->root);
  ASSERT(!unit->parent);
  ASSERT(!unit->left);
  ASSERT(!unit->right);
  tree->root = unit;
  unit->left = mbs_pop_unit_new(MB_POP_NONE, 0, 0);
  unit->left->parent = unit;
  unit->right = mbs_pop_unit_new(MB_POP_NONE, 0, 0);
  unit->right->parent = unit;
  unit->red = 0; /* the root is black */
  TREE_CHECK(tree);
}



static void insert_case_1 ( MbsPopTree *tree,
                            MbsPopUnit *unit );
static void insert_case_2 ( MbsPopTree *tree,
                            MbsPopUnit *unit );
static void insert_case_3 ( MbsPopTree *tree,
                            MbsPopUnit *unit );
static void insert_case_4 ( MbsPopTree *tree,
                            MbsPopUnit *unit );
static void insert_case_5 ( MbsPopTree *tree,
                            MbsPopUnit *unit );



static inline MbsPopUnit *unit_grandparent ( MbsPopUnit *unit )
{
  if (unit->parent)
    return unit->parent->parent;
  else
    return NULL;
}



static inline MbsPopUnit *unit_uncle ( MbsPopUnit *unit )
{
  MbsPopUnit *gp = unit_grandparent(unit);
  if (!gp)
    return NULL;
  if (unit->parent == gp->left)
    return gp->right;
  else
    return gp->left;
}



static inline void rotate_left ( MbsPopTree *tree,
                                 MbsPopUnit *root )
{
  MbsPopUnit *pivot;
  /* CL_TRACE("tree=%p, root=%p", tree, root); */
  /* TREE_CHECK(tree); */
  ASSERT(root);
  ASSERT(root->type != MB_POP_NONE);
  pivot = root->right;
  ASSERT(pivot);
  /* ASSERT(pivot->type != MB_POP_NONE); */
  if (tree->root == root) {
    tree->root = pivot;
  } else {
    ASSERT(root->parent);
    if (root == root->parent->left) {
      root->parent->left = pivot;
    } else {
      ASSERT(root == root->parent->right);
      root->parent->right = pivot;
    }
  }
  pivot->parent = root->parent;
  if ((root->right = pivot->left))
    root->right->parent = root;
  pivot->left = root;
  root->parent = pivot;
  /* TREE_CHECK(tree); */
  /* root->parent = pivot; */
}



/* rotate_right:
 *
 * Pivot = Root.OS
 * Root.OS = Pivot.RS
 * Pivot.RS = Root
 * Root = Pivot
 *
 */
static inline void rotate_right ( MbsPopTree *tree,
                                  MbsPopUnit *root )
{
  MbsPopUnit *pivot;
  /* CL_TRACE("tree=%p, root=%p", tree, root); */
  /* TREE_CHECK(tree); */
  ASSERT(root);
  ASSERT(root->type != MB_POP_NONE);
  pivot = root->left;
  ASSERT(pivot);
  /* ASSERT(pivot->type != MB_POP_NONE); */
  if (tree->root == root) {
    tree->root = pivot;
  } else {
    ASSERT(root->parent);
    if (root == root->parent->left) {
      root->parent->left = pivot;
    } else {
      ASSERT(root == root->parent->right);
      root->parent->right = pivot;
    }
  }
  pivot->parent = root->parent;
  if ((root->left = pivot->right))
    root->left->parent = root;
  pivot->right = root;
  root->parent = pivot;
  /* TREE_CHECK(tree); */
  /* root->parent = pivot; */
}



static void insert_case_1 ( MbsPopTree *tree,
                            MbsPopUnit *unit )
{
  /* CL_TRACE("tree=%p, unit=%p", tree, unit); */
  /* TREE_CHECK(tree); */
  if (!unit->parent)
    unit->red = 0;
  else
    insert_case_2(tree, unit);
}



static void insert_case_2 ( MbsPopTree *tree,
                            MbsPopUnit *unit )
{
  /* CL_TRACE("tree=%p, unit=%p", tree, unit); */
  /* TREE_CHECK(tree); */
  if (!unit->parent->red)
    return;
  else
    insert_case_3(tree, unit);
}



/* Note: In the following cases it can be assumed that N has a
 * grandparent node G, because its parent P is red, and if it were the
 * root, it would be black. Thus, N also has an uncle node U, although
 * it may be a leaf in cases 4 and 5.
 */
static void insert_case_3 ( MbsPopTree *tree,
                            MbsPopUnit *unit )
{
  MbsPopUnit *uncle = unit_uncle(unit);
  /* CL_TRACE("tree=%p, unit=%p", tree, unit); */
  /* TREE_CHECK(tree); */
  if (uncle && uncle->red)
    {
      MbsPopUnit *gp;
      unit->parent->red = 0;
      uncle->red = 0;
      gp = unit_grandparent(unit);
      gp->red = 1;
      insert_case_1(tree, gp);
    }
  else
    {
      insert_case_4(tree, unit);
    }
}



static void insert_case_4 ( MbsPopTree *tree,
                            MbsPopUnit *unit )
{
  MbsPopUnit *gp = unit_grandparent(unit);
  /* CL_TRACE("tree=%p, unit=%p", tree, unit); */
  /* TREE_CHECK(tree); */
  if (unit == unit->parent->right && unit->parent == gp->left)
    {
      rotate_left(tree, unit->parent);
      unit = unit->left;
    }
  else if (unit == unit->parent->left && unit->parent == gp->right)
    {
      rotate_right(tree, unit->parent);
      unit = unit->right;
    }
  insert_case_5(tree, unit);
}



static void insert_case_5 ( MbsPopTree *tree,
                            MbsPopUnit *unit )
{
  MbsPopUnit *gp = unit_grandparent(unit);
  /* CL_TRACE("tree=%p, unit=%p", tree, unit); */
  /* TREE_CHECK(tree); */
  unit->parent->red = 0;
  gp->red = 1;
  if (unit == unit->parent->left)
    rotate_right(tree, gp);
  else
    rotate_left(tree, gp);
}



static void insert_at ( MbsPopTree *tree,
                        MbsPopUnit *place,
                        MbsPopUnit *unit )
{
  ASSERT(place->type == MB_POP_NONE);
  ASSERT(!place->left);
  ASSERT(!place->right);
  ASSERT(!place->red);
  ASSERT(!unit->parent);
  ASSERT(!unit->left);
  ASSERT(!unit->right);
  unit->parent = place->parent;
  if (place == place->parent->left)
    {
      place->parent->left = unit;
    }
  else
    {
      ASSERT(place == place->parent->right);
      place->parent->right = unit;
    }
  place->parent = unit;
  unit->left = place;
  unit->right = mbs_pop_unit_new(MB_POP_NONE, 0, 0);
  unit->right->parent = unit;
  unit->red = 1;
  /* TREE_CHECK(tree); */
  insert_case_1(tree, unit);
  TREE_CHECK(tree);
}



/* mbs_pop_tree_add:
 */
void mbs_pop_tree_add ( MbsPopTree *tree,
                        MbPopType type,
                        guint birthdate,
                        gint64 count )
{
  MbsPopUnit *unit;
  tree->pop[type] += count;
  if ((unit = lookup_unit(tree, type, birthdate)))
    {
      if (unit->type == MB_POP_NONE)
        {
          MbsPopUnit *new_unit = mbs_pop_unit_new(type, birthdate, count);
          insert_at(tree, unit, new_unit);
          /* unit = new_unit; */
        }
      else
        {
          unit->count += count;
          /* [fixme] ?? */
          if (unit->task)
            mbs_task_adjust_workers(unit->task, count);
        }
    }
  else
    {
      unit = mbs_pop_unit_new(type, birthdate, count);
      insert_root(tree, unit);
    }
  /* if (unit->count == 0) */
  /*   CL_DEBUG("[TODO] remove unit"); */
  /* CL_DEBUG("tree_add(%p, %d, %d, %" G_GINT64_FORMAT ")", */
  /*          tree, type, birthdate, count); */
  /* TREE_CHECK(tree); */
}



#define TRAVERSE_STACK_SIZE 32



/* mbs_pop_tree_traverse:
 */
void mbs_pop_tree_traverse ( MbsPopTree *tree,
                             MbsPopTreeTraverseFunc func,
                             gpointer data )
{
  MbsPopUnit *stack[TRAVERSE_STACK_SIZE];
  guint stack_size;
  MbsPopUnit *node;
  if (!tree->root)
    return;
  node = tree->root;
  stack_size = 0;
  while (stack_size > 0 || node->type != MB_POP_NONE)
    {
      if (node->type != MB_POP_NONE)
        {
          ASSERT(stack_size < TRAVERSE_STACK_SIZE);
          stack[stack_size++] = node;
          node = node->left;
        }
      else
        {
          ASSERT(stack_size > 0);
          node = stack[--stack_size];
          func(node, data);
          node = node->right;
        }
    }
}



struct update_data
{
  MbsPopTree *dest;
};



void _update ( MbsPopUnit *unit,
               gpointer data_ )
{
  struct update_data *data = data_;
  ASSERT(!unit->task);
  mbs_pop_tree_add(data->dest, unit->type, unit->birthdate, unit->count);
}



/* mbs_pop_tree_update:
 */
void mbs_pop_tree_update ( MbsPopTree *tree,
                           MbsPopTree *adj )
{
  struct update_data data;
  data.dest = tree;
  mbs_pop_tree_traverse(adj, _update, &data);
  mbs_pop_tree_clear(adj);
}



/* mbs_pop_unit_affect_task:
 */
void mbs_pop_unit_affect_task ( MbsPopUnit *unit,
                                MbsTask *task )
{
  if (unit->task == task)
    return;
  if (unit->task)
    mbs_task_adjust_workers(unit->task, -unit->count);
  if ((unit->task = task))
    mbs_task_adjust_workers(task, unit->count);
}
