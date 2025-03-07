#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIOS_VERSION "1.0.0"
#define MAX_CONFIG_SIZE 1024

typedef struct {
    char ssid[32];
    char password[64];
    char ip_address[16];
    char subnet_mask[16];
    char gateway[16];
    char dns_server[16];
} NetworkConfig;

typedef struct {
    char hostname[64];
    NetworkConfig network_config;
    int channel;
    int tx_power;
} RouterConfig;

void load_default_config(RouterConfig *config) {
    strcpy(config->hostname, "TP-Link AX5400");
    strcpy(config->network_config.ssid, "TP-Link_5400");
    strcpy(config->network_config.password, "admin");
    strcpy(config->network_config.ip_address, "192.168.0.1");
    strcpy(config->network_config.subnet_mask, "255.255.255.0");
    strcpy(config->network_config.gateway, "192.168.0.254");
    strcpy(config->network_config.dns_server, "8.8.8.8");

    config->channel = 6; // Default to channel 6
    config->tx_power = 100; // Default to 100%
}

void print_config(const RouterConfig *config) {
    printf("Hostname: %s\n", config->hostname);
    printf("SSID: %s\n", config->network_config.ssid);
    printf("Password: %s\n", config->network_config.password);
    printf("IP Address: %s\n", config->network_config.ip_address);
    printf("Subnet Mask: %s\n", config->network_config.subnet_mask);
    printf("Gateway: %s\n", config->network_config.gateway);
    printf("DNS Server: %s\n", config->network_config.dns_server);
    printf("Channel: %d\n", config->channel);
    printf("TX Power: %d%%\n", config->tx_power);
}

void save_config(const RouterConfig *config, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(config, sizeof(RouterConfig), 1, file);
    fclose(file);
}

void load_config(RouterConfig *config, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }

    fread(config, sizeof(RouterConfig), 1, file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    RouterConfig config;
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
