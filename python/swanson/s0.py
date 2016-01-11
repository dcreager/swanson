# -*- coding: utf-8 -*-
# Copyright (c) 2016, Swanson Project.
# Please see the COPYING file in this distribution for license details.

import attrs
from attrs.validators import instance_of
from six import binary_type


@attr.s
class Name(object):
    name = attr.ib(validator=instance_of(binary_type))


class Environment(object):
    def __init__(self):
        self.entries = {}

    def __contains__(self, name):
        return name in self.entries

    def __delitem__(self, name):
        assert name in self.entries
        del self.entries[name]

    def __getitem__(self, name):
        return self.entries[name]

    def __iter__(self):
        return iter(self.entries)

    def __len__(self):
        return len(self.entries)

    def __setitem__(self, name, value):
        assert isinstance(name, Name)
        assert isinstance(value, Entity)
        assert name not in self.entries
        self.entries[name] = value

    def get(self, name):
        return self.entries.get(name)


class Entity(object):
    pass


class Atom(Entity):
    pass


class Block(object):
    def __init__(self, inputs):
        self.inputs = inputs
        self.constructors = []
        self.invocation = None

    def add_constructor(self, constructor):
        assert isinstance(constructor, Constructor)
        assert self.invocation is None
        # TODO: Type-check the constructor
        self.constructors.append(constructor)

    def add_invocation(self, invocation):
        assert isinstance(invocation, Invocation)
        assert self.invocation is None
        # TODO: Type-check the invocation
        self.invocation = invocation


class NamedBlocks(object):
    def __init__(self):
        self.blocks = {}

    def add_block(self, name, block):
        assert isinstance(name, Name)
        assert isinstance(block, Block)
        assert name not in self.blocks
        self.blocks[name] = block


@attr.s
class Closure(Entity):
    closed_environment = attr.ib(validator=instance_of(Environment))
    blocks = attr.ib(validator=instance_of(NamedBlocks))


@attr.s
class Literal(Entity):
    name = attr.ib(validator=instance_of(binary_type))


class Object(Entity):
    pass


class Step(object):
    pass


class Constructor(Step):
    pass


@attr.s
class CreateAtom(Constructor):
    dest = attr.ib(validator=instance_of(Name))


class CreateClosure(Constructor):
    pass


class CreateLiteral(Constructor):
    pass


class CreateObject(Constructor):
    pass


@attr.s
class Rename(Constructor):
    source = attr.ib(validator=instance_of(Name))
    dest = attr.ib(validator=instance_of(Name))


@attr.s
class Invocation(Step):
    source = attr.ib(validator=instance_of(Name))
    branch = attr.ib(validator=instance_of(Name))
