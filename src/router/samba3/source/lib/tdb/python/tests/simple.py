#!/usr/bin/python
# Some simple tests for the Python bindings for TDB
# Note that this tests the interface of the Python bindings
# It does not test tdb itself.
#
# Copyright (C) 2007-2008 Jelmer Vernooij <jelmer@samba.org>
# Published under the GNU LGPLv3 or later

import tdb
from unittest import TestCase
import os, tempfile


class OpenTdbTests(TestCase):
    def test_nonexistant_read(self):
        self.assertRaises(IOError, tdb.Tdb, "/some/nonexistant/file", 0, tdb.DEFAULT, os.O_RDWR)


class SimpleTdbTests(TestCase):
    def setUp(self):
        super(SimpleTdbTests, self).setUp()
        self.tdb = tdb.Tdb(tempfile.mkstemp()[1], 0, tdb.DEFAULT, os.O_CREAT|os.O_RDWR)
        self.assertNotEqual(None, self.tdb)

    def tearDown(self):
        del self.tdb

    def test_repr(self):
        self.assertTrue(repr(self.tdb).startswith("Tdb('"))

    def test_lockall(self):
        self.tdb.lock_all()

    def test_max_dead(self):
        self.tdb.max_dead = 20

    def test_unlockall(self):
        self.tdb.lock_all()
        self.tdb.unlock_all()

    def test_lockall_read(self):
        self.tdb.read_lock_all()
        self.tdb.read_unlock_all()

    def test_reopen(self):
        self.tdb.reopen()

    def test_store(self):
        self.tdb.store("bar", "bla")
        self.assertEquals("bla", self.tdb.get("bar"))

    def test_getitem(self):
        self.tdb["bar"] = "foo"
        self.tdb.reopen()
        self.assertEquals("foo", self.tdb["bar"])

    def test_delete(self):
        self.tdb["bar"] = "foo"
        del self.tdb["bar"]
        self.assertRaises(KeyError, lambda: self.tdb["bar"])
    
    def test_contains(self):
        self.tdb["bla"] = "bloe"
        self.assertTrue("bla" in self.tdb)

    def test_keyerror(self):
        self.assertRaises(KeyError, lambda: self.tdb["bla"])

    def test_hash_size(self):
        self.tdb.hash_size

    def test_map_size(self):
        self.tdb.map_size

    def test_name(self):
        self.tdb.name

    def test_iterator(self):
        self.tdb["bla"] = "1"
        self.tdb["brainslug"] = "2"
        self.assertEquals(["bla", "brainslug"], list(self.tdb))

    def test_items(self):
        self.tdb["bla"] = "1"
        self.tdb["brainslug"] = "2"
        self.assertEquals([("bla", "1"), ("brainslug", "2")], self.tdb.items())

    def test_iteritems(self):
        self.tdb["bloe"] = "2"
        self.tdb["bla"] = "25"
        i = self.tdb.iteritems()
        self.assertEquals(set([("bla", "25"), ("bloe", "2")]),
                              set([i.next(), i.next()]))

    def test_transaction_cancel(self):
        self.tdb["bloe"] = "2"
        self.tdb.transaction_start()
        self.tdb["bloe"] = "1"
        self.tdb.transaction_cancel()
        self.assertEquals("2", self.tdb["bloe"])

    def test_transaction_commit(self):
        self.tdb["bloe"] = "2"
        self.tdb.transaction_start()
        self.tdb["bloe"] = "1"
        self.tdb.transaction_commit()
        self.assertEquals("1", self.tdb["bloe"])

    def test_iterator(self):
        self.tdb["bloe"] = "2"
        self.tdb["bla"] = "hoi"
        i = iter(self.tdb)
        self.assertEquals(set(["bloe", "bla"]), set([i.next(), i.next()]))

    def test_keys(self):
        self.tdb["bloe"] = "2"
        self.tdb["bla"] = "25"
        self.assertEquals(["bla", "bloe"], self.tdb.keys())

    def test_iterkeys(self):
        self.tdb["bloe"] = "2"
        self.tdb["bla"] = "25"
        i = self.tdb.iterkeys()
        self.assertEquals(set(["bloe", "bla"]), set([i.next(), i.next()]))

    def test_values(self):
        self.tdb["bloe"] = "2"
        self.tdb["bla"] = "25"
        self.assertEquals(["25", "2"], self.tdb.values())

    def test_itervalues(self):
        self.tdb["bloe"] = "2"
        self.tdb["bla"] = "25"
        i = self.tdb.itervalues()
        self.assertEquals(set(["25", "2"]), set([i.next(), i.next()]))

    def test_clear(self):
        self.tdb["bloe"] = "2"
        self.tdb["bla"] = "25"
        self.assertEquals(2, len(self.tdb))
        self.tdb.clear()
        self.assertEquals(0, len(self.tdb))

    def test_len(self):
        self.assertEquals(0, len(self.tdb))
        self.tdb["entry"] = "value"
        self.assertEquals(1, len(self.tdb))


if __name__ == '__main__':
    import unittest
    unittest.TestProgram()
