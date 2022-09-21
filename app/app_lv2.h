#ifndef APP_LV2_H_
#define APP_LV2_H_

#include <lilv/lilv.h>

#include "./app_def.h"

class App_LV2 {
protected:
    // LilvWorld* world = lilv_world_new();

public:
    App_LV2()
    {
        LilvWorld* world = lilv_world_new();
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

        // LilvNode* plugin_uri = lilv_new_uri(world, "http://zynaddsubfx.sourceforge.net");
        LilvNode* plugin_uri = lilv_new_uri(world, "http://tytel.org/helm");
        // LilvNode* plugin_uri = lilv_new_uri(world, "http://vital.audio");
        const LilvPlugin* plugin = lilv_plugins_get_by_uri(plugins, plugin_uri);
        // LilvInstance* instance = lilv_plugin_instantiate(plugin, SAMPLE_RATE, NULL);

        uint32_t ports = lilv_plugin_get_num_ports(plugin);
        printf("ports: %d\n", ports);

        // for (uint32_t i = 0; i < ports; i++) {
        //     const LilvPort* p = lilv_plugin_get_port_by_index(plugin, i);
        //     LilvNode* portName = lilv_port_get_name(plugin, p);
        //     printf("(%d) port name: %s\n", i, lilv_node_as_string(portName));
        // }

        lilv_world_free(world);
    }
};

#endif