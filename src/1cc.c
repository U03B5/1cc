#include "1cc/1cc.h"

int main(int argc, char** argv) {
  clock_t timer = clock();
  log_register("stderr", stderr, LEVEL_FATAL);
  Allocator* allocator = make_allocator();
  Lexer* lexer = make_lexer(allocator);
  int i = 0;
  if (argv[1]) {
    lexer_register(lexer, make_source(argv[1]));
    while (token_dump(lexer_get(lexer))) {
      ++i;
    }
  }
  lexer_destroy(lexer);
  allocator_destroy(allocator);
  log_shutdown();
  printf("Execution time: %lf\n%d\n",
         (double)(clock() - timer) / CLOCKS_PER_SEC, i);
  return 0;
}
