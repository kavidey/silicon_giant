//
// Created by Kavi Dey on 12/22/22.
//

#ifndef SILICON_GIANT_NEURON_H
#define SILICON_GIANT_NEURON_H

#include <list>
#include <memory>

const float DEFAULT_CHARGE = -70; // mV
const float ACTION_POTENTIAL_THRESHOLD = 55; // mV
const float CHARGE_DECREASE_RATE = 0.98; // %
const int REFRACTORY_TIME = 100;

class Synapse;

class Neuron {
    int last_fired = -REFRACTORY_TIME;
    bool in_refractory_period = false;
    float charge = DEFAULT_CHARGE;
    std::list<std::shared_ptr<Synapse>> outgoing_synapses;
    std::list<std::shared_ptr<Synapse>> incoming_synapses;

public:
    Neuron();

    /// Simulates the neuron for one timestep
    /// \param timestep current simulation timestep
    void tick(int timestep);

    /// Fires the neuron, sending a charge to all of its outgoing synapses
    /// \param timestep current simulation timestep
    void fire(int timestep);

    /// Receives charge from an incoming synapse
    /// \param incoming_charge the amount of electrical charge (in mV)
    void receive(float incoming_charge);

    void add_outgoing_synapse(const std::shared_ptr<Synapse>& synapse);
    void add_incoming_synapse(const std::shared_ptr<Synapse>& synapse);
};


#endif //SILICON_GIANT_NEURON_H
