/*
 * JOOS is Copyright (C) 1997 Laurie Hendren & Michael I. Schwartzbach
 *
 * Reproduction of all or part of this software is permitted for
 * educational or research use on condition that this copyright notice is
 * included in any copy. This software comes with no warranty of any
 * kind. In no event will the authors be liable for any damages resulting from
 * use of this software.
 *
 * email: hendren@cs.mcgill.ca, mis@brics.dk
 */

/*****************************************************
 * Patterns                                          *
 *                                                   *
 * Group 05: Ethan Macdonald, Teng Long, Hardik Vala *
 *****************************************************/


/*
 * L: (Dead)
 * --------->
 *
 */
int drop_dead_label(CODE **c)
{ int l;
  if (is_label(*c, &l) && deadlabel(l)) {
    return replace(c, 1, NULL);
  }

  return 0;
}

/* 
 * ifeq L1
 * {ldc 0, iconst_0}
 * goto L2
 * L1:
 * {ldc 1, iconst_1}
 * L2:
 * ifeq L3
 * --------->
 * ifneq L3   (L1 & L2 must be unique)
 */
int simplify_consec_ifeqs(CODE **c)
{ int l1, l2, l3, l4, l5, x1, x2;
  if (is_ifeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEifne(l5, NULL));
  }

  return 0;
}

/* 
 * if_icmplt L1
 * {ldc 0, iconst_0}
 * goto L2
 * L1:
 * {ldc 1, iconst_1}
 * L2:
 * ifeq L3
 * --------->
 * ifneq L3   (L1 & L2 must be unique)
 */
/* TODO: Add this pattern for other if-constructs. */
int simplify_consec_if_icmplt_and_ifeq(CODE **c)
{ int l1, l2, l3, l4, l5, x1, x2;
  if (is_if_icmplt(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_icmpge(l5, NULL));
  }

  return 0;
}

/*
 * {ldc 0, iconst_0}
 * if_icmpeq L
 * --------->
 * ifeq L
 */
int simplify_icmpeq_zero(CODE **c)
{ int k, l;
  if (is_ldc_int(*c, &k) && k == 0 && is_if_icmpeq(next(*c), &l)) {
    return replace(c, 2, makeCODEifeq(l, NULL));
  }

  return 0;
}

/*
 * {ldc 0, iconst_0}
 * if_icmpneq L
 * --------->
 * ifneq L
 */
int simplify_icmpne_zero(CODE **c)
{ int k, l;
  if (is_ldc_int(*c, &k) && k == 0 && is_if_icmpne(next(*c), &l)) {
    return replace(c, 2, makeCODEifne(l, NULL));
  }

  return 0;
}

/*
 * {return, areturn, ireturn}
 * ...
 * L2:
 * --------->
 * {return, areturn, ireturn}
 * L2:
 */
/* TODO: Generalize this pattern to handle any number of intervening
 * statements. */
int strip_after_return(CODE **c)
{ int l1, l2;
  /* return. */
  if (is_return(*c) &&
      !is_label(next(*c), &l1) &&
      is_label(nextby(*c, 2), &l2)) {
    return replace_modified(c, 3, makeCODEreturn(makeCODElabel(l2, NULL)));
  }

  /* areturn. */
  if (is_areturn(*c) &&
      !is_label(next(*c), &l1) &&
      is_label(nextby(*c, 2), &l2)) {
    return replace_modified(c, 3, makeCODEareturn(makeCODElabel(l2, NULL)));
  }

  /* ireturn. */
  if (is_ireturn(*c) &&
      !is_label(next(*c), &l1) &&
      is_label(nextby(*c, 2), &l2)) {
    return replace_modified(c, 3, makeCODEireturn(makeCODElabel(l2, NULL)));
  }

  return 0;
}

/*
 * {return, ireturn}
 * nop
 * -------->
 * {return, ireturn}  (Not areturn)
 */
int strip_nops_after_return(CODE **c)
{ if (is_return(*c) && is_nop(next(*c))) {
    return replace(c, 2, makeCODEreturn(NULL));
  }

  if (is_ireturn(*c) && is_nop(next(*c))) {
    return replace(c, 2, makeCODEireturn(NULL));
  }

  return 0;
}

int ldc_dup_ifnull(CODE **c)
{
  int x, y;
  char* s;
  if (is_ldc_int(*c, &x))
  {
    if (is_dup(next(*c)))
    {
      if (is_ifnull(next(next(*c)), &y))
      {
        return replace(c,3,makeCODEldc_int(x, NULL));
      }
    }
  }
  else if (is_ldc_string(*c, &s))
  {
    if (is_dup(next(*c)))
    {
      if (is_ifnull(next(next(*c)), &y))
      {
        return replace(c,3,makeCODEldc_string(s, NULL));
      }
    }
  }
  return 0;
}

int load_load_swap(CODE **c)
{
  int first, second;
  if (is_iload(*c, &first))
  {
    if (is_iload(next(*c), &second))
    {
      if (is_swap(next(next(*c))))
      {
        return replace(c,3, makeCODEiload(second, makeCODEiload(first, NULL)));
      }
    }
    else if(is_aload(next(*c), &second))
    {
      if (is_swap(next(next(*c))))
      {
        return replace(c,3,makeCODEaload(second, makeCODEiload(first, NULL)));
      }
    }
  }
  else if (is_aload(*c, &first))
  {
    if (is_iload(next(*c), &second))
    {
      if (is_swap(next(next(*c))))
      {
        return replace(c,3, makeCODEiload(second, makeCODEaload(first, NULL)));
      }
    }
    else if(is_aload(next(*c), &second))
    {
      if (is_swap(next(next(*c))))
      {
        return replace(c,3,makeCODEaload(second, makeCODEaload(first, NULL)));
      }
    }
  }
  if (is_iload(*c, &first))
  {
    if (is_iload(next(*c), &second))
    {
      if (is_swap(next(next(*c))))
      {
        return replace(c,3, makeCODEiload(second, makeCODEiload(first, NULL)));
      }
    }
    else if(is_aload(next(*c), &second))
    {
      if (is_swap(next(next(*c))))
      {
        return replace(c,3,makeCODEaload(second, makeCODEiload(first, NULL)));
      }
    }
  }
  else if (is_aload(*c, &first))
  {
    if (is_iload(next(*c), &second))
    {
      if (is_swap(next(next(*c))))
      {
        return replace(c,3, makeCODEiload(second, makeCODEaload(first, NULL)));
      }
    }
    else if(is_aload(next(*c), &second))
    {
      if (is_swap(next(next(*c))))
      {
        return replace(c,3,makeCODEaload(second, makeCODEaload(first, NULL)));
      }
    }
  }
  return 0;
}

int aconst_null_dup_ifeq(CODE **c)
{
  int y;
  if (is_aconst_null(*c))
  {
    if (is_dup(next(*c)) && (is_if_acmpeq(next(next(*c)), &y) || is_if_icmpeq(next(next(*c)), &y)))
    {
      return replace(c, 3, makeCODEgoto(y, NULL));
    }
  }
  return 0;
}

/*
 * ldc 0
 * iload x
 * iadd
 * ------>
 * iload x
 */

int simplify_addition_left(CODE **c) {
    int x, k;
    if (is_ldc_int(*c, &k) && is_iload(next(*c), &x) && is_iadd(next(next(*c)))) {
        if (k == 0) {
            return replace(c, 3, makeCODEiload(x, NULL));
        }
        return 0;
    }
    return 0;
}

/*
 * iload x
 * ldc 0
 * iadd
 * ------>
 * iload x
 */

int simplify_addition_right(CODE **c) {
    int x, k;
    if (is_iload(*c, &x) && is_ldc_int(next(*c), &k) && is_iadd(next(next(*c)))) {
        if (k == 0) {
            return replace(c, 3, makeCODEiload(x, NULL));
        }
        return 0;
    }
    return 0;
}

/*
 * iload x
 * idc 0
 * isub
 * ------>
 * iload x
 */

int simpify_subtraction_right(CODE **c) {
    int x, k;
    if (is_iload(*c, &x) && is_ldc_int(next(*c), &k) && is_isub(next(next(*c)))) {
        if (k == 0) {
            return replace(c, 3, makeCODEiload(x, NULL));
        }
        return 0;
    }
    return 0;
}

/* ldc 0            ldc 1           ldc 2
 * iload x          iload x         iload x
 * imul             imul            imul
 * ------>          ------>         ------>
 * ldc 0            iload x         iload x
 *                                  dup
 *                                  iadd
 */

int simplify_multiplication_left(CODE **c) {
    int x, k;
    if (is_ldc_int(*c, &k) && is_iload(next(*c), &x) && is_imul(next(next(*c)))) {
        if (k == 0){
            return replace(c, 3, makeCODEldc_int(0, NULL));
        } else if (k == 1) {
            return replace(c, 3, makeCODEiload(x, NULL));
        } else if (k == 2) {
            return replace(c, 3, makeCODEiload(x, makeCODEdup(makeCODEiadd(NULL))));
        }
        return 0;
    }
    return 0;
}

/* iload x
 * ldc 1
 * idiv
 * ------>
 * iload x
 */

int simplify_division_right(CODE **c) {
    int x, k;
    if (is_iload(*c, &x) && is_ldc_int(next(*c), &k) && is_idiv(next(next(*c)))) {
        if (k == 1) {
            return replace(c, 3, makeCODEiload(x, NULL));
        }
        return 0;
    }
    return 0;
}

/* Handle dup'ing and then compare equals branching */
int simplify_dup_cmpeq(CODE **c)
{
  int x,y;
  if (is_dup(*c))
  {
    if (is_if_acmpeq(next(*c),&x))
    {
      return replace_modified(c,2, makeCODEgoto(x, NULL));
    }
    else if (is_if_icmpeq(next(*c),&y))
    {
      return replace_modified(c,2, makeCODEgoto(y, NULL));
    }
  }
  return 0;
}

/* load 1
 * load 1
 * ------->
 * load 1
 * dup
 */
int replace_double_load_with_dup(CODE **c)
{
  int x,y;
  if (is_iload(*c, &x) && is_iload(next(*c), &y)) {
    if (x == y)
    {
      CODE* n = next(*c);
      return replace(&n, 1, makeCODEdup(NULL));
    }
  }
  else if (is_aload(*c, &x) && is_aload(next(*c), &y))
  {
    if (x == y)
    {
      CODE* n = next(*c);
      return replace(&n, 1, makeCODEdup(NULL));
    }
  }
  return 0;
}

/* remove any sort of loads followed by pops
 */
int rm_redundant_loads(CODE **c)
{
  int x;
  if (is_iload(*c, &x) && is_pop(next(*c)))
  {
    CODE *n = next(*c);
    kill_line(&n);
    return kill_line(c);
  }
  return 0;
}

/* iload 1
 * istore 1
 * -------->
 *
 */
 int rm_same_iload_istore(CODE **c)
 {
  int x,y;
  if (is_iload(*c,&x) && is_istore(next(*c),&y))
  {
    if (x==y)
    {
      CODE* n = next(*c);
      kill_line(&n);
      return(kill_line(c));
    }
  }
  return 0;
 }

 /* aload 1
 * astore 1
 * -------->
 *
 */
 int rm_same_aload_astore(CODE **c)
 {
  int x,y;
  if (is_aload(*c,&x) && is_astore(next(*c),&y))
  {
    if (x==y)
    {
      CODE* n = next(*c);
      kill_line(&n);
      return(kill_line(c));
    }
  }
  return 0;
 }

 /* dup
  * istore
  * pop
  * ----->
  * istore x
  */

 int simplify_istore(CODE **c)
 {
   int x;
   if (is_dup(*c) && is_istore(next(*c), &x) && is_pop(next(next(*c))))
   {
     return replace(c,3,makeCODEistore(x,NULL));
   }
   return 0;
 }

/* istore x
 * iload x
 * ------->
 * dup
 * istore x
 */

int simplify_istore_iload(CODE **c) {
    int x, y;
    if (is_istore(*c, &x) && is_iload(next(*c), &y)) {
        if (x == y) {
            return replace(c, 2, makeCODEdup(makeCODEistore(x, NULL)));
        }
        return 0;
    }
    return 0;
}

/* astore x
 * aload x
 * ------->
 * dup
 * astore x
 */

int simplify_astore_aload(CODE **c) {
    int x, y;
    if (is_astore(*c, &x) && is_aload(next(*c), &y)) {
        if (x == y) {
            return replace(c, 2, makeCODEdup(makeCODEastore(x, NULL)));
        }
        return 0;
    }
    return 0;
}

/* aload 0
 * .......
 * .......
 * .......
 * aload 0
 * .......
 * .......
 * .......
 * ------>
 * aload 0
 * dup
 * TODO: doesn't work
 */

int simplify_two_aload_0(CODE **c) {
    int x, y;
    char *arg_1, *arg_2, *arg_3, *arg_4, *arg_5, *arg_6;
    if (is_aload(*c, &x) && is_getfield(next(*c), &arg_1) &&
        is_ldc_string(next(next(*c)), &arg_2) && is_invokevirtual(next(next(next(*c))), &arg_3) &&
        is_aload(next(next(next(next(*c)))), &y) && is_getfield(next(next(next(next(next(*c))))), &arg_4) &&
        is_ldc_string(next(next(next(next(next(next(*c)))))), &arg_5) && is_invokevirtual(next(next(next(next(next(next(next(*c))))))), &arg_6)) {
        if (x == 0 && y == 0) {
            return replace(c, 8, makeCODEaload(x,
                                    makeCODEdup(
                                        makeCODEgetfield(arg_1,
                                        makeCODEldc_string(arg_2,
                                        makeCODEinvokevirtual(arg_3,
                                        makeCODEgetfield(arg_4,
                                        makeCODEldc_string(arg_5,
                                        makeCODEinvokevirtual(arg_6,
                                        NULL))))))
                                    )));
        }
        return 0;
    }
    return 0;
}

/* dup
 * aload 0
 * swap
 * putfield arg
 * pop
 * ------------>
 * aload 0
 * swap
 * putfield arg
 */

int simplify_putfield(CODE **c) {
    int x;
    char* arg;
    if (is_dup(*c) && is_aload(next(*c), &x) && is_swap(next(next(*c))) &&
        is_putfield(next(next(next(*c))), &arg) && is_pop(next(next(next(next(*c)))))) {
        if (x == 0) {
            return replace(c, 5, makeCODEaload(x, makeCODEswap(makeCODEputfield(arg, NULL))));
        }
        return 0;
    }
    return 0;
}

/*
 * new ...
 * dup
 * invokenonvirtual ...
 * aload_0
 * swap
 * --------->
 * aload_0
 * new ...
 * dup
 * invokenonvirtual ...
 */
/* TODO: Should we check the method corresponding to the invokenonvirtual
 * accepts no arguments? */
int simplify_invokenonvirtual(CODE **c)
{ int x;
  char *arg1, *arg2;
  if (is_new(*c, &arg1) && is_dup(next(*c)) &&
      is_invokenonvirtual(next(next(*c)), &arg2) &&
      is_aload(next(next(next(*c))), &x) &&
      is_swap(next(next(next(next(*c)))))) {
      /* TODO: Is this check needed? */
      if (x == 0) {
        return replace(c, 5,
          makeCODEaload(x,
            makeCODEnew(arg1,
              makeCODEdup(
                makeCODEinvokenonvirtual(arg2, NULL)
              )
            )
          )
        );
      }
  }

  return 0;
}


/*********************
 * LAURIE'S PATTERNS *
 **********************/


/* iload x
 * ldc k   (0<=k<=127)
 * iadd
 * istore x
 * --------->
 * iinc x k
 */
int positive_increment(CODE **c)
{ int x,y,k;
  if (is_iload(*c,&x) &&
      is_ldc_int(next(*c),&k) &&
      is_iadd(next(next(*c))) &&
      is_istore(next(next(next(*c))),&y) &&
      x==y && 0<=k && k<=127) {
     return replace(c,4,makeCODEiinc(x,k,NULL));
  }
  return 0;
}

/* dup
 * astore x
 * pop
 * -------->
 * astore x
 */
int simplify_astore(CODE **c)
{ int x;
  if (is_dup(*c) &&
      is_astore(next(*c),&x) &&
      is_pop(next(next(*c)))) {
     return replace(c,3,makeCODEastore(x,NULL));
  }
  return 0;
}

/* goto L1
 * ...
 * L1:
 * goto L2
 * ...
 * L2:
 * --------->
 * goto L2
 * ...
 * L1:    (reference count reduced by 1)
 * goto L2
 * ...
 * L2:    (reference count increased by 1)
 */
int simplify_goto_goto(CODE **c)
{ int l1,l2;
  if (is_goto(*c,&l1) && is_goto(next(destination(l1)),&l2) && l1>l2) {
     droplabel(l1);
     copylabel(l2);
     return replace(c,1,makeCODEgoto(l2,NULL));
  }
  return 0;
}

/* iload x        iload x        iload x
 * ldc 0          ldc 1          ldc 2
 * imul           imul           imul
 * ------>        ------>        ------>
 * ldc 0          iload x        iload x
 *                               dup
 *                               iadd
 */

int simplify_multiplication_right(CODE **c)
{ int x,k;
  if (is_iload(*c,&x) &&
      is_ldc_int(next(*c),&k) &&
      is_imul(next(next(*c)))) {
     if (k==0) return replace(c,3,makeCODEldc_int(0,NULL));
     else if (k==1) return replace(c,3,makeCODEiload(x,NULL));
     else if (k==2) return replace(c,3,makeCODEiload(x,
                                       makeCODEdup(
                                       makeCODEiadd(NULL))));
     return 0;
  }
  return 0;
}


/* TODO: Sometimes lowering this number results in more optimization (Huh?)... */
#define OPTS 29

OPTI optimization[OPTS] = {
  /* Our patterns. */
  drop_dead_label,
  simplify_consec_ifeqs,
  simplify_consec_if_icmplt_and_ifeq,
  simplify_icmpeq_zero,
  simplify_icmpne_zero,
  strip_after_return,
  load_load_swap,
  aconst_null_dup_ifeq,
  simplify_dup_cmpeq,
  strip_nops_after_return,
  ldc_dup_ifnull,
  simplify_addition_left,
  simplify_addition_right,
  simpify_subtraction_right,
  simplify_multiplication_left,
  simplify_division_right,
  rm_same_iload_istore,
  rm_same_aload_astore,
  rm_redundant_loads,
  simplify_dup_cmpeq,
  simplify_istore,
  simplify_istore_iload,
  simplify_astore_aload,
  simplify_putfield,
  simplify_invokenonvirtual,
  /* Laurie's patterns. */
  positive_increment,
  simplify_astore,
  simplify_goto_goto,
  simplify_multiplication_right
};

/*
 * TODO: Figure out why this method of adding patterns doesn't work.
int init_patterns()
{
  ADD_PATTERN(simplify_multiplication_right);
  ADD_PATTERN(simplify_astore);
  ADD_PATTERN(positive_increment);
  ADD_PATTERN(simplify_goto_goto);
}
*/
