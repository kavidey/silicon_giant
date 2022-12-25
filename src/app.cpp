#include <iostream>
#include "imgui.h"
#include "implot.h"
#include <boost/circular_buffer.hpp>

#include "Network.h"
#include "Neuron.h"
#include "Synapse.h"
#include "../lib/silicon_giant/util.h"
#include "../lib/silicon_giant/ui.h"

Network network;
std::shared_ptr<Neuron> a;
std::shared_ptr<Neuron> b;

void setup() {
    network = Network();

    a = std::make_shared<Neuron>();
    b = std::make_shared<Neuron>();

    for (int i = 0; i < 10; i++) {
        network.add_neuron(std::make_shared<Neuron>());
    }
    network.add_neuron(a);
    for (int i = 0; i < 100; i++) {
        network.add_neuron(std::make_shared<Neuron>());
    }
    network.add_neuron(b);
    for (int i = 0; i < 10; i++) {
        network.add_neuron(std::make_shared<Neuron>());
    }


    int num_neurons = network.getNumNeurons();
    for (int i = 0; i < num_neurons * 20; i++) {
        bool valid_synapse = false;
        while (!valid_synapse) {
            int pre_index = (int) (randUniform() * num_neurons);
            int post_index = -1;
            while (post_index <= 0 || post_index >= num_neurons-1) {
                post_index = (randNormal() * 20) + pre_index;
            }

            std::shared_ptr<Neuron> pre_neuron = network.getNeuronByIndex(pre_index);
            std::shared_ptr<Neuron> post_neuron = network.getNeuronByIndex(post_index);
            if (post_index != pre_index && !network.are_connected(pre_neuron, post_neuron) && !network.are_connected(post_neuron, pre_neuron)) {
                network.add_connection(pre_neuron, post_neuron);
                valid_synapse = true;
            }
        }
    }

//    std::shared_ptr<Synapse> synapse = network.add_connection(a, b);

//    while (b->probe() == -70) {
//        a->stimulate(50);
//        network.tick();
//    }
//
//    for (int i = 0; i < 100; i++) {
//        a->stimulate(50);
//        std::cout << b->probe() << std::endl;
//        network.tick();
//    }

//    for (std::shared_ptr<Synapse> synapse: a->getOutgoingSynapses()) {
//        std::cout << synapse->getPostSynapticNeuron() << " " << synapse->getCurrentStrength() << " " << synapse->getPostSynapticNeuron()->probe() << std::endl;
//    }

//    std::cout << network.get_dotvis_representation() << std::endl;
//    std::cout << network.get_csv_representation() << std::endl;
}

void render_ui() {
    ImGui::Begin("Settings");
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
    ImGui::DragInt("Simulation Speed", &sim_speed, 0, 1, 10, "%dx");

    static boost::circular_buffer<float> a_charge(500);
    static boost::circular_buffer<float> b_charge(500);
    if (simulating) {
        for (int i = 0; i < sim_speed; i++) {
            a->stimulate(50);
            network.tick();

            a_charge.push_back(a->probe());
            b_charge.push_back(b->probe());
        }
    }

    a_charge.linearize();
    b_charge.linearize();

    static ImGuiTableFlags flags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                   ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
    ImGui::BeginTable("##table", 3, flags, ImVec2(-1, 0));
    ImGui::TableSetupColumn("Neuron ID", ImGuiTableColumnFlags_WidthFixed, 1000.0f);
    ImGui::TableSetupColumn("Voltage", ImGuiTableColumnFlags_WidthFixed, 75.0f);
    ImGui::TableSetupColumn("Signal");
    ImGui::TableHeadersRow();
    ImPlot::PushColormap(ImPlotColormap_Cool);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%p", a.get());
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%.2f mV", a->probe());
    ImGui::TableSetColumnIndex(2);
    ImGui::PushID(0);
    Sparkline("##spark", a_charge.array_one().first, a_charge.array_one().second, -100.0f, 100.0f, 0, ImPlot::GetColormapColor(0),ImVec2(-1, 35));
    ImGui::PopID();

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%p", b.get());
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%.2f mV", b->probe());
    ImGui::TableSetColumnIndex(2);
    ImGui::PushID(1);
    Sparkline("##spark", b_charge.array_one().first, b_charge.array_one().second, -100.0f, 100.0f, 0, ImPlot::GetColormapColor(1),ImVec2(-1, 35));
    ImGui::PopID();

    ImPlot::PopColormap();
    ImGui::EndTable();

    ImGui::End();

//    ImGui::ShowDemoWindow();
//    ImPlot::ShowDemoWindow();
}