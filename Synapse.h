//
// Created by Kavi Dey on 12/22/22.
//

#ifndef SILICON_GIANT_SYNAPSE_H
#define SILICON_GIANT_SYNAPSE_H

#include <memory>
#include "Neuron.h"

const float BASE_STRENGTH = 0.1;
const float CURRENT_STRENGTH_DECREASE_RATE = 0.99;
const float FIRE_TOGETHER_WIRE_TOGETHER_STRENGTH = 0.1;

// Long Term Potentiation (LTP) consts
const int LTP_TIME = 500;
const float LTP_INFLUENCE = 0.1;

class Synapse {
    float base_strength = BASE_STRENGTH;
    float current_strength = BASE_STRENGTH;
    int last_fired = -LTP_TIME;
    std::shared_ptr<Neuron> post_synaptic_neuron;

public:
    explicit Synapse(const std::shared_ptr<Neuron> &postSynapticNeuron);

    /// Simulates the neuron for one timestep
    /// \param timestep current simulation timestep
    void tick(int timestep);

    /// Fires the synapse, passing the incoming charge to its \p post_synaptic_neuron
    /// \param timestep
    /// \param charge
    void fire(int timestep, float charge);

    /// Callback function to tell the synapse that its post synaptic neuron was fired
    /// Used to help with training via long term potentiation
    /// \param timestep current simulation timestep
    void post_synaptic_neuron_fired(int timestep);

private:
    /// Updates \p base_strength based on the delay between this synapse firing and the \p post_synaptic_neuron firing
    /// If the two neurons fire with in the time interval defined by \p LTP_TIME the synapse will be strengthened,
    /// otherwise it will be weakened.
    ///
    /// The strength of the change is proportional to time difference to \p LTP_TIME
    /// \param timestep
    void long_term_potentiation(int timestep);
};


#endif //SILICON_GIANT_SYNAPSE_H