//
// Created by Kavi Dey on 12/22/22.
//

#ifndef SILICON_GIANT_NETWORK_H
#define SILICON_GIANT_NETWORK_H

#include <list>
#include <memory>
#include <string>
#include "Neuron.h"

class Network {
    std::list<std::shared_ptr<Neuron>> neurons = {};
    std::list<std::shared_ptr<Synapse>> synapses = {};
    int timestep = 0; // there are 100 steps per ms

public:
    Network();

    ///
    /// \return current simulation timestep
    int getTimestep() const;

    int getNumNeurons() const;
    const std::shared_ptr<Neuron> &getNeuronByIndex(int i) const;

    /// Advances the simulation by one timestep
    void tick();

    /// Adds a neuron to the simulation
    /// \param neuron
    void add_neuron(const std::shared_ptr<Neuron>& neuron);

    /// Removes a neuron from the simulation
    /// \param neuron
    void remove_neuron(const std::shared_ptr<Neuron>& neuron);

    /// Adds a synapse between two neurons
    /// \param pre_synaptic_neuron
    /// \param post_synaptic_neuron
    /// \return a pointer to the newly added synapse
    std::shared_ptr<Synapse> add_connection(const std::shared_ptr<Neuron>& pre_synaptic_neuron, const std::shared_ptr<Neuron>& post_synaptic_neuron);

    void reset();

    /// Checks whether there is a connection from the \p pre_synaptic_neuron to the \p post_synaptic_neuron
    /// \param pre_synaptic_neuron
    /// \param post_synaptic_neuron
    /// \return
    static bool are_connected(const std::shared_ptr<Neuron>& pre_synaptic_neuron, const std::shared_ptr<Neuron>& post_synaptic_neuron);

    std::string get_dotvis_representation();
    std::string get_csv_representation();
};


#endif //SILICON_GIANT_NETWORK_H
