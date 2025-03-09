#include "pret_analyse.h"
#include "pret_orient.h"
#include "pret_tocut.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file rotation_angle");
    orient(atof(argv[2]), argv[1]);
    tocut(atof(argv[2]), argv[1]);
    analyse(atof(argv[2]), argv[1]);
    return EXIT_SUCCESS;
}
