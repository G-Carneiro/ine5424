#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

int main()
{
    cout << "Hello world!" << endl;
    cout << "read cycle first time: "<< PMU::read(0) << endl;
    cout << "read instret first time: "<< PMU::read(2) << endl;
    cout << "read mhpmcounter3 first time: "<< PMU::read(3) << endl;
    cout << "read cycle second time: "<< PMU::read(0) << endl;
    cout << "read instret second time: "<< PMU::read(2) << endl;
    cout << "read mhpmcounter3 second time: "<< PMU::read(3) << endl;

    return 0;
}
