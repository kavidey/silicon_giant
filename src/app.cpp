#include <iostream>
#include "imgui.h"
#include "implot.h"
#include <boost/circular_buffer.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include "vector"

#include "Network.h"
#include "Neuron.h"
#include "Synapse.h"
#include "../lib/silicon_giant/util.h"
#include "../lib/silicon_giant/ui.h"

std::shared_ptr<Network> network;
std::shared_ptr<Neuron> a;
std::shared_ptr<Neuron> b;

std::vector<std::pair<std::shared_ptr<Neuron>, std::shared_ptr<boost::circular_buffer<float>>>> neuron_monitor;

void setup_network() {
    network = std::make_unique<Network>();

    a = std::make_shared<Neuron>();
    b = std::make_shared<Neuron>();

    // Monitor A and B in the UI
    neuron_monitor.emplace_back(a, std::make_shared<boost::circular_buffer<float>>(GRAPH_DURATION));
    neuron_monitor.emplace_back(b, std::make_shared<boost::circular_buffer<float>>(GRAPH_DURATION));

    for (int i = 0; i < 10; i++) {
        network->add_neuron(std::make_shared<Neuron>());
    }
    network->add_neuron(a);
    for (int i = 0; i < 100; i++) {
        network->add_neuron(std::make_shared<Neuron>());
    }
    network->add_neuron(b);
    for (int i = 0; i < 10; i++) {
        network->add_neuron(std::make_shared<Neuron>());
    }


    int num_neurons = network->getNumNeurons();
    for (int i = 0; i < num_neurons * 20; i++) {
        bool valid_synapse = false;
        while (!valid_synapse) {
            int pre_index = (int) (randUniform() * num_neurons);
            int post_index = -1;
            while (post_index <= 0 || post_index >= num_neurons - 1) {
                post_index = (randNormal() * 20) + pre_index;
            }

            std::shared_ptr<Neuron> pre_neuron = network->getNeuronByIndex(pre_index);
            std::shared_ptr<Neuron> post_neuron = network->getNeuronByIndex(post_index);
            if (post_index != pre_index && !Network::are_connected(pre_neuron, post_neuron) &&
                !Network::are_connected(post_neuron, pre_neuron)) {
                network->add_connection(pre_neuron, post_neuron);
                valid_synapse = true;
            }
        }
    }
}

void render_ui() {
    ImGui::DockSpaceOverViewport();

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
//    window_flags |= ImGuiWindowFlags_NoScrollbar;
//    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
//    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNav;

    ImGui::Begin("Control", nullptr, window_flags);

    ImGui::Text("Framerate: %f", ImGui::GetIO().Framerate);

    // This doesn't work. For some reason it rounds to an int
//    ImGui::Text("Sim Time: %3.f s", ((float) network.getTimestep()) / 1000.0f);
    ImGui::Text("Sim Time: %d us", network->getTimestep());

    ImGui::Separator();

    static bool simulating = false;
    if (!simulating) {
        simulating = ImGui::Button("Start");
    } else {
        simulating = !ImGui::Button("Stop");
    }
    ImGui::SameLine();

    if (ImGui::Button("Reset")) {
        simulating = false;
        network.reset();
    }

    static int sim_speed = 1;
    ImGui::SliderInt("Speed", &sim_speed, 1, 10000, "%dx", ImGuiSliderFlags_Logarithmic);

    ImGui::Spacing();

    bool reset_network_viz = false;
    static bool show_refractory_period = true;
    static float scale = 2.0f;

    if (ImGui::CollapsingHeader("Visualization Settings")) {
        if (ImGui::Button("Reset Network Visualization")) {
            reset_network_viz = true;
        }

        ImGui::SliderFloat("Scale", &scale, 0, 5, "%.3fx");
        ImGui::Checkbox("Display Neuron Refractory Period", &show_refractory_period);
    }

    ImGui::End();

    if (simulating) {
        for (int i = 0; i < sim_speed; i++) {
            a->stimulate(50);
            network->tick();

            for (auto monitor: neuron_monitor) {
                monitor.second->push_back(monitor.first->probe());
            }
        }
    }

    for (auto monitor: neuron_monitor) {
        monitor.second->linearize();
    }

    ImGui::Begin("Monitoring", nullptr, window_flags);
    static ImGuiTableFlags flags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg |
                                   ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
    ImGui::BeginTable("##table", 3, flags, ImVec2(-1, 0));
    ImGui::TableSetupColumn("Neuron ID", ImGuiTableColumnFlags_WidthFixed, 200.0f);
    ImGui::TableSetupColumn("Voltage", ImGuiTableColumnFlags_WidthFixed, 75.0f);
    ImGui::TableSetupColumn("Signal");
    ImGui::TableHeadersRow();
    ImPlot::PushColormap(ImPlotColormap_Cool);

    for (int i = 0; i < neuron_monitor.size(); i++) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Neuron %d", neuron_monitor[i].first->getId());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.2f mV", neuron_monitor[i].first->probe());
        ImGui::TableSetColumnIndex(2);
        ImGui::PushID(i);
        auto buffer_to_array = neuron_monitor[i].second->array_one();
        Sparkline("##spark", buffer_to_array.first, buffer_to_array.second, MINIMUM_POSSIBLE_CHARGE, 100.0f, 0,
                  ImPlot::GetColormapColor(i), ImVec2(-1, 35));
        ImGui::PopID();
    }

    ImPlot::PopColormap();
    ImGui::EndTable();
    ImGui::End();

    ImGui::Begin("Network", nullptr, window_flags);

    // Generate Graph and Positions
    static Graph g = network->to_graph();
    PositionVec position_vec(boost::num_vertices(g));
    PositionMap positions(position_vec.begin(), get(boost::vertex_index, g));

    // Assign random position to all nodes
    static boost::minstd_rand gen;
    const double width = 500;
    const double height = 500;
    static boost::rectangle_topology topo(gen, -width / 2, -height / 2, width / 2, height / 2);
//    boost::random_graph_layout(g, positions, topo);

    for (auto neuron: network->getNeurons()) {
        if (neuron->getPos().x == 0 and neuron->getPos().y == 0) {
            neuron->setPos({(randUniform() - 0.5) * width, (randUniform() - 0.5) * height});
        }
    }

    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        auto neuron = boost::get(boost::vertex_name, g, *vi);
        positions[*vi][0] = neuron->getPos().x;
        positions[*vi][1] = neuron->getPos().y;
    }

    // Pretty layout
    const double max_temp = 100;
    const double step = 0.1;
    const double dec = 0.975;

    static double temp = max_temp;
    int iters = 1;
    const auto custom_cooling = [&, frame_number = 0]() mutable {
        if (iters > 0) {
            iters--;
            temp *= dec;
            return temp;
        } else {
            return (double) 0;
        }
    };

    boost::fruchterman_reingold_force_directed_layout(g, positions, topo, boost::cooling(custom_cooling));

    temp = std::max(temp - step, (double) 0);
    if (reset_network_viz) {
        temp = max_temp;

        for (const auto &neuron: network->getNeurons()) {
            neuron->setPos({0, 0});
        }
    }

    // Setup ImGui
    ImGui::PushItemWidth(-ImGui::GetFontSize() * 15);
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    const ImVec2 p = ImGui::GetCursorScreenPos();

    static float sz = 10.0f;
    float x_offset = p.x + canvas_sz.x / 2 + sz * 0.5f;
    float y_offset = p.y + canvas_sz.y / 2 + sz * 0.5f;

    boost::graph_traits<Graph>::vertex_iterator vj, vj_end;
    for (boost::tie(vj, vj_end) = boost::vertices(g); vj != vj_end; ++vj) {
        boost::get(boost::vertex_name, g, *vj)->setPos({positions[*vj][0], positions[*vj][1]});
    }

    for (const auto &synapse: network->getSynapses()) {
        float strength = std::abs(synapse->getCurrentStrength());
        ImColor col;
        if (synapse->getCurrentStrength() > 0) {
            col = ImColor(0, 255, 0, (int) (strength * 200));
        } else {
            col = ImColor(255, 0, 0, (int) (strength * 200));
        }
        draw_list->AddLine(ImVec2(synapse->getPreSynapticNeuron()->getPos().x * scale + x_offset,
                                  synapse->getPreSynapticNeuron()->getPos().y * scale + y_offset),
                           ImVec2(synapse->getPostSynapticNeuron()->getPos().x * scale + x_offset,
                                  synapse->getPostSynapticNeuron()->getPos().y * scale + y_offset), col,
                           3 * std::abs(synapse->getCurrentStrength()));
    }

    for (const auto &neuron: network->getNeurons()) {
        ImColor col;
        if (neuron->isInRefractoryPeriod() && show_refractory_period) {
            col = ImColor(255, 0, 255);
        } else {
            int charge_amount_color = (int) remap(neuron->probe(), BASELINE_CHARGE, DEFAULT_FIRE_CHARGE, 100, 255);
            col = ImColor(charge_amount_color, charge_amount_color, 255);
        }
        draw_list->AddCircleFilled(ImVec2(neuron->getPos().x * scale + x_offset, neuron->getPos().y * scale + y_offset),
                                   sz * 0.5f, col, 12);
    }

    ImGuiIO &io = ImGui::GetIO();
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
//    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held

    static std::shared_ptr<Neuron> selected_neuron = nullptr;
    if (is_active) {
        for (const auto &neuron: network->getNeurons()) {
            if (dist(io.MousePos.x, io.MousePos.y, neuron->getPos().x * scale + x_offset,
                     neuron->getPos().y * scale + y_offset) <= sz) {
//                std::cout << "selected: " << neuron->getId() << std::endl;
                selected_neuron = neuron;
            }
        }
    }

    ImGui::PopItemWidth();

    ImGui::End();

    ImGui::Begin("Selected", nullptr, window_flags);
    if (selected_neuron != nullptr) {
        ImGui::Text("Neuron %d", selected_neuron->getId());
        ImGui::Text("%p", selected_neuron.get());
        ImGui::Separator();

        ImGui::Spacing();
        ImGui::Text("Refractory Period: %s", selected_neuron->isInRefractoryPeriod() ? "true" : "false");
        ImGui::Text("Charge: %.2f", selected_neuron->probe());

        ImGui::Spacing();
        bool monitored = false;
        for (auto monitor: neuron_monitor) {
            if (monitor.first == selected_neuron) {
                monitored = true;
            }
        }
        if (monitored) ImGui::BeginDisabled(true);
        if (ImGui::Button("Monitor")) {
            neuron_monitor.emplace_back(selected_neuron,
                                        std::make_shared<boost::circular_buffer<float>>(GRAPH_DURATION));
        }
        if (monitored) ImGui::EndDisabled();
    }
    ImGui::End();

    ImGui::ShowDemoWindow();
//    ImPlot::ShowDemoWindow();
}