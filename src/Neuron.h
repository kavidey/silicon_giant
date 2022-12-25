//
// Created by Kavi Dey on 12/22/22.
//

#ifndef SILICON_GIANT_NEURON_H
#define SILICON_GIANT_NEURON_H

#include <list>
#include <memory>

const float BASELINE_CHARGE = -70; // mV
const float DEFAULT_FIRE_CHARGE = 50; // mV
const float ACTION_POTENTIAL_THRESHOLD = 55; // mV
const float MINIMUM_POSSIBLE_CHARGE = -100; // mV
const float CHARGE_DECREASE_RATE = 0.9999; // %
const int REFRACTORY_TIME = 100; // 1 ms

class Synapse;

struct vec2d {
    double x;
    double y;
};

class Neuron {
    int last_fired = -REFRACTORY_TIME;
    bool in_refractory_period = false;
    float charge = BASELINE_CHARGE;
    std::list<std::shared_ptr<Synapse>> outgoing_synapses;
    std::list<std::shared_ptr<Synapse>> incoming_synapses;

    static int global_id;
    int id;

    vec2d pos;

public:
    Neuron();

    /// Simulates the neuron for one timestep
    /// \param timestep current simulation timestep
    void tick(int timestep);

    int getId() const;

    /// Fires the neuron, sending a charge to all of its outgoing synapses
    /// \param timestep current simulation timestep
    void fire(int timestep);

    [[nodiscard]] bool isInRefractoryPeriod() const;

    /// Receives charge from an incoming synapse
    /// \param incoming_charge the amount of electrical charge (in mV)
    void stimulate(float incoming_charge);

    const vec2d &getPos() const;

    void setPos(const vec2d &pos);

    void add_outgoing_synapse(const std::shared_ptr<Synapse>& synapse);
    void add_incoming_synapse(const std::shared_ptr<Synapse>& synapse);

    [[nodiscard]] const std::list<std::shared_ptr<Synapse>> &getOutgoingSynapses() const;

    [[nodiscard]] const std::list<std::shared_ptr<Synapse>> &getIncomingSynapses() const;

    [[nodiscard]] float probe() const;

    void remove_incoming_synapse(const std::shared_ptr<Synapse>& synapse);
    void remove_outgoing_synapse(const std::shared_ptr<Synapse>& synapse);

    void reset();
};


#endif //SILICON_GIANT_NEURON_H
