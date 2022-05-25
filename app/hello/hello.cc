#include <utility/ostream.h>
#include <machine/flash.h>

using namespace EPOS;

OStream cout;

int main()
{
    cout << "Hello world!" << endl;
    int * alloc_flash = new (FLASH) int;
    int * alloc_flas = new (FLASH) int;
    int * alloc_fla = new (FLASH) int;
    *alloc_flash = 5;
    *alloc_flas = 4;
    *alloc_fla = 3;
    cout << "address=" << alloc_flash << ", value=" << *alloc_flash << endl;
    cout << "address=" << alloc_flas << ", value=" << *alloc_flas << endl;
    cout << "address=" << alloc_fla << ", value=" << *alloc_fla << endl;

    return 0;
}
