#include <mh/testing.hpp>
#include <mh/io/json.hpp>
#include <stdio.h>


TEST(json_unmarshal_2_test) {
  Json j{"test_json_2.json"};

  printf("%s\n", j.toString().data);
}


TEST(json_unmarshal_test) {
  Json j{"test_json_1.json"};
  
  printf("%s\n", j.toString().data); 
}


TEST_LIST(json_unmarshal_test, json_unmarshal_2_test);
