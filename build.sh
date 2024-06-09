set -ex

CFLAGS="-Wall -Wextra -Werror -O0 -g3"
gcc eval_test.c $CFLAGS -o eval_test
gcc eval_app.c $CFLAGS -o eval
./eval_test