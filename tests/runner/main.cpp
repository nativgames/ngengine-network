/*
  Test Runner
  - Launches the tests
  - Returns the count of errors
*/

#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>

// the list of tests to execute
std::vector<std::string> tests;

// a macro to define tests faster
#define TEST(name) tests.push_back(name);

void init_tests()
{
  // TEST("orm/database/connections/sqlite3");
}

int call_test(std::string command);

void setCurrentDir(const char *argv0);

int main(int argc, char **argv)
{
  setCurrentDir(argv[0]);
  init_tests();
  
  int tests_ok = 0, tests_failed = 0;
  
  int ret;
  
  for(unsigned int i = 0; i < tests.size(); i++) {
    ret = call_test(tests[i]);

    if(ret == 0)
      tests_ok ++;
    else {
      tests_failed ++;
      printf("test %s failed with startus %d\n", tests[i].c_str(), ret);
    }
  }
  
  printf("%d/%d tests success.\n", tests_ok, tests_failed + tests_ok);
  
  return tests_failed;  
}

void setCurrentDir(const char *argv0)
{
    int i, l;	
    char *s;

    s = strdup(argv0);
    l = strlen(s);

    for(i = l - 1; (i >= 0) && (s[i] != '/'); i--)
      ;

    if((i >= 0) && (i < (l - 1)))
            s[i+1] = 0;
    else;

    chdir(s);

    free(s);

    return;
}

int call_test(std::string command)
{
  std::string cmd2 = "./tests/" + command + "/test";
  FILE *f = popen(cmd2.c_str(), "r"); // launch the command
  int ret;

  if(f == NULL) {
    printf("Can't open %s\n", command.c_str());
    ret = -1;
  }
  else {

    std::string t;
    char buf[128];

    while(!feof(f)) {
      if(fgets(buf, 128, f) != NULL)
        t += buf;
    }

    int status = pclose(f);
    ret = WEXITSTATUS(status);
    printf("The test %s exited with code %d\n", command.c_str(), ret);
    if(ret != 0) {
      printf("================= CONTENT OF THE TEST =================\n");
      printf("%s\n", t.c_str());
      printf("=======================================================\n");
    }
  }

  return ret;
}

