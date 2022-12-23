//
// Created by Kavi Dey on 12/22/22.
//

#include "Synapse.h"
#include "Neuron.h"
#include "Network.h"
#include <string>
#include <format>
#include <algorithm>


Network::Network() = default;

void Network::tick() {
    for (const std::shared_ptr<Neuron> &neuron: neurons) {
        neuron->tick(timestep);
    }

    for (const std::shared_ptr<Synapse> &synapse: synapses) {
        synapse->tick(timestep);
    }

    // TODO: Create and destroy new neurons as well as new connections between neurons

    timestep++;
}

void Network::add_neuron(const std::shared_ptr<Neuron> &neuron) {
    neurons.push_back(neuron);
}

void Network::remove_neuron(const std::shared_ptr<Neuron> &neuron) {
    // TODO: Test this very very heavily
    neurons.remove(neuron);
    for (const std::shared_ptr<Synapse> &synapse: neuron->getIncomingSynapses()) {
        synapse->getPreSynapticNeuron()->remove_outgoing_synapse(synapse);
    }
    for (const std::shared_ptr<Synapse> &synapse: neuron->getOutgoingSynapses()) {
        synapse->getPostSynapticNeuron()->remove_incoming_synapse(synapse);
    }
}

std::shared_ptr<Synapse>
Network::add_connection(const std::shared_ptr<Neuron> &pre_synaptic_neuron,
                        const std::shared_ptr<Neuron> &post_synaptic_neuron) {
    // Make sure that both neurons are in already in the list
    if ((std::find(neurons.begin(), neurons.end(), pre_synaptic_neuron) != neurons.end()) &&
        (std::find(neurons.begin(), neurons.end(), post_synaptic_neuron) != neurons.end())) {
        auto synapse = std::make_shared<Synapse>(pre_synaptic_neuron, post_synaptic_neuron, true);

        synapses.push_back(synapse);
        pre_synaptic_neuron->add_outgoing_synapse(synapse);
        post_synaptic_neuron->add_incoming_synapse(synapse);

        return synapse;
    } else {
        throw std::invalid_argument("one or both of the input neurons have not been added yet");
    }
}

int Network::getTimestep() const {
    return timestep;
}

bool Network::are_connected(const std::shared_ptr<Neuron> &pre_synaptic_neuron,
                            const std::shared_ptr<Neuron> &post_synaptic_neuron) {
//    for (const std::shared_ptr<Synapse>& synapse: pre_synaptic_neuron->getOutgoingSynapses()) {
//        if (synapse->getPostSynapticNeuron() == post_synaptic_neuron) {
//            return true;
//        }
//    }
//    return false;
    return std::any_of(pre_synaptic_neuron->getOutgoingSynapses().begin(),
                       pre_synaptic_neuron->getOutgoingSynapses().end(),
                       [post_synaptic_neuron](const std::shared_ptr<Synapse> &synapse) {
                           return synapse->getPostSynapticNeuron() == post_synaptic_neuron;
                       });
}

const std::shared_ptr<Neuron> &Network::getNeuronByIndex(int i) const {
    auto it = neurons.begin();
    std::advance(it, i);
    return *it;
}

int Network::getNumNeurons() const {
    return neurons.size();
}

std::string Network::get_dotvis_representation() {
    std::string result = "digraph {";
    for (const std::shared_ptr<Synapse>& synapse: synapses) {
//        std::format("{} -> {};", synapse->getPreSynapticNeuron().get(), synapse->getPostSynapticNeuron().get());
        char str[100];
        snprintf(str, 100, R"("%p" -> "%p";)", synapse->getPreSynapticNeuron().get(), synapse->getPostSynapticNeuron().get());
        result += str;
    }
    result += "}";
    return result;
}

std::string Network::get_csv_representation() {
    std::string result = "source, target\n";
    for (const std::shared_ptr<Synapse>& synapse: synapses) {
        char str[100];
        snprintf(str, 100, "%p, %p\n", synapse->getPreSynapticNeuron().get(), synapse->getPostSynapticNeuron().get());
        result += str;
    }
    return result;
}

