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
 * branch L1:
 * ...
 * L1:
 * L2:
 * --------->
 * branch L2:
 * ...
 * L1:
 * L2:
 */
/* Helps in removing unnecessary labels. */
int branch_to_last_label(CODE **c)
{ int l1, l2;

  /* goto */
  if (is_goto(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEgoto(l2, NULL));
  }

  /* ifeq */
  if (is_ifeq(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEifeq(l2, NULL));
  }

  /* ifne */
  if (is_ifne(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEifne(l2, NULL));
  }

  /* ifnull */
  if (is_ifnull(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEifnull(l2, NULL));
  }

  /* ifnonnull */
  if (is_ifnonnull(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEifnonnull(l2, NULL));
  }

  /* if_icmpeq */
  if (is_if_icmpeq(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEif_icmpeq(l2, NULL));
  }

  /* if_icmpne */
  if (is_if_icmpne(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEif_icmpne(l2, NULL));
  }

  /* if_icmpgt */
  if (is_if_icmpgt(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEif_icmpgt(l2, NULL));
  }

  /* if_icmplt */
  if (is_if_icmplt(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEif_icmplt(l2, NULL));
  }

  /* if_icmpge */
  if (is_if_icmpge(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEif_icmpge(l2, NULL));
  }

  /* if_icmple */
  if (is_if_icmple(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEif_icmple(l2, NULL));
  }

  /* if_acmpeq */
  if (is_if_acmpeq(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEif_acmpeq(l2, NULL));
  }

  /* if_acmpne */
  if (is_if_acmpne(*c, &l1) && is_label(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 1, makeCODEif_acmpne(l2, NULL));
  }

  return 0;
}

/*
 * branch1 L1         (branch1: ifeq, ifne, ifnull, ifnonnull, if_icmpeq,
 *                     ificmpne, if_icmpgt, if_icmplt, if_icmpge, if_icmple,
 *                     if_acmpeq, or if_acmpne) (NOT goto)
 *                    (L1: Unique)
 * goto L2
 * L1:
 * --------->
 * branch2 L2         (branch2: Opposite of branch1)
 */
int collapse_branch_goto(CODE **c)
{ int l1, l2, l3;

  /* ifeq, ldc integer */
  if (is_ifeq(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEifne(l2, NULL));
  }

  /* ifne, ldc integer */
  if (is_ifne(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEifeq(l2, NULL));
  }

  /* ifnull, ldc integer */
  if (is_ifnull(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEifnonnull(l2, NULL));
  }

  /* ifnonnull, ldc integer */
  if (is_ifnonnull(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEifnull(l2, NULL));
  }

  /* if_icmpeq, ldc integer */
  if (is_if_icmpeq(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEif_icmpne(l2, NULL));
  }

  /* if_icmpne, ldc integer */
  if (is_if_icmpne(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEif_icmpeq(l2, NULL));
  }

  /* if_icmpgt, ldc integer */
  if (is_if_icmpgt(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEif_icmple(l2, NULL));
  }

  /* if_icmplt, ldc integer */
  if (is_if_icmplt(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEif_icmpge(l2, NULL));
  }

  /* if_icmpge, ldc integer */
  if (is_if_icmpge(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEif_icmplt(l2, NULL));
  }

  /* if_icmple, ldc integer */
  if (is_if_icmple(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEif_icmpgt(l2, NULL));
  }

  /* if_acmpeq, ldc integer */
  if (is_if_acmpeq(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEif_acmpne(l2, NULL));
  }

  /* if_acmpne, ldc integer */
  if (is_if_acmpne(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1) {
    return replace(c, 3, makeCODEif_acmpeq(l2, NULL));
  }

  return 0;
}

/* 
 * branch1 L1         (branch1: ifeq, ifne, ifnull, ifnonnull, if_icmpeq,
 *                     ificmpne, if_icmpgt, if_icmplt, if_icmpge, if_icmple,
 *                     if_acmpeq, or if_acmpne) (NOT goto)
 *                    (L1: Unique)
 * iconst_0
 * goto L2            (L2: Unique)
 * L1:
 * iconst_1
 * L2:
 * ifeq L3
 * --------->
 * branch2 L3         (branch2: ifne, ifeq, ifnonnull, ifnull, if_icmpne,
 *                     ificmpeq, if_icmple, if_icmpge, if_icmplt, if_icmpgt,
 *                     if_acmpne, or if_acmpeq, respectively, depending on the
 *                     identity of branch1) (The opposite of branch1)
 */
int collapse_local_branching(CODE **c)
{ int l1, l2, l3, l4, l5, x1, x2;

  /* ifeq --> ifne */
  if (is_ifeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEifne(l5, NULL));
  }

  /* ifne --> ifeq */
  if (is_ifne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEifeq(l5, NULL));
  }

  /* ifnull --> ifnonnull */
  if (is_ifnull(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEifnonnull(l5, NULL));
  }

  /* ifnonnull --> ifnull */
  if (is_ifnonnull(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEifnull(l5, NULL));
  }

  /* if_icmpeq --> if_icmpne */
  if (is_if_icmpeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_icmpne(l5, NULL));
  }

  /* if_icmpne --> if_icmpeq */
  if (is_if_icmpne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_icmpeq(l5, NULL));
  }

  /* if_icmpgt --> if_icmple */
  if (is_if_icmpgt(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_icmple(l5, NULL));
  }

  /* if_icmplt --> if_icmpge */
  if (is_if_icmplt(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_icmpge(l5, NULL));
  }

  /* if_icmpge --> if_icmplt */
  if (is_if_icmpge(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_icmplt(l5, NULL));
  }

  /* if_icmple --> if_icmpgt */
  if (is_if_icmple(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_icmpgt(l5, NULL));
  }

  /* if_acmpeq --> if_acmpne */
  if (is_if_acmpeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_acmpne(l5, NULL));
  }

  /* if_acmpne --> if_acmpeq */
  if (is_if_acmpne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_ifeq(nextby(*c, 6), &l5)) {
    return replace(c, 7, makeCODEif_acmpeq(l5, NULL));
  }

  return 0;
}

/* 
 * branch1 L1         (branch1: ifeq, ifne, ifnull, ifnonnull, if_icmpeq,
 *                     ificmpne, if_icmpgt, if_icmplt, if_icmpge, if_icmple,
 *                     if_acmpeq, or if_acmpne) (NOT goto)
 *                    (L1: Unique)
 * iconst_0
 * goto L2            (L2: Unique)
 * L1:
 * iconst_1
 * L2:
 * dup
 * {ifeq, ifne} L3
 * pop
 * --------->
 * branch2 L1         (If ifeq, then branch2 = branch1, otherwise branch2 is the
 *                     opposite)
 * ldc x              (x: 0 if ifeq, 1 otherwise)
 * goto L3
 * L1:
 */
int collapse_local_branching_with_dup(CODE **c)
{ int l1, l2, l3, l4, l5, x1, x2;
  
  /* ifeq, ifeq */
  if (is_ifeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEifeq(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* ifeq, ifne */
  if (is_ifeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEifne(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* ifne, ifeq */
  if (is_ifne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEifne(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* ifne, ifne */
  if (is_ifne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEifeq(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* ifnull, ifeq */
  if (is_ifnull(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEifnull(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* ifnull, ifne */
  if (is_ifnull(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEifnonnull(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* ifnonnull, ifeq */
  if (is_ifnonnull(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEifnonnull(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* ifnonnull, ifne */
  if (is_ifnonnull(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEifnull(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmpeq, ifeq */
  if (is_if_icmpeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmpeq(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmpeq, ifne */
  if (is_if_icmpeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmpne(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmpne, ifeq */
  if (is_if_icmpne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmpne(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmpne, ifne */
  if (is_if_icmpne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmpeq(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmpgt, ifeq */
  if (is_if_icmpgt(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmpgt(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmpgt, ifne */
  if (is_if_icmpgt(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmple(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmplt, ifeq */
  if (is_if_icmplt(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmplt(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmplt, ifne */
  if (is_if_icmplt(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmpge(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmpge, ifeq */
  if (is_if_icmpge(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmpge(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmpge, ifne */
  if (is_if_icmpge(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmplt(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmple, ifeq */
  if (is_if_icmple(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmple(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_icmple, ifne */
  if (is_if_icmple(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_icmpgt(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_acmpeq, ifeq */
  if (is_if_acmpeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_acmpeq(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_acmpeq, ifne */
  if (is_if_acmpeq(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_acmpne(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_acmpne, ifeq */
  if (is_if_acmpne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifeq(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_acmpne(l1,
        makeCODEldc_int(0,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  /* if_acmpne, ifne */
  if (is_if_acmpne(*c, &l1) && uniquelabel(l1) &&
      is_ldc_int(next(*c), &x1) && x1 == 0 &&
      is_goto(nextby(*c, 2), &l2) && uniquelabel(l2) &&
      is_label(nextby(*c, 3), &l3) && l3 == l1 &&
      is_ldc_int(nextby(*c, 4), &x2) && x2 == 1 &&
      is_label(nextby(*c, 5), &l4) && l4 == l2 &&
      is_dup(nextby(*c, 6)) &&
      is_ifne(nextby(*c, 7), &l5) &&
      is_pop(nextby(*c, 8))) {
    return replace(c, 9,
      makeCODEif_acmpeq(l1,
        makeCODEldc_int(1,
          makeCODEgoto(l5,
            makeCODElabel(l1, NULL)
          )
        )
      )
    );
  }

  return 0;
}

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
 * aconst_null
 * dup
 * {if_acmpeq, if_icmpeq} L
 * -------->
 * goto L
 */
int simplify_aconstnull_dup_ifcmpeq(CODE **c)
{ int l;

  /* if_acmpeq */
  if (is_aconst_null(*c) &&
      is_dup(next(*c)) &&
      is_if_acmpeq(nextby(*c, 2), &l)) {
    return replace(c, 3, makeCODEgoto(l, NULL));
  }

  /* if_icmpeq */
  if (is_aconst_null(*c) &&
      is_dup(next(*c)) &&
      is_if_icmpeq(nextby(*c, 2), &l)) {
    return replace(c, 3, makeCODEgoto(l, NULL));
  }

  return 0;
}

/*
 * aconst_null
 * {if_acmpeq, if_acmpne} L
 * -------->
 * {ifnull, ifnonnull} L
 */
int simplify_aconstnull_if(CODE **c)
{ int l;

  /* if_acmpeq */
  if (is_aconst_null(*c) &&
      is_if_acmpeq(next(*c), &l)) {
    return replace(c, 2, makeCODEifnull(l, NULL));
  }

  /* if_acmpne */
  if (is_aconst_null(*c) &&
      is_if_acmpne(next(*c), &l)) {
    return replace(c, 2, makeCODEifnonnull(l, NULL));
  }

  return 0;
}

/*
 * {iconst_0, iconst_1}
 * goto L1
 * ...
 * L1:
 * {ifeq, ifne} L2
 * --------->
 * goto L2
 * ...
 * L1:
 * {ifeq, ifne} L2
 */
/* Helps in removing unnecessary labels. */
int simplify_goto_if(CODE **c)
{ int x, l1, l2;
  
  /* iconst_0, ifeq */
  if (is_ldc_int(*c, &x) && x == 0 &&
      is_goto(next(*c), &l1) &&
      is_ifeq(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 2, makeCODEgoto(l2, NULL));
  }

  /* iconst_1, ifne */
  if (is_ldc_int(*c, &x) && x == 1 &&
      is_goto(next(*c), &l1) &&
      is_ifne(next(destination(l1)), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 2, makeCODEgoto(l2, NULL));
  }

  return 0;
}

/*
 * {iconst_0, iconst_1}
 * goto L1
 * ...
 * L1:
 * dup
 * {ifeq, ifne} L2
 * --------->
 * {iconst_0, iconst_1}
 * goto L2
 * ...
 * L1:
 * dup
 * {ifeq, ifne} L2
 */
/* Helps in removing unnecessary labels. */
int simplify_goto_dup_if(CODE **c)
{ int x, l1, l2;
  
  /* iconst_0, ifeq */
  if (is_ldc_int(*c, &x) && x == 0 &&
      is_goto(next(*c), &l1) &&
      is_dup(next(destination(l1))) &&
      is_ifeq(nextby(destination(l1), 2), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 2, makeCODEldc_int(0, makeCODEgoto(l2, NULL)));
  }

  /* iconst_1, ifne */
  if (is_ldc_int(*c, &x) && x == 1 &&
      is_goto(next(*c), &l1) &&
      is_dup(next(destination(l1))) &&
      is_ifne(nextby(destination(l1), 2), &l2)) {
    droplabel(l1);
    copylabel(l2);
    return replace(c, 2, makeCODEldc_int(1, makeCODEgoto(l2, NULL)));
  }

  return 0;
}

/*
 * iconst_0
 * if_icmpeq L
 * --------->
 * ifeq L
 */
int simplify_ificmpeq_zero(CODE **c)
{ int k, l;
  if (is_ldc_int(*c, &k) && k == 0 && is_if_icmpeq(next(*c), &l)) {
    return replace(c, 2, makeCODEifeq(l, NULL));
  }

  return 0;
}

/*
 * iconst_0
 * if_icmpne L
 * --------->
 * ifne L
 */
int simplify_ificmpne_zero(CODE **c)
{ int k, l;
  if (is_ldc_int(*c, &k) && k == 0 && is_if_icmpne(next(*c), &l)) {
    return replace(c, 2, makeCODEifne(l, NULL));
  }

  return 0;
}

/*
 * ifnull L1    (L1: Unique)
 * goto L2
 * L1:
 * pop
 * ldc x
 * L2:
 * --------->
 * ifnonnull L2
 * pop
 * ldc x        (Integer or string)
 * L2:
 */
int simplify_ifnull(CODE **c)
{ int l1, l2, l3, l4, x;
  char* s;

  /* ldc integer */
  if (is_ifnull(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1 &&
      is_pop(nextby(*c, 3)) &&
      is_ldc_int(nextby(*c, 4), &x) &&
      is_label(nextby(*c, 5), &l4) && l4 == l2) {
    return replace(c, 6,
      makeCODEifnonnull(l2,
        makeCODEpop(
          makeCODEldc_int(x,
            makeCODElabel(l2, NULL)
          )
        )
      )
    );
  }

  /* ldc string */
  if (is_ifnull(*c, &l1) && uniquelabel(l1) &&
      is_goto(next(*c), &l2) &&
      is_label(nextby(*c, 2), &l3) && l3 == l1 &&
      is_pop(nextby(*c, 3)) &&
      is_ldc_string(nextby(*c, 4), &s) &&
      is_label(nextby(*c, 5), &l4) && l4 == l2) {
    return replace(c, 6,
      makeCODEifnonnull(l2,
        makeCODEpop(
          makeCODEldc_string(s,
            makeCODElabel(l2, NULL)
          )
        )
      )
    );
  }

  return 0;
}

/* 
 * ldc x        (Integer or string)
 * dup
 * ifnull L
 * --------->
 * ldc x
 */
int simplify_ldc_dup_ifnull(CODE **c)
{ int x, l;
  char* s;

  /* Integer */
  if (is_ldc_int(*c, &x) && is_dup(next(*c)) && is_ifnull(nextby(*c, 2), &l)) {
    droplabel(l);
    return replace(c, 3, makeCODEldc_int(x, NULL));
  }

  /* String */
  if (is_ldc_string(*c, &s) && is_dup(next(*c)) && is_ifnull(nextby(*c, 2), &l)) {
    droplabel(l);
    return replace(c, 3, makeCODEldc_string(s, NULL));
  }

  return 0;
}

/* 
 * dup
 * aload 0
 * swap
 * putfield arg
 * pop
 * --------->
 * aload 0
 * swap
 * putfield arg
 */
int simplify_putfield(CODE **c)
{ int x;
  char* arg;
  if (is_dup(*c) &&
      is_aload(next(*c), &x) && x == 0 &&
      is_swap(nextby(*c, 2)) &&
      is_putfield(nextby(*c, 3), &arg) &&
      is_pop(nextby(*c, 4))) {
    return replace(c, 5,
      makeCODEaload(x,
        makeCODEswap(
          makeCODEputfield(arg, NULL)
        )
      )
    );
  }

  return 0;
}

/*
 * load k
 * load l
 * swap 
 * --------->
 * load k
 * load l
 *
 * ldc x      (Integer or string)
 * aload k
 * swap
 * --------->
 * aload k
 * ldc x
 *
 * iload k
 * ldc x
 * iadd
 * aload l
 * swap
 * --------->
 * aload l
 * iload k
 * ldc x
 * iadd
 *
 * aload k
 * getfield ...
 * aload k
 * swap
 * --------->
 * aload k
 * dup
 * getfield ...
 *
 * aconst_null
 * aload k
 * swap
 * --------->
 * aload k
 * aconst_null
 *
 * new ...
 * dup
 * ldc x                 (Optional, integer or string)
 * invokenonvirtual ...
 * aload k
 * swap
 * --------->
 * aload k
 * new ...
 * dup
 * ldc x
 * invokenonvirtual ...
 *
 * new ...
 * dup
 * aload k
 * getfield ...
 * invokenonvirtual ...
 * aload l
 * swap
 * --------->
 * aload l
 * new ...
 * dup
 * aload k
 * getfield ...
 * invokenonvirtual ...
 *
 * new ...
 * dup
 * iload k
 * iload l
 * invokenonvirtual ...
 * aload m
 * swap
 * --------->
 * aload m
 * new ...
 * dup
 * iload k
 * iload l
 * invokenonvirtual ...
 *
 * new ...
 * dup
 * aload k
 * ldc s                 (String)
 * iload l
 * invokenonvirtual ...
 * aload m
 * swap
 * --------->
 * aload m
 * new ...
 * dup
 * aload k
 * ldc s                 (String)
 * iload l
 * invokenonvirtual ...
 *
 * aload k
 * iload l
 * invokevirtual ...
 * aload m
 * swap
 * --------->
 * aload m
 * aload k
 * iload l
 * invokevirtual ...
 */
int simplify_swap(CODE **c)
{ int k, l, m, x;
  char *s, *arg1, *arg2, *arg3;

  /*
   * iload k
   * iload l
   * swap 
   * --------->
   * iload k
   * iload l
   */
  if (is_iload(*c, &k) && is_iload(next(*c), &l) && is_swap(nextby(*c, 2))) {
    return replace(c, 3, makeCODEiload(l, makeCODEiload(k, NULL)));
  }

  /*
   * iload k
   * aload l
   * swap 
   * --------->
   * aload k
   * iload l
   */
  if (is_iload(*c, &k) && is_aload(next(*c), &l) && is_swap(nextby(*c, 2))) {
    return replace(c, 3, makeCODEaload(l, makeCODEiload(k, NULL)));
  }

  /*
   * aload k
   * iload l
   * swap 
   * --------->
   * iload k
   * aload l
   */
  if (is_aload(*c, &k) && is_iload(next(*c), &l) && is_swap(nextby(*c, 2))) {
    return replace(c, 3, makeCODEiload(l, makeCODEaload(k, NULL)));
  }

  /*
   * aload k
   * aload l
   * swap 
   * --------->
   * aload k
   * aload l
   */
  if (is_aload(*c, &k) && is_aload(next(*c), &l) && is_swap(nextby(*c, 2))) {
    return replace(c, 3, makeCODEaload(l, makeCODEaload(k, NULL)));
  }

  /* 
   * ldc x      (Integer)
   * aload k
   * swap
   * --------->
   * aload k
   * ldc x
   */
  if (is_ldc_int(*c, &x) && is_aload(next(*c), &k) && is_swap(nextby(*c, 2))) {
    return replace(c, 3, makeCODEaload(k, makeCODEldc_int(x, NULL)));
  }

  /* 
   * ldc x      (String)
   * aload k
   * swap
   * --------->
   * aload k
   * ldc x
   */
  if (is_ldc_string(*c, &s) && is_aload(next(*c), &k) && is_swap(nextby(*c, 2))) {
    return replace(c, 3, makeCODEaload(k, makeCODEldc_string(s, NULL)));
  }

  /* 
   * iload k
   * ldc x
   * iadd
   * aload l
   * swap
   * --------->
   * aload l
   * iload k
   * ldc x
   * iadd
   */
  if (is_iload(*c, &k) &&
      is_ldc_int(next(*c), &x) &&
      is_iadd(nextby(*c, 2)) &&
      is_aload(nextby(*c, 3), &l) &&
      is_swap(nextby(*c, 4))) {
    return replace(c, 5,
      makeCODEaload(l,
        makeCODEiload(k,
          makeCODEldc_int(x,
            makeCODEiadd(NULL)
          )
        )
      )
    );
  }

  /*
   * aload k
   * getfield ...
   * aload k
   * swap
   * --------->
   * aload k
   * dup
   * getfield ...
   */
  if (is_aload(*c, &k) &&
      is_getfield(next(*c), &arg1) &&
      is_aload(nextby(*c, 2), &l) && l == k &&
      is_swap(nextby(*c, 3))) {
    return replace(c, 4,
      makeCODEaload(k,
        makeCODEdup(
          makeCODEgetfield(arg1, NULL)
        )
      )
    );
  }

  /*
   * aconst_null
   * aload k
   * swap
   * --------->
   * aload k
   * aconst_null
   */
  if (is_aconst_null(*c) && is_aload(next(*c), &k) && is_swap(nextby(*c, 2))) {
    return replace(c, 3, makeCODEaload(k, makeCODEaconst_null(NULL)));
  }

  /*
   * new ...
   * dup
   * ldc x                 (Optional, integer or string)
   * invokenonvirtual ...
   * aload k
   * swap
   * --------->
   * aload k
   * new ...
   * dup
   * ldc x
   * invokenonvirtual ...
   */

  /* No ldc */
  if (is_new(*c, &arg1) &&
      is_dup(next(*c)) &&
      is_invokenonvirtual(nextby(*c, 2), &arg2) && strstr(arg2, "()") &&
      is_aload(nextby(*c, 3), &k) &&
      is_swap(nextby(*c, 4))) {
    return replace(c, 5,
      makeCODEaload(k,
        makeCODEnew(arg1,
          makeCODEdup(
            makeCODEinvokenonvirtual(arg2, NULL)
          )
        )
      )
    );
  }

  /* With ldc (integer) */
  if (is_new(*c, &arg1) &&
      is_dup(next(*c)) &&
      is_ldc_int(nextby(*c, 2), &x) &&
      is_invokenonvirtual(nextby(*c, 3), &arg2) &&
        (strstr(arg2, "(C)V") || strstr(arg2, "(I)")) &&
      is_aload(nextby(*c, 4), &k) &&
      is_swap(nextby(*c, 5))) {
    return replace(c, 6,
      makeCODEaload(k,
        makeCODEnew(arg1,
          makeCODEdup(
            makeCODEldc_int(x,
              makeCODEinvokenonvirtual(arg2, NULL)
            )
          )
        )
      )
    );
  }

  /* With ldc (string) */
  if (is_new(*c, &arg1) &&
      is_dup(next(*c)) &&
      is_ldc_string(nextby(*c, 2), &s) &&
      is_invokenonvirtual(nextby(*c, 3), &arg2) &&
        strstr(arg2, "(Ljava/lang/String;)") && 
      is_aload(nextby(*c, 4), &k) &&
      is_swap(nextby(*c, 5))) {
    return replace(c, 6,
      makeCODEaload(k,
        makeCODEnew(arg1,
          makeCODEdup(
            makeCODEldc_string(s,
              makeCODEinvokenonvirtual(arg2, NULL)
            )
          )
        )
      )
    );
  }

  /*
   * new ...
   * dup
   * aload k
   * getfield ...
   * invokenonvirtual ...
   * aload l
   * swap
   * --------->
   * aload l
   * new ...
   * dup
   * aload k
   * getfield ...
   * invokenonvirtual ...
   */
  if (is_new(*c, &arg1) &&
      is_dup(next(*c)) &&
      is_aload(nextby(*c, 2), &k) &&
      is_getfield(nextby(*c, 3), &arg2) &&
      is_invokenonvirtual(nextby(*c, 4), &arg3) && strstr(arg3, "(I)") &&
      is_aload(nextby(*c, 5), &l) &&
      is_swap(nextby(*c, 6))) {
    return replace(c, 7,
      makeCODEaload(l,
        makeCODEnew(arg1,
          makeCODEdup(
            makeCODEaload(k,
              makeCODEgetfield(arg2,
                makeCODEinvokenonvirtual(arg3, NULL)
              )
            )
          )
        )
      )
    );
  }

  /*
   * new ...
   * dup
   * iload k
   * iload l
   * invokenonvirtual ...
   * aload m
   * swap
   * --------->
   * aload m
   * new ...
   * dup
   * iload k
   * iload l
   * invokenonvirtual ...
   */
  if (is_new(*c, &arg1) &&
      is_dup(next(*c)) &&
      is_iload(nextby(*c, 2), &k) &&
      is_iload(nextby(*c, 3), &l) &&
      is_invokenonvirtual(nextby(*c, 4), &arg2) && strstr(arg2, "(II)") &&
      is_aload(nextby(*c, 5), &m) &&
      is_swap(nextby(*c, 6))) {
    return replace(c, 7,
      makeCODEaload(m,
        makeCODEnew(arg1,
          makeCODEdup(
            makeCODEiload(k,
              makeCODEiload(l,
                makeCODEinvokenonvirtual(arg2, NULL)
              )
            )
          )
        )
      )
    );
  }

  /*
   * new ...
   * dup
   * aload k
   * ldc s                 (String)
   * iload l
   * invokenonvirtual ...
   * aload m
   * swap
   * --------->
   * aload m
   * new ...
   * dup
   * aload k
   * ldc s                 (String)
   * iload l
   * invokenonvirtual ...
   */
  if (is_new(*c, &arg1) &&
      is_dup(next(*c)) &&
      is_aload(nextby(*c, 2), &k) &&
      is_ldc_string(nextby(*c, 3), &s) &&
      is_iload(nextby(*c, 4), &l) &&
      is_invokenonvirtual(nextby(*c, 5), &arg2) &&
        strstr(arg2, "(LRoom;Ljava/lang/String;I)") &&
      is_aload(nextby(*c, 6), &m) &&
      is_swap(nextby(*c, 7))) {
    replace(c, 8,
      makeCODEaload(m,
        makeCODEnew(arg1,
          makeCODEdup(
            makeCODEaload(k,
              makeCODEldc_string(s,
                makeCODEiload(l,
                  makeCODEinvokenonvirtual(arg2, NULL)
                )
              )
            )
          )
        )
      )
    );
  }

  /*
   * aload k
   * iload l
   * invokevirtual ...
   * aload m
   * swap
   * --------->
   * aload m
   * aload k
   * iload l
   * invokevirtual ...
   */
  if (is_aload(*c, &k) &&
      is_iload(next(*c), &l) &&
      is_invokevirtual(nextby(*c, 2), &arg1) && strstr(arg1, "(I)") &&
      is_aload(nextby(*c, 3), &m) &&
      is_swap(nextby(*c, 4))) {
    return replace(c, 5,
      makeCODEaload(m,
        makeCODEaload(k,
          makeCODEiload(l,
            makeCODEinvokevirtual(arg1, NULL)
          )
        )
      )
    );
  }

  return 0;
}

/* 
 * store k
 * load k
 * --------->
 * dup
 * store k
 */
int simplify_store_load(CODE **c)
{ int k1, k2;

  /* istore, iload */
  if (is_istore(*c, &k1) && is_iload(next(*c), &k2) && k1 == k2) {
    return replace(c, 2, makeCODEdup(makeCODEistore(k1, NULL)));
  }

  /* astore, aload */
  if (is_astore(*c, &k1) && is_aload(next(*c), &k2) && k1 == k2) {
    return replace(c, 2, makeCODEdup(makeCODEastore(k1, NULL)));
  }

  return 0;
}

/*
 * goto L
 * (Non-label)
 * --------->
 * goto L
 */
int strip_after_goto(CODE **c)
{ int l1, l2;
  if (is_goto(*c, &l1) &&
      !is_label(next(*c), &l2)) {
    return replace(c, 2, makeCODEgoto(l1, NULL));
  }

  return 0;
}

/*
 * {return, areturn, ireturn}
 * ...
 * L:
 * --------->
 * {return, areturn, ireturn}
 * L:
 */
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
 * goto L
 * L:
 * --------->
 * L:
 */
int strip_stupid_goto(CODE **c)
{ int l1, l2;
  if (is_goto(*c, &l1) &&
      is_label(next(*c), &l2) && l1 == l2) {
    return replace(c, 2, makeCODElabel(l1, NULL));
  }

  return 0;
}

/*
 * i?return
 * nop
 * -------->
 * i?return (Not areturn)
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

/*
 * ldc 0
 * iload x
 * iadd
 * -------->
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
 * -------->
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
 * -------->
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

/* 
 * ldc 0            ldc 1           ldc 2
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

/* 
 * iload x
 * ldc 1
 * idiv
 * -------->
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

/*
 * dup
 * {if_acmpeq, if_icmpeq} L
 * -------->
 * goto L
 */
int simplify_dup_cmpeq(CODE **c)
{ int l;

  /* if_acmpeq */
  if (is_dup(*c) && is_if_acmpeq(next(*c), &l)) {
    return replace_modified(c, 2, makeCODEgoto(l, NULL));
  }

  /* if_icmpeq */
  if (is_dup(*c) && is_if_icmpeq(next(*c), &l)) {
    return replace_modified(c, 2, makeCODEgoto(l, NULL));
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


#define OPTS 32

OPTI optimization[OPTS] = {
  /* Our patterns. */
  branch_to_last_label,
  collapse_branch_goto,
  collapse_local_branching,
  collapse_local_branching_with_dup,
  drop_dead_label,
  simplify_aconstnull_dup_ifcmpeq,
  simplify_aconstnull_if,
  simplify_goto_if,
  simplify_goto_dup_if,
  simplify_ificmpeq_zero,
  simplify_ificmpne_zero,
  simplify_ifnull,
  simplify_ldc_dup_ifnull,
  simplify_putfield,
  simplify_swap,
  simplify_store_load,
  strip_after_goto,
  strip_after_return,
  strip_stupid_goto,
  simplify_dup_cmpeq,
  strip_nops_after_return,
  simplify_addition_left,
  simplify_addition_right,
  simpify_subtraction_right,
  simplify_multiplication_left,
  simplify_division_right,
  simplify_dup_cmpeq,
  simplify_istore,
  /* Laurie's patterns. */
  positive_increment,
  simplify_astore,
  simplify_goto_goto,
  simplify_multiplication_right
};
