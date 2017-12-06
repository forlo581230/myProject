#ifndef UTITERATOR_H
#define UTITERATOR_H

#include "number.h"
#include "atom.h"
#include "variable.h"
#include "list.h"
#include "struct.h"
#include "iterator.h"

TEST(iterator, first) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    Struct s(Atom("s"), { &one, &t, &Y });
    Iterator<Term*> *itStruct = s.createIterator();

    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    ASSERT_FALSE(itStruct->isDone());
    itStruct->next();
    ASSERT_EQ("t(X, 2)", itStruct->currentItem()->symbol());
    ASSERT_FALSE(itStruct->isDone());
    itStruct->next();
    ASSERT_EQ("Y", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_TRUE(itStruct->isDone());
}

TEST(iterator, nested_iterator) {
  Number one(1);
  Variable X("X");
  Variable Y("Y");
  Number two(2);
  Struct t(Atom("t"), { &X, &two });
  Struct s(Atom("s"), { &one, &t, &Y });
  Iterator<Term*> *it = s.createIterator();
  it->first();
  it->next();

  Iterator<Term*> *it2 = it->currentItem()->createIterator();
  it2->first();
  ASSERT_EQ("X", it2->currentItem()->symbol());
  ASSERT_FALSE(it2->isDone());
  it2->next();
  ASSERT_EQ("2", it2->currentItem()->symbol());
  ASSERT_FALSE(it2->isDone());
  it2->next();
  ASSERT_TRUE(it2->isDone());
}

TEST(iterator, firstList) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    List l({ &one, &t, &Y });

    Iterator<Term*>* itList = l.createIterator();
    itList->first();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    ASSERT_FALSE(itList->isDone());
    itList->next();
    ASSERT_EQ("t(X, 2)", itList->currentItem()->symbol());
    ASSERT_FALSE(itList->isDone());
    itList->next();
    ASSERT_EQ("Y", itList->currentItem()->symbol());
    itList->next();
    ASSERT_TRUE(itList->isDone());
}

TEST(iterator, NullIteratorNumber){
  Number one(1);
  NullIterator<Term*> nullIterator(&one);
  nullIterator.first();
  ASSERT_TRUE(nullIterator.isDone());
  Iterator<Term*> * it = one.createIterator();
  it->first();
  ASSERT_TRUE(it->isDone());

  Atom s("s");
  Iterator<Term*> * it2 = s.createIterator();
  it2->first();
  ASSERT_TRUE(it2->isDone());

  Variable X("X");
  Iterator<Term*> * it3 = X.createIterator();
  it3->first();
  ASSERT_TRUE(it3->isDone());
}


TEST(iterator, DFS_Struct1) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    Struct s(Atom("s"), { &one, &t, &Y });

    Iterator<Term*> *it = s.createDFSIterator();
    it->first();
    EXPECT_EQ("1", it->currentItem()->symbol());
    it->next();
    EXPECT_EQ("t(X, 2)", it->currentItem()->symbol());
    it->next();
    EXPECT_EQ("X", it->currentItem()->symbol());
    it->next();
    EXPECT_EQ("2", it->currentItem()->symbol());
    it->next();
    EXPECT_EQ("Y", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}

TEST(iterator, DFS_Struct2) {
  Atom bun("bun");
  Atom beefPatty("beefPatty");
  Atom shreddedLettuce("shreddedLettuce");
  Atom sauce("sauce");
  Atom cheese("cheese");
  Struct bigMac(Atom("bigMac"),{&bun,&beefPatty,&shreddedLettuce,&sauce,&cheese});

  Atom coke("coke");
  //root
  Struct combo1(Atom("combo1"),{&bigMac,&coke});

  Iterator<Term*> *it = combo1.createDFSIterator();
  it->first();
  EXPECT_EQ("bigMac(bun, beefPatty, shreddedLettuce, sauce, cheese)", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("bun", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("beefPatty", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("shreddedLettuce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("sauce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("cheese", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("coke", it->currentItem()->symbol());
  it->next();
  ASSERT_TRUE(it->isDone());
}

TEST(iterator, DFS_List1) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    List t({ &X, &two });
    List s({ &one, &t, &Y });

    Iterator<Term*> *it = s.createDFSIterator();
    it->first();
    EXPECT_EQ("1", it->currentItem()->symbol());
    it->next();
    EXPECT_EQ("[X, 2]", it->currentItem()->symbol());
    it->next();
    EXPECT_EQ("X", it->currentItem()->symbol());
    it->next();
    EXPECT_EQ("2", it->currentItem()->symbol());
    it->next();
    EXPECT_EQ("Y", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}

TEST(iterator, DFS_List2) {
  Atom bun("bun");
  Atom beefPatty("beefPatty");
  Atom shreddedLettuce("shreddedLettuce");
  Atom sauce("sauce");
  Atom cheese("cheese");
  List bigMac({&bun,&beefPatty,&shreddedLettuce,&sauce,&cheese});

  Atom coke("coke");
  //root
  List combo1({&bigMac,&coke});

  Iterator<Term*> *it = combo1.createDFSIterator();
  it->first();
  EXPECT_EQ("[bun, beefPatty, shreddedLettuce, sauce, cheese]", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("bun", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("beefPatty", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("shreddedLettuce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("sauce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("cheese", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("coke", it->currentItem()->symbol());
  it->next();
  ASSERT_TRUE(it->isDone());
}

TEST(iterator, DFS_StructAndList) {
  Atom pickleSlice1("pickleSlice1");
  Atom pickleSlice2("pickleSlice2");
  Atom onion1("onion1");
  Atom onion2("onion2");
  vector<Term *> args;

  args = {&pickleSlice1, &pickleSlice2};
  List list1(args);
  args = {&onion1, &onion2};
  List list2(args);

  Atom bun("bun");
  Atom beefPatty("beefPatty");
  Atom shreddedLettuce("shreddedLettuce");
  Atom sauce("sauce");
  Atom cheese("cheese");
  Struct bigMac(Atom("bigMac"), {&bun,&beefPatty,&shreddedLettuce,&sauce,&cheese, &list1, &list2});

  Atom coke("coke");
  Atom fries1("fries1");
  Atom fries2("fries2");
  args = {&fries1, &fries2};
  List list3(args);
  //root
  Struct combo1(Atom("combo1"), {&bigMac,&coke,&list3});

  Iterator<Term*> *it = combo1.createDFSIterator();
  it->first();
  EXPECT_EQ("bigMac(bun, beefPatty, shreddedLettuce, sauce, cheese, [pickleSlice1, pickleSlice2], [onion1, onion2])", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("bun", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("beefPatty", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("shreddedLettuce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("sauce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("cheese", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("[pickleSlice1, pickleSlice2]", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("pickleSlice1", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("pickleSlice2", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("[onion1, onion2]", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("onion1", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("onion2", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("coke", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("[fries1, fries2]", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("fries1", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("fries2", it->currentItem()->symbol());
  it->next();
  ASSERT_TRUE(it->isDone());
}

TEST(iterator, BFS_Struct) {
  Atom bun("bun");
  Atom beefPatty("beefPatty");
  Atom shreddedLettuce("shreddedLettuce");
  Atom sauce("sauce");
  Atom cheese("cheese");
  Struct bigMac(Atom("bigMac"),{&bun,&beefPatty,&shreddedLettuce,&sauce,&cheese});

  Atom coke("coke");
  //root
  Struct combo1(Atom("combo1"),{&bigMac,&coke});

  Iterator<Term*> *it = combo1.createBFSIterator();
  it->first();
  EXPECT_EQ("bigMac(bun, beefPatty, shreddedLettuce, sauce, cheese)", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("coke", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("bun", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("beefPatty", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("shreddedLettuce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("sauce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("cheese", it->currentItem()->symbol());
  it->next();
  ASSERT_TRUE(it->isDone());
}

TEST(iterator, BFS_List) {
  Atom bun("bun");
  Atom beefPatty("beefPatty");
  Atom shreddedLettuce("shreddedLettuce");
  Atom sauce("sauce");
  Atom cheese("cheese");
  List bigMac({&bun,&beefPatty,&shreddedLettuce,&sauce,&cheese});

  Atom coke("coke");
  //root
  List combo1({&bigMac,&coke});

  Iterator<Term*> *it = combo1.createBFSIterator();
  it->first();
  EXPECT_EQ("[bun, beefPatty, shreddedLettuce, sauce, cheese]", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("coke", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("bun", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("beefPatty", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("shreddedLettuce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("sauce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("cheese", it->currentItem()->symbol());
  it->next();
  ASSERT_TRUE(it->isDone());
}

TEST(iterator, BFS_StructAndList) {
  Atom pickleSlice1("pickleSlice1");
  Atom pickleSlice2("pickleSlice2");
  Atom onion1("onion1");
  Atom onion2("onion2");
  vector<Term *> args;

  args = {&pickleSlice1, &pickleSlice2};
  List list1(args);
  args = {&onion1, &onion2};
  List list2(args);

  Atom bun("bun");
  Atom beefPatty("beefPatty");
  Atom shreddedLettuce("shreddedLettuce");
  Atom sauce("sauce");
  Atom cheese("cheese");
  Struct bigMac(Atom("bigMac"), {&bun,&beefPatty,&shreddedLettuce,&sauce,&cheese, &list1, &list2});

  Atom coke("coke");
  Atom fries1("fries1");
  Atom fries2("fries2");
  args = {&fries1, &fries2};
  List list3(args);
  //root
  Struct combo1(Atom("combo1"), {&bigMac,&coke,&list3});

  Iterator<Term*> *it = combo1.createBFSIterator();
  it->first();
  EXPECT_EQ("bigMac(bun, beefPatty, shreddedLettuce, sauce, cheese, [pickleSlice1, pickleSlice2], [onion1, onion2])", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("coke", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("[fries1, fries2]", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("bun", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("beefPatty", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("shreddedLettuce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("sauce", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("cheese", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("[pickleSlice1, pickleSlice2]", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("[onion1, onion2]", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("fries1", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("fries2", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("pickleSlice1", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("pickleSlice2", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("onion1", it->currentItem()->symbol());
  it->next();
  EXPECT_EQ("onion2", it->currentItem()->symbol());
  it->next();
  ASSERT_TRUE(it->isDone());
}

#endif