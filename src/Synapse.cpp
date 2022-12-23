//
// Created by Kavi Dey on 12/22/22.
//



#include "Synapse.h"
#include "Neuron.h"
#include "../lib/silicon_giant/util.h"

Synapse::Synapse(const std::shared_ptr<Neuron> &postSynapticNeuron, bool random_strength = true)
        : post_synaptic_neuron(move(postSynapticNeuron)) {
    if (random_strength) {
        float rand_strength = rand_float() - 1; // remap it to [-0.5, 0.5]
        base_strength = rand_strength;
        current_strength = rand_strength;
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