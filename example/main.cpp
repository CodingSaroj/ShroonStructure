#include "Shroon/Structure/Tree.hpp"
#include "Shroon/Structure/GenericView.hpp"

SHRN_STRUCTURE_DEFAULT_ERROR_REPORTER

using namespace Shroon::Structure;

int main()
{
    Tree<std::string> tree("+");

    tree.GetRoot().AddChild("*").AddChild("1").AddSibling("2");
    tree.GetRoot().AddChild("*").AddChild("3").AddSibling("4");

    std::cout<<"Tree example:\n";
    std::cout<<tree<<"\n";

    std::string * strdata = new std::string[8]
    {
        "Hello",
        "Generic",
        "View!",
        "This memory",
        "is",
        "never",
        "copied",
        "again!"
    };

    GenericView<std::string> strView(strdata, 8);

    std::cout<<"Generic view example (of std::string):\n";
    std::cout<<strView<<"\n";

    uint8_t * memData = new uint8_t[256];

    for (uint16_t i = 0; i < 256; i++)
    {
        memData[i] = i;
    }

    MemoryView memView(memData, 256);

    std::cout<<"Memory view (Generic view of uint8_t) example:\n";
    std::cout<<memView<<"\n";

    MemoryView memView1(memView.View(128));

    std::cout<<"Memory view split example (second half of previous view):\n";
    std::cout<<memView1<<"\n";
}
