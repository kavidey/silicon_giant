#include <iostream>
#include "Network.h"
#include "Neuron.h"
#include "Synapse.h"

int main() {
    Network network = Network();

    auto a = std::make_shared<Neuron>();
    auto b = std::make_shared<Neuron>();

    network.add_neuron(a);
    network.add_neuron(b);

    auto synapse = network.add_connection(a, b);

    for (int i = 0; i < 10; i++) {
        a->stimulate(50);
        std::cout << "Timestep: " << network.getTimestep() << std::endl;
        std::cout << "Neuron A:" << std::endl;
        std::cout << "  a charge: " << a->probe() << std::endl;
        std::cout << "  a in refractory period: " << a->isInRefractoryPeriod() << std::endl;
        std::cout << std::endl;
        std::cout << "Neuron B:" << std::endl;
        std::cout << "  b charge: " << b->probe() << std::endl;
        std::cout << "  b in refractory period: " << b->isInRefractoryPeriod() << std::endl;
        std::cout << std::endl;
        std::cout << "Synapse:" << std::endl;
        std::cout << "  synapse curr strength: " << synapse->getCurrentStrength() << std::endl;
        std::cout << std::endl;

        network.tick();
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
