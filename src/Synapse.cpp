//
// Created by Kavi Dey on 12/22/22.
//



#include "Synapse.h"
#include "Neuron.h"
#include "../lib/silicon_giant/util.h"

Synapse::Synapse(const std::shared_ptr<Neuron> &preSynapticNeuron, const std::shared_ptr<Neuron> &postSynapticNeuron,
                 bool random_strength) : pre_synaptic_neuron(move(preSynapticNeuron)),
                                         post_synaptic_neuron(move(postSynapticNeuron)) {
    if (random_strength) {
        base_strength = generate_random_strength();
        current_strength = base_strength;
    }
}

void Synapse::fire(int timestep, float charge) {
    post_synaptic_neuron->stimulate(charge * current_strength);
    current_strength += FIRE_TOGETHER_WIRE_TOGETHER_STRENGTH;
    last_fired = timestep;
}

void Synapse::post_synaptic_neuron_fired(int timestep) {
    long_term_potentiation(timestep);
}

void Synapse::long_term_potentiation(int timestep) {
    int firing_delay = timestep - last_fired;
    float ltp_change = ((float) (LTP_TIME - firing_delay)) * LTP_INFLUENCE;
    base_strength += ltp_change;
}

void Synapse::tick(int timestep) {
    current_strength = lerp(base_strength, current_strength, CURRENT_STRENGTH_DECREASE_RATE);
}

float Synapse::getCurrentStrength() const {
    return current_strength;
}

const std::shared_ptr<Neuron> &Synapse::getPreSynapticNeuron() const {
    return pre_synaptic_neuron;
}

const std::shared_ptr<Neuron> &Synapse::getPostSynapticNeuron() const {
    return post_synaptic_neuron;
}

void Synapse::reset(bool random_strength) {
    if (random_strength) {
        base_strength = generate_random_strength();

    } else {
        base_strength = DEFAULT_BASE_STRENGTH;
    }
    current_strength = base_strength;

    last_fired = -LTP_TIME;
}

float Synapse::generate_random_strength() {
    float rand_strength = randUniform() - 0.5; // remap it to [-0.5, 0.5]
    rand_strength *= 2;

    return rand_strength;
}
