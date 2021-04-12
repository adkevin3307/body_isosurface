#include <iostream>

#include "Volume.h"
#include "IsoSurface.h"

using namespace std;

int main(int argc, char** argv)
{
    Volume volume("Data/engine.inf", "Data/engine.raw");

    cout << volume << '\n';

    IsoSurface iso_surface(volume);
    iso_surface.iso_value() = 80.0;

    iso_surface.run();

    cout << iso_surface.vertices().size() << '\n';
    cout << iso_surface.normals().size() << '\n';

    return 0;
}
