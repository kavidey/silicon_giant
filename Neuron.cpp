//
// Created by Kavi Dey on 12/22/22.
//

#include "Neuron.h"
#include "util.h"

Neuron::Neuron() {}

void Neuron::tick(int timestep) {
    if (in_refractory_period) {
        // Exit refractory period if ready
        if (timestep - last_fired < REFRACTORY_TIME) {
            in_refractory_period = false;
            charge = DEFAULT_CHARGE;
        }
    } else {
        // Update charge and fire if ready
        if (charge >= ACTION_POTENTIAL_THRESHOLD) {
            fire(timestep);
        } else {
            charge = lerp(DEFAULT_CHARGE, charge, CHARGE_DECREASE_RATE);
        }
    }
}

void Neuron::fire(int timestep) {
    for (const std::shared_ptr<Synapse>& synapse: outgoing_synapses) {
        synapse->fire(0, charge);
    }

    for (const std::shared_ptr<Synapse>& synapse: incoming_synapses) {
        synapse->post_synaptic_neuron_fired(timestep);
    }

    in_refractory_period = true;
    charge = DEFAULT_CHARGE; // I think this is redundant
}

void Neuron::receive(float incoming_charge) {
    if (!in_refractory_period) {
        charge += incoming_charge;
    }
}

void Neuron::add_outgoing_synapse(const std::shared_ptr<Synapse>& synapse) {
    outgoing_synapses.push_back(synapse);
}

void Neuron::add_incoming_synapse(const std::shared_ptr<Synapse>& synapse) {
    incoming_synapses.push_back(synapse);
}
