/* mbspoptree.c -
 */

#include "server/srvprivate.h"
#include "server/mbspoptree.h"
#include "server/mbstask.h"



/* TrashItem:
 */
typedef struct _TrashItem TrashItem;
struct _TrashItem
{
  TrashItem *next;
};



/* Node:
 */
typedef struct _Node Node;
struct _Node
{
  MbsPopUnit *unit;
  Node *parent;
  Node *left;
  Node *right;
  guint red : 1;
};



/* MbsPopTree:
 */
struct _MbsPopTree
{
  gint64 *pop;
  Node *root;
};



/* nodes */
#define ISLEAF(node)   ((node)->unit == NULL)
#define ISRED(node)    ((node)->red)
#define ISBLACK(node)  (!(node)->red)
#define SETRED(node)   ((node)->red = 1)
#define SETBLACK(node) ((node)->red = 0)
/* note: these don't check parent so make sure there _is_ a parent
   before calling them */
#define ISLEFT(node)  ((node) == (node)->parent->left)
#define ISRIGHT(node) ((node) == (node)->parent->right)

#define NODE_STACK_SIZE 32 /* used for traversal */

static inline Node *node_new ( void );
static inline void node_free ( Node *node );
static inline gint node_check ( Node *node,
                                MbPopType type,
                                guint birthdate );
static inline gboolean node_lookup ( MbsPopTree *tree,
                                     MbPopType type,
                                     guint birthdate,
                                     Node **node );
static inline void node_insert ( MbsPopTree *tree,
                                 Node *node,
                                 MbsPopUnit *unit );
static inline Node *node_grandparent ( Node *node );
static inline Node *node_uncle ( Node *node );
static inline Node *node_sibling ( Node *node );
static inline void node_rotate_right ( MbsPopTree *tree,
                                       Node *node );
static inline void node_rotate_left ( MbsPopTree *tree,
                                      Node *node );
static void insert_case_1 ( MbsPopTree *tree,
                            Node *node );
static void insert_case_2 ( MbsPopTree *tree,
                            Node *node );
static void insert_case_3 ( MbsPopTree *tree,
                            Node *node );
static void insert_case_4 ( MbsPopTree *tree,
                            Node *node );
static void insert_case_5 ( MbsPopTree *tree,
                            Node *node );
static inline void node_remove ( MbsPopTree *tree,
                                 Node *node );
static inline Node *node_remove_subst ( MbsPopTree *tree,
                                        Node *node );
static void node_remove_case_1 ( MbsPopTree *tree,
                                 Node *node );
static void node_remove_case_2 ( MbsPopTree *tree,
                                 Node *node );
static void node_remove_case_3 ( MbsPopTree *tree,
                                 Node *node );
static void node_remove_case_4 ( MbsPopTree *tree,
                                 Node *node );
static void node_remove_case_5 ( MbsPopTree *tree,
                                 Node *node );
static void node_remove_case_6 ( MbsPopTree *tree,
                                 Node *node );

static TrashItem *free_node = NULL;

/* units */
static inline MbsPopUnit *unit_new ( MbPopType type,
                                     guint birthdate,
                                     gint64 count );
static inline void unit_free ( MbsPopUnit *unit );

static TrashItem *free_unit = NULL;



/********************************************************************/
/*                                                                  */
/*                            PUBLIC API                            */
/*                                                                  */
/********************************************************************/


/* mbs_pop_tree_new:
 */
MbsPopTree *mbs_pop_tree_new ( gint64 *pop )
{
  MbsPopTree *tree;
  tree = g_slice_alloc0(sizeof(MbsPopTree));
  tree->root = node_new();
  tree->pop = pop;
  return tree;
}



/* check_data:
 */
struct check_data
{
  gint64 pop[MB_POP_TYPE_COUNT];
};



/* _check_node:
 */
static void _check_node ( Node *node,
                          Node *parent,
                          struct check_data *data )
{
  ASSERT(node->parent == parent);
  if (ISLEAF(node))
    {
      ASSERT(!node->left);
      ASSERT(!node->right);
    }
  else
    {
      ASSERT(node->left);
      ASSERT(node->right);
      if (parent)
        {
          ASSERT(ISLEFT(node) ^ ISRIGHT(node));
          if (ISLEFT(node))
            ASSERT(node_check(node, parent->unit->type, parent->unit->birthdate) > 0);
          else
            ASSERT(node_check(node, parent->unit->type, parent->unit->birthdate) < 0);
        }
      _check_node(node->left, node, data);
      _check_node(node->right, node, data);
    }
}



/* mbs_pop_tree_check:
 */
void mbs_pop_tree_check ( MbsPopTree *tree )
{
  struct check_data data;
  _check_node(tree->root, NULL, &data);
}



/* _clear:
 */
static void _clear ( Node *node )
{
  if (node->left)
    _clear(node->left);
  if (node->right)
    _clear(node->right);
  if (!ISLEAF(node))
    {
      ASSERT(!node->unit->task); /* ?? */
      unit_free(node->unit);
    }
  node_free(node);
}



/* mbs_pop_tree_clear:
 */
void mbs_pop_tree_clear ( MbsPopTree *tree )
{
  _clear(tree->root);
  tree->root = node_new();
  memset(tree->pop, 0, sizeof(gint64) * MB_POP_TYPE_COUNT);
}



/* mbs_pop_tree_add:
 */
void mbs_pop_tree_add ( MbsPopTree *tree,
                        MbPopType type,
                        guint birthdate,
                        gint64 count )
{
  Node *node;
  if (count == 0)
    return;
  tree->pop[type] += count;
  if (node_lookup(tree, type, birthdate, &node))
    {
      ASSERT(node->unit);
      node->unit->count += count;
      if (node->unit->task)
        mb_task_add_workers(MB_TASK(node->unit->task), count);
      if (node->unit->count == 0) {
        mbs_pop_unit_affect_task(node->unit, NULL);
        node_remove(tree, node);
      }
    }
  else
    {
      MbsPopUnit *unit;
      unit = unit_new(type, birthdate, count);
      node_insert(tree, node, unit);
    }
  MBS_POP_TREE_CHECK(tree);
}



/* mbs_pop_tree_traverse:
 */
void mbs_pop_tree_traverse ( MbsPopTree *tree,
                             MbsPopTreeTraverseFunc func,
                             gpointer data )
{
  Node *stack[NODE_STACK_SIZE];
  guint stack_size = 0;
  Node *node = tree->root;
  while (stack_size > 0 || !ISLEAF(node))
    {
      if (!ISLEAF(node))
        {
          ASSERT(stack_size < NODE_STACK_SIZE);
          stack[stack_size++] = node;
          node = node->left;
        }
      else
        {
          ASSERT(stack_size > 0);
          node = stack[--stack_size];
          func(node->unit, data);
          node = node->right;
        }
    }
}



/* update_data:
 */
struct update_data
{
  MbsPopTree *dest;
};



/* _traverse_update:
 */
static void _traverse_update ( MbsPopUnit *unit,
                               gpointer data )
{
  ASSERT(!unit->task); /* ?? */
  mbs_pop_tree_add(((struct update_data *) data)->dest,
                   unit->type,
                   unit->birthdate,
                   unit->count);
}



/* mbs_pop_tree_update:
 */
void mbs_pop_tree_update ( MbsPopTree *tree,
                           MbsPopTree *adj )
{
  struct update_data data;
  data.dest = tree;
  mbs_pop_tree_traverse(adj, _traverse_update, &data);
  /* [FIXME] this should not be here !? */
  mbs_pop_tree_clear(adj);
}



/* mbs_pop_tree_get_pop:
 */
void mbs_pop_tree_get_pop ( MbsPopTree *tree,
                            gint64 *pop )
{
  memcpy(pop, tree->pop, sizeof(gint64) * MB_POP_TYPE_COUNT);
}



/* mbs_pop_unit_affect_task:
 */
void mbs_pop_unit_affect_task ( MbsPopUnit *unit,
                                MbObject *task )
{
  ASSERT((!task) || MBS_IS_TASK(task));
  if (task == unit->task)
    return;
  if (unit->task) {
    mb_task_add_workers(MB_TASK(unit->task), -unit->count);
    L_OBJECT_CLEAR(unit->task);
  }
  if (task) {
    unit->task = l_object_ref(task);
    mb_task_add_workers(MB_TASK(unit->task), unit->count);
  }
}



/********************************************************************/
/*                                                                  */
/*                              UNITS                               */
/*                                                                  */
/********************************************************************/



/* unit_new:
 */
static inline MbsPopUnit *unit_new ( MbPopType type,
                                     guint birthdate,
                                     gint64 count )
{
  MbsPopUnit *unit;
  if (free_unit)
    {
      unit = (MbsPopUnit *) free_unit;
      free_unit = ((TrashItem *) unit)->next;
    }
  else
    {
      unit = g_new(MbsPopUnit, 1);
    }
  unit->type = type;
  unit->birthdate = birthdate;
  unit->count = count;
  unit->task = NULL;
  return unit;
}


  
/* unit_free:
 */
static inline void unit_free ( MbsPopUnit *unit )
{
  ASSERT(!unit->task); /* ?? */
  ((TrashItem *) unit)->next = free_unit;
  free_unit = (TrashItem *) unit;
}



/********************************************************************/
/*                                                                  */
/*                              NODES                               */
/*                                                                  */
/********************************************************************/



/* node_new:
 */
static inline Node *node_new ( void )
{
  if (free_node)
    {
      TrashItem *node = free_node;
      free_node = node->next;
      memset(node, 0, sizeof(Node));
      return (Node *) node;
    }
  else
    {
      return g_new0(Node, 1);
    }
}



/* node_free:
 */
static inline void node_free ( Node *node )
{
  ((TrashItem *) node)->next = free_node;
  free_node = (TrashItem *) node;
}



/* node_check:
 */
static inline gint node_check ( Node *node,
                                MbPopType type,
                                guint birthdate )
{
  ASSERT(!ISLEAF(node));
  if (type < node->unit->type)
    return -1;
  else if (type > node->unit->type)
    return 1;
  else if (birthdate < node->unit->birthdate)
    return -1;
  else if (birthdate > node->unit->birthdate)
    return 1;
  else
    return 0;
}



/* node_lookup:
 */
static inline gboolean node_lookup ( MbsPopTree *tree,
                                     MbPopType type,
                                     guint birthdate,
                                     Node **node )
{
  Node *n = tree->root;
  while (!ISLEAF(n))
    {
      gint d = node_check(n, type, birthdate);
      if (d < 0) {
        n = n->left;
      } else if (d > 0) {
        n = n->right;
      } else {
        *node = n;
        return TRUE;
      }
    }
  *node = n;
  return FALSE;
}



/* node_rotate_right:
 */
static inline void node_rotate_right ( MbsPopTree *tree,
                                       Node *root )
{
  Node *pivot;
  ASSERT(root);
  ASSERT(!ISLEAF(root));
  pivot = root->left;
  ASSERT(pivot);
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
}



/* node_rotate_left:
 */
static inline void node_rotate_left ( MbsPopTree *tree,
                                      Node *root )
{
  Node *pivot;
  ASSERT(root);
  ASSERT(!ISLEAF(root)); /* ?? */
  pivot = root->right;
  ASSERT(pivot);
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
}



/* node_insert:
 */
static inline void node_insert ( MbsPopTree *tree,
                                 Node *node,
                                 MbsPopUnit *unit )
{
  ASSERT(ISLEAF(node));
  ASSERT(!(node->parent && ISLEAF(node->parent)));
  node->unit = unit;
  if (!node->left) {
    node->left = node_new();
    node->left->parent = node;
  }
  if (!node->right) {
    node->right = node_new();
    node->right->parent = node;
  }
  SETRED(node); /* [FIXME] not sure */
  insert_case_1(tree, node);
}



/* node_grandparent:
 */
static inline Node *node_grandparent ( Node *node )
{
  if (node && node->parent)
    return node->parent->parent;
  else
    return NULL;
}



/* node_uncle:
 */
static inline Node *node_uncle ( Node *node )
{
  Node *g = node_grandparent(node);
  if (!g)
    return NULL;
  if (node->parent == g->left)
    return g->right;
  else
    return g->left;
}



/* node_sibling:
 */
static inline Node *node_sibling ( Node *node )
{
  if (ISLEFT(node))
    return node->parent->right;
  else
    return node->parent->left;
}



/********************************************************************/
/*                                                                  */
/*                            INSERTION                             */
/*                                                                  */
/********************************************************************/



/* insert_case_1:
 */
static void insert_case_1 ( MbsPopTree *tree,
                            Node *node )
{
  ASSERT(ISRED(node)); /* [fixme] not sure */
  if (!node->parent)
    {
      ASSERT(node == tree->root);
      SETBLACK(node);
    }
  else
    {
      insert_case_2(tree, node);
    }
}



/* insert_case_2:
 */
static void insert_case_2 ( MbsPopTree *tree,
                            Node *node )
{
  if (ISBLACK(node->parent))
    return;
  else
    insert_case_3(tree, node);
}



/* insert_case_3:
 */
static void insert_case_3 ( MbsPopTree *tree,
                            Node *node )
{
  Node *uncle = node_uncle(node);
  if (uncle && ISRED(uncle))
    {
      Node *gp;
      SETBLACK(node->parent);
      SETBLACK(uncle);
      gp = node_grandparent(node);
      ASSERT(gp);
      SETRED(gp);
      insert_case_1(tree, gp);
    }
  else
    {
      insert_case_4(tree, node);
    }
}



/* insert_case_4:
 */
static void insert_case_4 ( MbsPopTree *tree,
                            Node *node )
{
  /* Node *gp = node_grandparent(node); */
  if (ISRIGHT(node) && ISLEFT(node->parent))
    {
      node_rotate_left(tree, node->parent);
      node = node->left;
    }
  else if (ISLEFT(node) && ISRIGHT(node->parent))
    {
      node_rotate_right(tree, node->parent);
      node = node->right;
    }
  insert_case_5(tree, node);
}



/* insert_case_5:
 */
static void insert_case_5 ( MbsPopTree *tree,
                            Node *node )
{
  Node *gp = node_grandparent(node);
  SETBLACK(node->parent);
  SETRED(gp);
  if (ISLEFT(node))
    node_rotate_right(tree, gp);
  else
    node_rotate_left(tree, gp);
}



/********************************************************************/
/*                                                                  */
/*                             DELETION                             */
/*                                                                  */
/********************************************************************/



/* node_replace:
 */
static inline void node_replace ( MbsPopTree *tree,
                                  Node *node,
                                  Node *child )
{
  ASSERT(child->parent == node);
  if (node->parent)
    {
      child->parent = node->parent;
      if (ISLEFT(node))
        child->parent->left = child;
      else
        child->parent->right = child;
    }
  else
    {
      ASSERT(tree->root == node);
      tree->root = child;
      child->parent = NULL;
    }
}



/* node_remove:
 */
static inline void node_remove ( MbsPopTree *tree,
                                 Node *node )
{
  Node *child;
  ASSERT(!ISLEAF(node));
  if (!(ISLEAF(node->left) || ISLEAF(node->right)))
    {
      node = node_remove_subst(tree, node);
    }
  /* delete_one_child */
  child = ISLEAF(node->right) ? node->left : node->right;
  node_replace(tree, node, child);
  /* go */
  if (ISBLACK(node))
    {
      if (ISRED(child))
        SETBLACK(child);
      else
        node_remove_case_1(tree, child);
    }
  /* free node */
  unit_free(node->unit);
  if (node->left == child)
    {
      ASSERT(ISLEAF(node->right));
      node_free(node->right);
    }
  else
    {
      ASSERT(ISLEAF(node->left));
      node_free(node->left);
    }
  node_free(node);
}



/* node_remove_subst:
 */
static inline Node *node_remove_subst ( MbsPopTree *tree,
                                        Node *node )
{
  Node *max;
  MbsPopUnit *unit;
  ASSERT(!ISLEAF(node->left));
  for (max = node->left; !ISLEAF(max->right); max = max->right);
  unit = max->unit;
  max->unit = node->unit;
  node->unit = unit;
  return max;
}



/* node_remove_case_1:
 */
static void node_remove_case_1 ( MbsPopTree *tree,
                                 Node *node )
{
  if (node->parent)
    node_remove_case_2(tree, node);
}



/* node_remove_case_2:
 */
static void node_remove_case_2 ( MbsPopTree *tree,
                                 Node *node )
{
  Node *s = node_sibling(node);
  if (ISRED(s))
    {
      SETRED(node->parent);
      SETBLACK(s);
      if (ISLEFT(node))
        node_rotate_left(tree, node->parent);
      else
        node_rotate_right(tree, node->parent);
    }
  node_remove_case_3(tree, node);
}



/* node_remove_case_3:
 */
static void node_remove_case_3 ( MbsPopTree *tree,
                                 Node *node )
{
  Node *s = node_sibling(node);
  if (ISBLACK(node->parent) &&
      ISBLACK(s) &&
      ISBLACK(s->left) &&
      ISBLACK(s->right))
    {
      SETRED(s);
      node_remove_case_1(tree, node->parent);
    }
  else
    {
      node_remove_case_4(tree, node);
    }
}



/* node_remove_case_4:
 */
static void node_remove_case_4 ( MbsPopTree *tree,
                                 Node *node )
{
  Node *s = node_sibling(node);
  if (ISRED(node->parent) &&
      ISBLACK(s) &&
      ISBLACK(s->left) &&
      ISBLACK(s->right))
    {
      SETRED(s);
      SETBLACK(node->parent);
    }
  else
    {
      node_remove_case_5(tree, node);
    }
}



/* node_remove_case_5:
 */
static void node_remove_case_5 ( MbsPopTree *tree,
                                 Node *node )
{
  Node *s = node_sibling(node);
  /* [FIXME] trivial test ? */
  if (ISBLACK(s))
    {
      if (ISLEFT(node) && ISBLACK(s->right) && ISRED(s->left))
        {
          SETRED(s);
          SETBLACK(s->left);
          node_rotate_right(tree, s);
        }
      else if (ISRIGHT(node) && ISBLACK(s->left) && ISRED(s->right))
        {
          SETRED(s);
          SETBLACK(s->right);
          node_rotate_left(tree, s);
        }
    }
  node_remove_case_6(tree, node);
}



/* node_remove_case_6:
 */
static void node_remove_case_6 ( MbsPopTree *tree,
                                 Node *node )
{
  Node *s = node_sibling(node);
  s->red = node->parent->red;
  SETBLACK(node->parent);
  if (ISLEFT(node))
    {
      SETBLACK(s->right);
      node_rotate_left(tree, node->parent);
    }
  else
    {
      SETBLACK(s->left);
      node_rotate_right(tree, node->parent);
    }
}
