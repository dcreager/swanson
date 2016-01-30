/* -*- coding: utf-8 -*-
 * Copyright © 2016, Swanson Project.
 * Please see the COPYING file in this distribution for license details.
 */

#include <string.h>

#include "ccan/tap/tap.h"
#include "swanson.h"

/*-----------------------------------------------------------------------------
 * Helpers
 */

#define ok_alloc(var, call) \
    do { \
        var = call; \
        ok(var != NULL, #call " != NULL"); \
    } while (0)

#define ok0(call, desc) \
    do { \
        ok((call) == 0, desc); \
    } while (0)

/*-----------------------------------------------------------------------------
 * S₀: Names
 */

#define TEST_COUNT_S0_NAMES  11

static void
test_s0_names(void)
{
    struct s0_name  *n1;
    struct s0_name  *n2;
    struct s0_name  *n3;

    diag("S₀ names");

    ok_alloc(n1, s0_name_new_str("hello"));
    ok_alloc(n2, s0_name_new(5, "hello"));
    /* content includes NUL terminator */
    ok_alloc(n3, s0_name_new(6, "hello"));

    ok(s0_name_size(n1) == 5, "[name(5, \"hello\")] == 5");
    ok(s0_name_size(n2) == 5, "[name(5, \"hello\")] == 5");
    ok(s0_name_size(n3) == 6, "[name(6, \"hello\\x00\")] == 6");

    ok(memcmp(s0_name_content(n1), "hello", 5) == 0,
       "name(5, \"hello\") == \"hello\"");
    ok(memcmp(s0_name_content(n2), "hello", 5) == 0,
       "name(5, \"hello\") == \"hello\"");
    ok(memcmp(s0_name_content(n3), "hello\x00", 6) == 0,
       "name(6, \"hello\\x00\") == \"hello\\x00\"");

    ok(s0_name_eq(n1, n2),  "name(5, \"hello\") == name(5, \"hello\")");
    ok(!s0_name_eq(n1, n3), "name(5, \"hello\") != name(6, \"hello\\x00\")");

    s0_name_free(n1);
    s0_name_free(n2);
    s0_name_free(n3);
}

/*-----------------------------------------------------------------------------
 * S₀: Name sets
 */

#define TEST_COUNT_S0_NAME_SETS  14

static void
test_s0_name_sets(void)
{
    struct s0_name_set  *set;
    struct s0_name  *name;

    diag("S₀ name sets");

#define check_size(expected) \
    ok(s0_name_set_size(set) == expected, \
       "s0_name_set_size(set) == " #expected);

    ok_alloc(set, s0_name_set_new());
    check_size(0);

    ok_alloc(name, s0_name_new_str("a"));
    ok0(s0_name_set_add(set, name),
        "s0_name_set_add(set, \"a\")");
    check_size(1);

    ok_alloc(name, s0_name_new_str("b"));
    ok0(s0_name_set_add(set, name),
        "s0_name_set_add(set, \"b\")");
    check_size(2);

    ok_alloc(name, s0_name_new_str("a"));
    ok(s0_name_eq(name, s0_name_set_at(set, 0)),
       "s0_name_set_at(set, 0) == \"a\"");
    s0_name_free(name);
    check_size(2);

    ok_alloc(name, s0_name_new_str("b"));
    ok(s0_name_eq(name, s0_name_set_at(set, 1)),
       "s0_name_set_at(set, 1) == \"b\"");
    s0_name_free(name);
    check_size(2);

    s0_name_set_free(set);
#undef check_size
}

/*-----------------------------------------------------------------------------
 * S₀: Atoms
 */

#define TEST_COUNT_S0_ATOMS  12

static void
test_s0_atoms(void)
{
    struct s0_entity  *a1;
    struct s0_entity  *a2;
    struct s0_entity  *a3;

    diag("S₀ atoms");

    ok_alloc(a1, s0_atom_new());
    ok_alloc(a2, s0_atom_new());
    ok_alloc(a3, s0_atom_new());

    ok(s0_entity_type(a1) == S0_ENTITY_TYPE_ATOM, "type(atom1) == atom");
    ok(s0_entity_type(a2) == S0_ENTITY_TYPE_ATOM, "type(atom2) == atom");
    ok(s0_entity_type(a3) == S0_ENTITY_TYPE_ATOM, "type(atom3) == atom");

    ok(s0_atom_eq(a1, a1),  "atom1 == atom1");
    ok(s0_atom_eq(a2, a2),  "atom2 == atom2");
    ok(s0_atom_eq(a3, a3),  "atom3 == atom3");

    ok(!s0_atom_eq(a1, a2),  "atom1 != atom2");
    ok(!s0_atom_eq(a1, a3),  "atom1 != atom3");
    ok(!s0_atom_eq(a2, a3),  "atom2 != atom3");

    s0_entity_free(a1);
    s0_entity_free(a2);
    s0_entity_free(a3);
}

/*-----------------------------------------------------------------------------
 * S₀: Literals
 */

#define TEST_COUNT_S0_LITERALS  12

static void
test_s0_literals(void)
{
    struct s0_entity  *l1;
    struct s0_entity  *l2;
    struct s0_entity  *l3;

    diag("S₀ literals");

    ok_alloc(l1, s0_literal_new_str("hello"));
    ok_alloc(l2, s0_literal_new(5, "hello"));
    /* content includes NUL terminator */
    ok_alloc(l3, s0_literal_new(6, "hello"));

    ok(s0_entity_type(l1) == S0_ENTITY_TYPE_LITERAL,
       "type(literal1) == literal");
    ok(s0_entity_type(l2) == S0_ENTITY_TYPE_LITERAL,
       "type(literal2) == literal");
    ok(s0_entity_type(l3) == S0_ENTITY_TYPE_LITERAL,
       "type(literal3) == literal");

    ok(s0_literal_size(l1) == 5, "[literal(5, \"hello\")] == 5");
    ok(s0_literal_size(l2) == 5, "[literal(5, \"hello\")] == 5");
    ok(s0_literal_size(l3) == 6, "[literal(6, \"hello\\x00\")] == 6");

    ok(memcmp(s0_literal_content(l1), "hello", 5) == 0,
       "literal(5, \"hello\") == \"hello\"");
    ok(memcmp(s0_literal_content(l2), "hello", 5) == 0,
       "literal(5, \"hello\") == \"hello\"");
    ok(memcmp(s0_literal_content(l3), "hello\x00", 6) == 0,
       "literal(6, \"hello\\x00\") == \"hello\\x00\"");

    s0_entity_free(l1);
    s0_entity_free(l2);
    s0_entity_free(l3);
}

/*-----------------------------------------------------------------------------
 * S₀: Objects
 */

#define TEST_COUNT_S0_OBJECTS  25

static void
test_s0_objects(void)
{
    struct s0_entity  *obj;
    struct s0_name  *name;
    struct s0_entity  *atom1;
    struct s0_entity  *atom2;
    struct s0_object_entry  entry;

    diag("S₀ objects");

#define check_size(expected) \
    ok(s0_object_size(obj) == expected, \
       "s0_object_size(obj) == " #expected);

    ok_alloc(obj, s0_object_new());
    check_size(0);
    ok(s0_entity_type(obj) == S0_ENTITY_TYPE_OBJECT, "type(obj) == object");

    ok_alloc(name, s0_name_new_str("a"));
    ok_alloc(atom1, s0_atom_new());
    ok0(s0_object_add(obj, name, atom1),
        "s0_object_add(\"a\", atom1)");
    check_size(1);

    ok_alloc(name, s0_name_new_str("b"));
    ok_alloc(atom2, s0_atom_new());
    ok0(s0_object_add(obj, name, atom2),
        "s0_object_add(\"b\", atom2)");
    check_size(2);

    ok_alloc(name, s0_name_new_str("a"));
    ok(s0_object_get(obj, name) == atom1,
       "s0_object_get(obj, \"a\") == atom1");
    s0_name_free(name);
    check_size(2);

    ok_alloc(name, s0_name_new_str("b"));
    ok(s0_object_get(obj, name) == atom2,
       "s0_object_get(obj, \"b\") == atom2");
    s0_name_free(name);
    check_size(2);

    entry = s0_object_at(obj, 0);
    ok_alloc(name, s0_name_new_str("a"));
    ok(s0_name_eq(entry.name, name), "s0_object_at(obj, 0) == \"a\"");
    s0_name_free(name);
    ok(entry.entity == atom1, "s0_object_at(obj, 0) == atom1");
    check_size(2);

    entry = s0_object_at(obj, 1);
    ok_alloc(name, s0_name_new_str("b"));
    ok(s0_name_eq(entry.name, name), "s0_object_at(obj, 1) == \"b\"");
    s0_name_free(name);
    ok(entry.entity == atom2, "s0_object_at(obj, 1) == atom2");
    check_size(2);

    s0_entity_free(obj);
#undef check_size
}

/*-----------------------------------------------------------------------------
 * S₀: Environments
 */

#define TEST_COUNT_S0_ENVIRONMENTS  28

static void
test_s0_environments(void)
{
    struct s0_environment  *env;
    struct s0_name  *name;
    struct s0_entity  *atom1;
    struct s0_entity  *atom2;

    diag("S₀ environments");

#define check_size(expected) \
    ok(s0_environment_size(env) == expected, \
       "s0_environment_size(env) == " #expected);

    ok_alloc(env, s0_environment_new());
    check_size(0);

    ok_alloc(name, s0_name_new_str("a"));
    ok_alloc(atom1, s0_atom_new());
    ok0(s0_environment_add(env, name, atom1),
        "s0_environment_add(\"a\", atom1)");
    check_size(1);

    ok_alloc(name, s0_name_new_str("b"));
    ok_alloc(atom2, s0_atom_new());
    ok0(s0_environment_add(env, name, atom2),
        "s0_environment_add(\"b\", atom2)");
    check_size(2);

    ok_alloc(name, s0_name_new_str("a"));
    ok(s0_environment_get(env, name) == atom1,
       "s0_environment_get(env, \"a\") == atom1");
    s0_name_free(name);
    check_size(2);

    ok_alloc(name, s0_name_new_str("b"));
    ok(s0_environment_get(env, name) == atom2,
       "s0_environment_get(env, \"b\") == atom2");
    s0_name_free(name);
    check_size(2);

    ok_alloc(name, s0_name_new_str("a"));
    ok(s0_environment_delete(env, name) == atom1,
       "s0_environment_delete(env, \"a\") == atom1");
    s0_name_free(name);
    s0_entity_free(atom1);
    check_size(1);

    ok_alloc(name, s0_name_new_str("a"));
    ok(s0_environment_get(env, name) == NULL,
       "s0_environment_get(env, \"a\") == NULL");
    s0_name_free(name);
    check_size(1);

    ok_alloc(name, s0_name_new_str("b"));
    ok(s0_environment_delete(env, name) == atom2,
       "s0_environment_delete(env, \"b\") == atom2");
    s0_name_free(name);
    s0_entity_free(atom2);
    check_size(0);

    ok_alloc(name, s0_name_new_str("b"));
    ok(s0_environment_get(env, name) == NULL,
       "s0_environment_get(env, \"b\") == NULL");
    s0_name_free(name);
    check_size(0);

    s0_environment_free(env);
#undef check_size
}

/*-----------------------------------------------------------------------------
 * Harness
 */

#define TEST_COUNT_TOTAL \
    TEST_COUNT_S0_NAMES + \
    TEST_COUNT_S0_NAME_SETS + \
    TEST_COUNT_S0_ATOMS + \
    TEST_COUNT_S0_LITERALS + \
    TEST_COUNT_S0_OBJECTS + \
    TEST_COUNT_S0_ENVIRONMENTS + \
    0

int main(void)
{
    plan_tests(TEST_COUNT_TOTAL);
    test_s0_names();
    test_s0_name_sets();
    test_s0_atoms();
    test_s0_literals();
    test_s0_objects();
    test_s0_environments();
    return exit_status();
}
