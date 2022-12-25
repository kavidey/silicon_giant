//
// Created by Kavi Dey on 12/22/22.
//

#include "Neuron.h"
#include "Synapse.h"
#include "../lib/silicon_giant/util.h"

Neuron::Neuron() {}

void Neuron::tick(int timestep) {
    if (in_refractory_period) {
        // Exit refractory period if ready
        if (timestep - last_fired >= REFRACTORY_TIME) {
            in_refractory_period = false;
            charge = BASELINE_CHARGE;
        }
    } else {
        // Update charge and fire if ready
        if (charge >= ACTION_POTENTIAL_THRESHOLD) {
            fire(timestep);
        } else {
            charge = lerp(BASELINE_CHARGE, charge, CHARGE_DECREASE_RATE);
        }
    }
}

void Neuron::fire(int timestep) {
    DEBUG(incoming_synapses.front()->getPostSynapticNeuron() << " fired")
    for (const std::shared_ptr<Synapse>& synapse: outgoing_synapses) {
        synapse->fire(0, DEFAULT_FIRE_CHARGE);
    }

    for (const std::shared_ptr<Synapse>& synapse: incoming_synapses) {
        synapse->post_synaptic_neuron_fired(timestep);
    }

    in_refractory_period = true;
    charge = BASELINE_CHARGE; // I think this is redundant
    last_fired = timestep;
}

void Neuron::stimulate(float incoming_charge) {
    if (!in_refractory_period) {
        charge = std::max(charge + incoming_charge, MINIMUM_POSSIBLE_CHARGE);
    }
}

void Neuron::add_outgoing_synapse(const std::shared_ptr<Synapse>& synapse) {
    outgoing_synapses.push_back(synapse);
}

void Neuron::add_incoming_synapse(const std::shared_ptr<Synapse>& synapse) {
    incoming_synapses.push_back(synapse);
}

float Neuron::probe() const {
    return charge;
}

bool Neuron::isInRefractoryPeriod() const {
    return in_refractory_period;
}

const std::list<std::shared_ptr<Synapse>> &Neuron::getOutgoingSynapses() const {
    return outgoing_synapses;
}

const std::list<std::shared_ptr<Synapse>> &Neuron::getIncomingSynapses() const {
    return incoming_synapses;
}

void Neuron::remove_incoming_synapse(const std::shared_ptr<Synapse>& synapse) {
    incoming_synapses.remove(synapse);
}

void Neuron::remove_outgoing_synapse(const std::shared_ptr<Synapse>& synapse) {
    outgoing_synapses.remove(synapse);
}

void Neuron::reset() {
    last_fired = -REFRACTORY_TIME;
    in_refractory_period = false;
    charge = BASELINE_CHARGE;
}
