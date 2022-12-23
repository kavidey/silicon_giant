//
// Created by Kavi Dey on 12/22/22.
//

#include "Synapse.h"
#include "Neuron.h"
#include "Network.h"
#include <algorithm>


Network::Network() = default;

void Network::tick() {
    for (const std::shared_ptr<Neuron>& neuron: neurons) {
        neuron->tick(timestep);
    }

    for (const std::shared_ptr<Synapse>& synapse: synapses) {
        synapse->tick(timestep);
    }

    // TODO: Create and destroy new neurons as well as new connections between neurons

    timestep++;
}

void Network::add_neuron(const std::shared_ptr<Neuron>& neuron) {
    neurons.push_back(neuron);
}

void Network::remove_neuron(const std::shared_ptr<Neuron>& neuron) {
    neurons.remove(neuron);
}

void
Network::add_connection(const std::shared_ptr<Neuron>& pre_synaptic_neuron, const std::shared_ptr<Neuron>& post_synaptic_neuron) {
    // Make sure that both neurons are in already in the list
    if ((std::find(neurons.begin(), neurons.end(), pre_synaptic_neuron) != neurons.end()) &&
        (std::find(neurons.begin(), neurons.end(), post_synaptic_neuron) != neurons.end())) {
         auto synapse = std::make_shared<Synapse>(post_synaptic_neuron);
         synapses.push_back(synapse);
    } else {
        throw std::invalid_argument( "one or both of the input neurons have not been added yet" );
    }
}