#ifndef APP_LV2_H_
#define APP_LV2_H_

#include <lilv/lilv.h>

#include "./app_def.h"

class App_LV2 {
protected:
    LilvWorld* world;
    LilvInstance* instance;

    LilvNode* lv2_AudioPort;
    LilvNode* lv2_OutputPort;

    float audio_out;
    // float audio_out[128];
    uint8_t audio = 0;

    float control_in_1 = 0.0f;
    float control_in_2 = 0.0f;

public:
    App_LV2()
    {
        world = lilv_world_new();

        lv2_AudioPort = lilv_new_uri(world, LV2_CORE__AudioPort);
        lv2_OutputPort = lilv_new_uri(world, LV2_CORE__OutputPort);
        lilv_world_load_all(world);

        const LilvPlugins* plugins = lilv_world_get_all_plugins(world);
        LILV_FOREACH(plugins, i, plugins)
        {
            const LilvPlugin* p = lilv_plugins_get(plugins, i);
            LilvNode* n = lilv_plugin_get_name(p);
            printf("node: %s\n", lilv_node_as_string(n));
            lilv_node_free(n);
            printf("uri: %s\n", lilv_node_as_uri(lilv_plugin_get_uri(p)));
        }

        // LilvNode* plugin_uri = lilv_new_uri(world, "http://code.google.com/p/amsynth/amsynth");
        LilvNode* plugin_uri = lilv_new_uri(world, "http://tytel.org/helm");
        // LilvNode* plugin_uri = lilv_new_uri(world, "http://geontime.com/geonkick/single");
        // LilvNode* plugin_uri = lilv_new_uri(world, "http://geontime.com/geonkick");
        // LilvNode* plugin_uri = lilv_new_uri(world, "http://zynaddsubfx.sourceforge.net");
        // LilvNode* plugin_uri = lilv_new_uri(world, "http://samplv1.sourceforge.net/lv2");
        // LilvNode* plugin_uri = lilv_new_uri(world, "http://vital.audio");
        const LilvPlugin* plugin = lilv_plugins_get_by_uri(plugins, plugin_uri);
        lilv_node_free(plugin_uri);

        instance = lilv_plugin_instantiate(plugin, SAMPLE_RATE, NULL);

        uint32_t ports = lilv_plugin_get_num_ports(plugin);
        printf("ports: %d\n", ports);

        printf("audio port count: %d\n", lilv_plugin_get_num_ports_of_class(plugin, lv2_AudioPort, NULL));
        printf("out port count: %d\n", lilv_plugin_get_num_ports_of_class(plugin, lv2_OutputPort, NULL));

        for (uint32_t i = 0; i < ports; i++) {
            const LilvPort* port = lilv_plugin_get_port_by_index(plugin, i);
            LilvNode* portName = lilv_port_get_name(plugin, port);
            printf("(%d) port name: %s\n", i, lilv_node_as_string(portName));
            // LilvNodes* properties = lilv_port_get_properties(plugin, port);
            // LILV_FOREACH(nodes, i, properties)
            // {
            //     printf("\t%s\n", lilv_node_as_uri(lilv_nodes_get(properties, i)));
            // }

            // const LilvNodes* classes = lilv_port_get_classes(plugin, port);
            // LILV_FOREACH(nodes, i, classes)
            // {
            //     printf("\t%s\n", lilv_node_as_string(lilv_nodes_get(classes, i)));
            // }

            if (lilv_port_is_a(plugin, port, lv2_AudioPort) && lilv_port_is_a(plugin, port, lv2_OutputPort)) {
                printf("\t\taudio out\n");
                if (audio == 0) {
                    lilv_instance_connect_port(instance, i, &audio_out);
                }
                audio++;
            }
        }
        // lilv_instance_connect_port(instance, 2, &audio_out);
        // lilv_instance_connect_port(instance, 10, &control_in_1);

        lilv_instance_activate(instance);
    }

    ~App_LV2()
    {
        lilv_instance_deactivate(instance);

        lilv_node_free(lv2_AudioPort);
        lilv_node_free(lv2_OutputPort);

        lilv_instance_free(instance);

        lilv_world_free(world);
    }

    int16_t sample() {
        lilv_instance_run(instance, 1);
        return (int16_t)(audio_out * 10000.0f);
    }
};

#endif