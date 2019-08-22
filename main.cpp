#include "__header__.h"

int main(int argc, char **argv)
{
    string algorithm = argv[1];
    search_algorithm *search_alg;

    if (algorithm == "hc")
        search_alg = new hc();
}
