#include <iostream>
#include "Network.h"
#include "Neuron.h"
#include "Synapse.h"
#include "../lib/silicon_giant/util.h"

int main() {
    Network network = Network();

    auto a = std::make_shared<Neuron>();
    auto b = std::make_shared<Neuron>();

    network.add_neuron(a);
    network.add_neuron(b);

    auto synapse = network.add_connection(a, b);

    for (int i = 0; i < 1000; i++) {
        a->stimulate(50);
        std::cout << a->isInRefractoryPeriod() << std::endl;
        std::cout << b->probe() << std::endl;
        network.tick();
    }
    return 0;
}
