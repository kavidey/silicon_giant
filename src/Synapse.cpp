//
// Created by Kavi Dey on 12/22/22.
//



#include "Synapse.h"
#include "Neuron.h"
#include "../lib/silicon_giant/util.h"

int Synapse::global_id = 0;
Synapse::Synapse(const std::shared_ptr<Neuron> &preSynapticNeuron, const std::shared_ptr<Neuron> &postSynapticNeuron, int delay,
                 bool random_strength) : pre_synaptic_neuron(move(preSynapticNeuron)),
                                         post_synaptic_neuron(move(postSynapticNeuron)),
                                         id(global_id++),
                                         fire_delay(delay)
                                         {
    if (random_strength) {
        base_strength = generate_random_strength();
        current_strength = base_strength;
    }
}


void Synapse::fire(int timestep, float charge) {
    to_fire_charge = charge;
    to_fire_time = timestep + fire_delay;
    last_fired = to_fire_time;
    current_strength += FIRE_TOGETHER_WIRE_TOGETHER_STRENGTH;
    base_strength = std::clamp(base_strength, -SYNAPSE_STRENGTH_LIMIT, SYNAPSE_STRENGTH_LIMIT);
}

void Synapse::post_synaptic_neuron_fired(int timestep) {
    long_term_potentiation(timestep);
}

void Synapse::long_term_potentiation(int timestep) {
    int firing_delay = timestep - last_fired;
    float ltp_change = std::clamp((float) (LTP_TIME - firing_delay) * LTP_INFLUENCE, -LTP_INFLUENCE, LTP_INFLUENCE);
    base_strength += ltp_change;
    base_strength = std::clamp(base_strength, -SYNAPSE_STRENGTH_LIMIT, SYNAPSE_STRENGTH_LIMIT);
}

void Synapse::tick(int timestep) {
    current_strength = lerp(base_strength, current_strength, CURRENT_STRENGTH_DECREASE_RATE);
    if (timestep == to_fire_time) {
        post_synaptic_neuron->stimulate(to_fire_charge * current_strength);
    }
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

void Synapse::set_strength(bool random_strength) {
    if (random_strength) {
        base_strength = generate_random_strength();

    } else {
        base_strength = DEFAULT_BASE_STRENGTH;
    }
    current_strength = base_strength;
}

void Synapse::set_strength(float strength) {
    base_strength = strength;
    current_strength = base_strength;
}

void Synapse::reset() {
    last_fired = -LTP_TIME;
    to_fire_time = -LTP_TIME;
}

float Synapse::generate_random_strength() {
    float rand_strength = randUniform() - 0.4;
    rand_strength *= 2;

    return rand_strength;
}

int Synapse::getId() const {
    return id;
}
