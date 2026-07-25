#include <mh/testing.hpp>
#include <mh/io/json.hpp>
#include <stdio.h>

TEST(json_unmarshal_large_json_test) {
  Json j{"large-file.json"};
  String cont = j.toString();
  printf("%d\n", cont.len);
}

TEST(json_unmarshal_youtube_json_test) {
  Json j{"test_json_5.json"};

  String cont = j.toString();

  //printf("%s\n", cont.data);
  printf("Len of json: %d\n", cont.len);
}


TEST(json_unmarshal_4_test) {
  Json j{"test_json_4.json"};
  String cont = j.toString();
  printf("Len of json: %d\n", cont.len);
}

// TEST(json_unmarshal_2_test) {
//   Json j{"test_json_2.json"};

//   printf("%s\n", j.toString().data);
// }

TEST(json_unmarshal_3_test) {
  Json j{"test_json_3.json"};

  String cont = j.toString();
  printf("Len of json: %d\n", cont.len);
}


TEST(json_unmarshal_test) {
  Json j{"test_json_1.json"};
  String cont = j.toString();
  printf("Len of json: %d\n", cont.len);
}


TEST_LIST(json_unmarshal_test, json_unmarshal_3_test, json_unmarshal_4_test, json_unmarshal_youtube_json_test, json_unmarshal_large_json_test);
