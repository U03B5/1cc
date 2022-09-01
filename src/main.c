#include "1cc/1cc.h"

int main(int argc, char **argv) {
  clock_t timer = clock();
  log_register("stderr", stderr, LEVEL_FATAL);
  Allocator *allocator = make_allocator();
  Lexer *lexer = make_lexer(allocator);
  Source *src;
  int i = 0;
  if (argv[1]) {
    src = make_source(argv[1]);
    source_read(src);
    while (token_dump(lexer_get(lexer, src))) {
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
