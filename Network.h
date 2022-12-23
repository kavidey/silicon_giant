//
// Created by Kavi Dey on 12/22/22.
//

#ifndef SILICON_GIANT_NETWORK_H
#define SILICON_GIANT_NETWORK_H

#include <list>
#include <memory>
#include "Neuron.h"

class Network {
    std::list<std::shared_ptr<Neuron>> neurons = {};
    std::list<std::shared_ptr<Synapse>> synapses = {};
    int timestep = 0;

public:
    Network();

    void tick();
    void add_neuron(std::shared_ptr<Neuron> neuron);
    void remove_neuron(std::shared_ptr<Neuron> neuron);
    void add_connection(std::shared_ptr<Neuron> pre_synaptic_neuron, std::shared_ptr<Neuron> post_synaptic_neuron);
};


#endif //SILICON_GIANT_NETWORK_H
