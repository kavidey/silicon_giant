#include <iostream>
#include "Network.h"
#include "Neuron.h"
#include "Synapse.h"
#include "../lib/silicon_giant/util.h"

int main() {
    Network network = Network();

    auto a = std::make_shared<Neuron>();
    auto b = std::make_shared<Neuron>();

    for (int i = 0; i < 10; i++) {
        network.add_neuron(std::make_shared<Neuron>());
    }
    network.add_neuron(a);
    for (int i = 0; i < 100; i++) {
        network.add_neuron(std::make_shared<Neuron>());
    }
    network.add_neuron(b);
    for (int i = 0; i < 10; i++) {
        network.add_neuron(std::make_shared<Neuron>());
    }


    int num_neurons = network.getNumNeurons();
    for (int i = 0; i < num_neurons * 2; i++) {
        bool valid_synapse = false;
        while (!valid_synapse) {
            int pre_index = (int) (randUniform() * num_neurons);
            int post_index = -1;
            while (post_index <= 0 || post_index >= num_neurons-1) {
                post_index = (randNormal() * 20) + pre_index;
            }

            std::shared_ptr<Neuron> pre_neuron = network.getNeuronByIndex(pre_index);
            std::shared_ptr<Neuron> post_neuron = network.getNeuronByIndex(post_index);
            if (post_index != pre_index && !network.are_connected(pre_neuron, post_neuron) && !network.are_connected(post_neuron, pre_neuron)) {
                network.add_connection(pre_neuron, post_neuron);
                valid_synapse = true;
            }
        }
    }

//    std::shared_ptr<Synapse> synapse = network.add_connection(a, b);

    for (int i = 0; i < 100000; i++) {
        a->stimulate(50);
//        std::cout << a->isInRefractoryPeriod() << std::endl;
        std::cout << b->probe() << std::endl;
        network.tick();
    }
    std::cout << network.get_dotvis_representation() << std::endl;
    return 0;
}
