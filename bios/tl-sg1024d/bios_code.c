#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIOS_VERSION "1.0.0"
#define MAX_CONFIG_SIZE 1024

typedef struct {
    char ip_address[16];
    char subnet_mask[16];
    char gateway[16];
    char dns_server[16];
} NetworkConfig;

typedef struct {
    char hostname[64];
    NetworkConfig network_config;
    int port_speed[24];
    int vlan_config[24];
} SwitchConfig;

void load_default_config(SwitchConfig *config) {
    strcpy(config->hostname, "TL-SG1024D");
    strcpy(config->network_config.ip_address, "192.168.0.1");
    strcpy(config->network_config.subnet_mask, "255.255.255.0");
    strcpy(config->network_config.gateway, "192.168.0.254");
    strcpy(config->network_config.dns_server, "8.8.8.8");

    for (int i = 0; i < 24; i++) {
        config->port_speed[i] = 1000; // Default to 1Gbps
        config->vlan_config[i] = 1; // Default to VLAN 1
    }
}

void print_config(const SwitchConfig *config) {
    printf("Hostname: %s\n", config->hostname);
    printf("IP Address: %s\n", config->network_config.ip_address);
    printf("Subnet Mask: %s\n", config->network_config.subnet_mask);
    printf("Gateway: %s\n", config->network_config.gateway);
    printf("DNS Server: %s\n", config->network_config.dns_server);

    for (int i = 0; i < 24; i++) {
        printf("Port %d Speed: %d Mbps\n", i + 1, config->port_speed[i]);
        printf("Port %d VLAN: %d\n", i + 1, config->vlan_config[i]);
    }
}

void save_config(const SwitchConfig *config, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(config, sizeof(SwitchConfig), 1, file);
    fclose(file);
}

void load_config(SwitchConfig *config, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }

    fread(config, sizeof(SwitchConfig), 1, file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    SwitchConfig config;
    load_default_config(&config);

    if (argc > 1 && strcmp(argv[1], "load") == 0 && argc == 3) {
        load_config(&config, argv[2]);
    } else if (argc > 1 && strcmp(argv[1], "save") == 0 && argc == 3) {
        save_config(&config, argv[2]);
    } else {
        print_config(&config);
    }

    return 0;
}
