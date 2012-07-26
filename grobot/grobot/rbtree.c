#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define    RB_RED       0
#define    RB_BLACK     1



/**
 * 定义自己的entry，first是key，second是value
 */
struct my_data {
    int first, second;
};

/*定义自己的数据的比较函数*/
static int cmp(struct my_data *left, struct my_data *right) {
    return left->first - right->first;
}

struct _tag_rb_node {
    /*在rb_node中添加自己的数据*/
    struct my_data key;
    struct _tag_rb_node *rb_parent;
    int rb_color;
    struct _tag_rb_node *rb_right;
    struct _tag_rb_node *rb_left;

    /* used for select and rank function.
     * when a new rb_node is initialized, weight should be 1.
     */
    int weight;
};
typedef struct _tag_rb_node rb_node;

struct _tag_rb_root {
    struct _tag_rb_node *rb_node;
    int size;
};
typedef struct _tag_rb_root rb_root;


static void __rb_rotate_left(rb_node *node, rb_root *root) {
    int a, b, c;
    rb_node *right;

    a=b=c=0;
    right = node->rb_right;
    if (node->rb_left) {
        a = node->rb_left->weight;
    }
    if (right->rb_left) {
        b = right->rb_left->weight;
    }
    if (right->rb_right) {
        c = right->rb_right->weight;
    }

    node->weight = a + b + 1;
    right->weight = a + b + c + 2;
    if ((node->rb_right = right->rb_left)) {
        right->rb_left->rb_parent = node;
    }

    right->rb_left = node;
    if ((right->rb_parent = node->rb_parent)) {
        if (node == node->rb_parent->rb_left) {
            node->rb_parent->rb_left = right;
        }
        else {
            node->rb_parent->rb_right = right;
        }
    }
    else {
        root->rb_node = right;
    }
    node->rb_parent = right;
}

static void __rb_rotate_right(rb_node *node, rb_root *root) {
    int a, b, c;
    rb_node *left;

    left = node->rb_left;
    if (node->rb_right) {
        c = node->rb_right->weight;
    }
    if (left->rb_left) {
        a = left->rb_left->weight;
    }
    if (left->rb_right) {
        b = left->rb_right->weight;
    }

    node->weight = b + c + 1;
    left->weight = a + b + c + 2;
    if ((node->rb_left = left->rb_right)) {
        left->rb_right->rb_parent = node;
    }

    left->rb_right = node;
    if ((left->rb_parent = node->rb_parent)) {
        if (node == node->rb_parent->rb_right) {
            node->rb_parent->rb_right = left;
        }
        else {
            node->rb_parent->rb_left = left;
        }
    }
    else {
        root->rb_node = left;
    }
    node->rb_parent = left;
}

void rb_insert_color(rb_node *node, rb_root *root) {
    register rb_node *uncle, *tmp;
    rb_node *parent, *gparent;

    while ((parent = node->rb_parent) && parent->rb_color == RB_RED) {
        gparent = parent->rb_parent;
        if (parent == gparent->rb_left) {
            uncle = gparent->rb_right;
            if (uncle && uncle->rb_color == RB_RED) {
                uncle->rb_color = RB_BLACK;
                parent->rb_color = RB_BLACK;
                gparent->rb_color = RB_RED;
                node = gparent;
                continue;
            }
            if (parent->rb_right == node) {
                __rb_rotate_left(parent, root);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            parent->rb_color = RB_BLACK;
            gparent->rb_color = RB_RED;
            __rb_rotate_right(gparent, root);
        }
        else {
            uncle = gparent->rb_left;
            if (uncle && uncle->rb_color == RB_RED) {
                uncle->rb_color = RB_BLACK;
                parent->rb_color = RB_BLACK;
                gparent->rb_color = RB_RED;
                node = gparent;
                continue;
            }
            if (parent->rb_left == node) {
                __rb_rotate_right(parent, root);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            parent->rb_color = RB_BLACK;
            gparent->rb_color = RB_RED;
            __rb_rotate_left(gparent, root);
        }
    }
    root->rb_node->rb_color = RB_BLACK;
}
static void __rb_erase_color(rb_node *node, rb_node *parent, rb_root *root) {
    rb_node *other;
    register rb_node *o_left, o_right;

    while ((!node || node->rb_color == RB_BLACK) && node != root->rb_node) {
        if (parent->rb_left == node) {
            other = parent->rb_right;
            if (other->rb_color == RB_RED) {
                other->rb_color = RB_BLACK;
                parent->rb_color = RB_RED;
                __rb_rotate_left(parent, root);
                other = parent->rb_right;
            }
            if ((!other->rb_left || other->rb_left->rb_color == RB_BLACK) && 
                (!other->rb_right|| other->rb_right->rb_color == RB_BLACK)) {
                other->rb_color = RB_RED;
                node = parent;
                parent = node->rb_parent;
            }
            else {
                if (!other->rb_right || other->rb_right->rb_color == RB_BLACK) {
                    if ((o_left = other->rb_left)) {
                        o_left->rb_color = RB_BLACK;
                    }
                    other->rb_color = RB_RED;
                    __rb_rotate_right(other, root);
                    other = parent->rb_right;
                }
                other->rb_color = parent->rb_color;
                parent->rb_color = RB_BLACK;
                if (other->rb_right) {
                    other->rb_right->rb_color = RB_BLACK;
                }
                __rb_rotate_left(parent, root);
                node = root->rb_node;
                break;
            }
        }
        else {
            other = parent->rb_left;
            if (other->rb_color == RB_RED) {
                other->rb_color = RB_BLACK;
                parent->rb_color = RB_RED;
                __rb_rotate_right(parent, root);
                other = parent->rb_left;
            }
            if ((!other->rb_left || other->rb_left->rb_color == RB_BLACK) &&
                (!other->rb_right || other->rb_right->rb_color == RB_BLACK)) {
                other->rb_color = RB_RED;
                node = parent;
                parent = node->rb_parent;
            }
            else {
                if (!other->rb_left || other->rb_left->rb_color == RB_BLACK) {
                    if ((o_right = other->rb_right)) {
                        o_right->rb_color = RB_BLACK;
                    }
                    other->rb_color = RB_RED;
                    __rb_rotate_left(other, root);
                    other = parent->rb_left;
                }
                other->rb_color = parent->rb_color;
                parent->rb_color = RB_BLACK;
                if (other->rb_left)
                    other->rb_left->rb_color = RB_BLACK;
                __rb_rotate_right(parent, root);
                node = root->rb_node;
                break;
            }
        }
    }
    if (node) {
        node->rb_color = RB_BLACK;
    }
}

void rb_erase_full_child(rb_node *node, rb_root *root) {
    int color;
    rb_node *old, *left, *child, *parent, *q;

    old = node;
    node = node->rb_right;

    while ((left = node->rb_left) != NULL) {
        node = left;
    }
    child = node->rb_right;
    parent = node->rb_parent;
    color = node->rb_color;
    if (child) {
        child->rb_parent = parent;
    }
    if (parent) {
        if (parent->rb_left == node) {
            parent->rb_left = child;
        }
        else {
            parent->rb_right = child;
        }
    }
    else {
        root->rb_node = child;
    }
    if (node->rb_parent == old) {
        parent = node;
    }

    q = parent;
    while (q) {
        q->weight--;
        q = q->rb_parent;
    }

    node->rb_parent = old->rb_parent;
    node->rb_color = old->rb_color;
    node->weight = old->weight;
    node->rb_right = old->rb_right;
    node->rb_left = old->rb_left;
    if (old->rb_parent) {
        if (old->rb_parent->rb_left == old) {
            old->rb_parent->rb_left = node;
        }
        else {
            old->rb_parent->rb_right = node;
        }
    }
    else {
        root->rb_node = node;
    }

    old->rb_left->rb_parent = node;
    if (old->rb_right) {
        old->rb_right->rb_parent = node;
    }
}
void rb_erase_one_child(rb_node *node, rb_root *root, rb_node *child) {
    rb_node *parent, *q;
    int color;

    parent = node->rb_parent;
    color = node->rb_color;
    if (child) {
        child->rb_parent = parent;
    }
    if (parent) {
        q = parent;
        while (q) {
            q->weight--;
            q = q->rb_parent;
        }
        if (parent->rb_left == node) {
            parent->rb_left = child;
        }
        else {
            parent->rb_right = child;
        }
    }
    else {
        root->rb_node = child;
    }
}

void rb_erase(rb_node *node, rb_root *root) {
    int color;
    rb_node *child, *parent;
    rb_node *old, *left;
    rb_node *q;

    root->size--;
    if (!node->rb_left) {
        child = node->rb_right;
        rb_erase_one_child(node, root, child);
    }
    else if (!node->rb_right) {
        child = node->rb_left;
        rb_erase_one_child(node, root, child);
    }
    else {
        rb_erase_full_child(node, root);
    }

    if (color == RB_BLACK) {
        __rb_erase_color(child, parent, root);
    }
}


/*
 * This function returns the first node (in sort order) of the tree.
 */
rb_node *rb_first(rb_root *root) {
    rb_node *n;
    n = root->rb_node;
    if (!n) {
        return NULL;
    }
    while (n->rb_left) {
        n = n->rb_left;
    }
    return n;
}

rb_node *rb_last(rb_root *root) {
    struct rb_node *n;
    n = root->rb_node;
    if (!n)
        return NULL;
    while (n->rb_right)
        n = n->rb_right;
    return n;
}

rb_node *rb_next(rb_node *node) {

    /* If we have a right-hand child, go down and then left as far
       as we can. */
    if (node->rb_right) {
        node = node->rb_right;
        while (node->rb_left)
            node = node->rb_left;
        return node;
    }

    /* No right-hand children. Everything down and left is
       smaller than us, so any 'next' node must be in the general
       direction of our parent. Go up the tree; any time the
       ancestor is a right-hand child of its parent, keep going
       up. First time it's a left-hand child of its parent, said
       parent is our 'next' node. */
    while (node->rb_parent && node == node->rb_parent->rb_right)
        node = node->rb_parent;
    return node->rb_parent;
}

rb_node *rb_prev(rb_node *node) {

    /* If we have a left-hand child, go down and then right as far
       as we can. */
    if (node->rb_left) {
        node = node->rb_left;
        while (node->rb_right)
            node = node->rb_right;
        return node;
    }

    /* No left-hand children. Go up till we find an ancestor which
       is a right-hand child of its parent */
    while (node->rb_parent && node == node->rb_parent->rb_left)
        node = node->rb_parent;
    return node->rb_parent;
}

int my_rb_insert(rb_node *t, rb_root *root) {
    rb_node **p = &root->rb_node;
    rb_node *parent = NULL;
    int sig;
    while (*p) {
        parent = *p;
        parent->weight++;
        sig = cmp(&t->key, &parent->key);
        if (sig < 0)
            p = &(parent)->rb_left;

        else if (sig > 0)
            p = &(parent)->rb_right;

        else {
            while (parent) {
                parent->weight--;
                parent = parent->rb_parent;
            }
            return 1;
        }
    }
    root->size++;
    t->rb_parent = parent;
    t->rb_color = RB_RED;
    t->rb_left = t->rb_right = NULL;
    t->weight = 1;
    *p = t;
    rb_insert_color(t, root);
    return 0;
}

static rb_node *my_rb_find(my_data *key, rb_root *root) {
    rb_node *n = root->rb_node;
    int sig;
    while (n) {
        sig = cmp(key, &n->key);
        if (sig < 0)
            n = n->rb_left;

        else if (sig > 0)
            n = n->rb_right;

        else
            return n;
    }
    return NULL;
}


/*order base is from 1 to root->size*/
static rb_node *select(int order, rb_root *root) {
    if (order <= 0 || order > root->size)
        return NULL;
    rb_node *cn = root->rb_node;
    int ls = 0;
    while (cn) {
        if (cn->rb_left)
            ls = cn->rb_left->weight;

        else
            ls = 0;
        if (ls + 1 == order)
            return cn;

        else if (ls + 1 > order) {
            cn = cn->rb_left;
        } else {
            order -= ls + 1;
            cn = cn->rb_right;
        }
    }
    return NULL;
}


/*return a integer which is the number of rb_node->key <= w->key */
static int rank(my_data *w, rb_root *root) {
    if (!root->rb_node)
        return 0;
    int res = 0;
    rb_node *cn = root->rb_node;
    int sig;
    while (cn) {
        sig = cmp(&cn->key, w);
        if (sig == 0)
            return res + 1 + (cn->rb_left ? cn->rb_left->weight : 0);

        else if (sig < 0) {
            res += 1 + (cn->rb_left ? cn->rb_left->weight : 0);
            cn = cn->rb_right;
        } else {
            cn = cn->rb_left;
        }
    }
    return res;
}


