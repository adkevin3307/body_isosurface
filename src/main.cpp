#include <iostream>

#include "Volume.h"

using namespace std;

int main(int argc, char** argv)
{
    Volume volume("Data/Leg_CT.inf", "Data/Leg_CT.raw");

    cout << volume << '\n';

    return 0;
}
