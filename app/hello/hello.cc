#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

int main()
{
//    char hello[] = {'H', 'e', 'l', 'l', 'o', '\n'};
//    Display::_print(hello);
    int aaa;
    aaa = CPU::mhartid();
    cout << "Hello world! from core n# " << aaa << endl;
    
    return 0;
}
