#include <mh/testing.hpp>
#include <mh/core.hpp>
#include <mh/containers/list.hpp>

TEST(list_huge_insert_test) {
  const i32 count = 2000000000;
  List<i32> l;

  for(i32 i = 0; i < count; i++) {
    l.append(i);
  }

  assertTrue(l.len == count);
  assertEql(l[count - 1], 2000000000 - 1);
}


TEST(list_clear_test) {
  List<i32> l;
  l.append(1);
  l.append(2);
  l.append(3);
  l.append(4);
  l.append(5);

  l.clear();
  assertTrue(l.len == 0);
  auto res = l.get(1);
  assertTrue(!res.ok);

}


TEST(list_get_test) {
  List<i32> l;
  l.append(1);
  l.append(2);
  l.append(3);
  l.append(4);
  l.append(5);

  Optional<i32> res = l.get(1);
  assertTrue(res.ok);
  assertEql(&res, 2);

  &res = 9;
  assertTrue(l[1] == 9);

  res = l.get(8);
  assertTrue(!res.ok);
}

TEST(list_contain_test) {
  List<i32> l;
  l.append(1);
  l.append(2);
  l.append(3);
  l.append(4);
  l.append(5);
  assertTrue(l.contain(3));
}

TEST(list_remove_odered_test) {
  List<i32> l;
  l.append(1);
  l.append(2);
  l.append(3);
  l.append(4);
  l.append(5);

  l.removeOrdered(1);
  assertTrue(l.len == 4);
  assertEql(3, l[1]);
}

TEST(list_remove_unordered_test) {
  List<i32> l;
  l.append(1);
  l.append(2);
  l.append(3);
  l.append(4);
  l.append(5);
  
  l.removeUnordered(1);
  assertTrue(l.len == 4);
  assertEql(5, l[1]);
}




TEST(list_insert_many_test) {
  List<i32> l;
  l.append(1);
  l.append(2);
  l.append(3);
  l.append(4);
  l.append(5);


  i32 arr[5] = {10, 9, 8, 7, 6};
  l.insert(arr, 5, 2);
  assertTrue(l.len == 10);
  assertEql(9, l[3]);
}

TEST(list_append_many_test) {
  List<i32> l;
  l.append(1);
  l.append(2);
  l.append(3);
  l.append(4);
  l.append(5);

  i32 arr[5] = {6, 7, 8, 9, 10};
  l.append(arr, 5);

  assertTrue(l.len == 10);
  assertEql(10, l[9]);
}

TEST(list_insert_test) {
  List<i32> l;
  l.append(88);
  l.append(8);
  l.append(78);
  l.append(44);
  l.append(55);
  l.append(11);

  l.insert(0, 1);

  assertTrue(l.len == 7);
  assertEql(0, l[1]);
}


TEST(list_create_test) {
  List<i32> a;
  a.append(123123);
  a.append(1123);
  a.append(7777);

  assertTrue(a.len == 3);
  assertTrue(a[2] == 7777);
}

TEST_LIST(list_create_test, 
    list_insert_test, 
    list_append_many_test, 
    list_insert_many_test, 
    list_remove_unordered_test,
    list_remove_odered_test,
    list_contain_test,
    list_get_test,
    list_clear_test,
    list_huge_insert_test
    );
