#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

int main()
{
    cout << "Hello world!" << endl;
    cout << "read mcycle first time: "<< PMU::read(0) << endl;
    cout << "read minstret first time: "<< PMU::read(2) << endl;
    cout << "read mhpmcounter3 first time: "<< PMU::read(3) << endl;
    cout << "read mcycle second time: "<< PMU::read(0) << endl;
    cout << "read minstret second time: "<< PMU::read(2) << endl;
    cout << "read mhpmcounter3 second time: "<< PMU::read(3) << endl;

    return 0;
}