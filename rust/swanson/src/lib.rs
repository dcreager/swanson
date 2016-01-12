// -*- coding: utf-8 -*-
// Copyright © 2016, Swanson Project.
// Please see the COPYING file in this distribution for license details.

use std::collections::HashMap;

#[derive(Hash, Eq, PartialEq, Debug)]
struct Name<'a> {
    name: &'a [u8],
}

trait Entity {
}

struct Environment<'a> {
    entries: HashMap<&'a Name, &'a Entity>,
}

#[test]
fn foo() {
    let n1 = Name { name: b"x" };
    let n2 = Name { name: b"x" };
    assert_eq!(n1, n2);
}
