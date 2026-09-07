#include <mh/testing.hpp>
#include <mh/io/logger.hpp>


TEST(log_test) {
  loggerInit();

  LOG(INFO, "Just test logging %s", "kitty");
  LOG(TRACE, "Just test logging %s", "kitty");
  LOG(DEBUG, "Just test logging %s", "kitty");
  LOG(ERROR, "Just test logging %s", "kitty");
  LOG(FATAL, "Just test logging %s", "kitty");
  LOG("", "Just test logging %s", "kitty");

  loggerCleanup();
}


TEST_LIST(log_test);
