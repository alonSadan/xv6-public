#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  if (argc != 3 && !(argc == 4 && strcmp(argv[2], "-s")))
  {
    printf(2, "Usage: ln old new\n");
    exit();
  }

  if (argc == 3)
  {
    //had link
    if (link(argv[1], argv[2]) < 0)
      printf(2, "link %s %s: failed\n", argv[1], argv[2]);

    //soft link
  }
  else
  {
    if (symlink(argv[2], argv[3]) < 0)
      printf(2, "link %s %s: failed\n", argv[2], argv[3]);
  }

  exit();
}
