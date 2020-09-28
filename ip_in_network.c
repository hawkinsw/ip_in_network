#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>

// This code is specifically designed for IPV4.
#define IPV4_ADDR_LENGTH 32

static unsigned ip_in_network(const char *ip,
        const char *network,
        const uint32_t netmask) {
        assert(netmask <= IPV4_ADDR_LENGTH);

        struct in_addr ip_as_addr = {};
        struct in_addr network_as_addr = {};
        const uint32_t netmask_bitcount = IPV4_ADDR_LENGTH - netmask;

        // Convert the ip to an unsigned integer.
        int res = inet_aton(ip, &ip_as_addr);
        assert(res != 0);

        // Convert the network to an unsigned integer. Assume that the network 
        // is really the network and do not apply the netmask to it.
        res = inet_aton(network, &network_as_addr);
        assert(res != 0);

        const uint32_t ip_in_network = ip_as_addr.s_addr & 
                (((unsigned)(-1 << netmask_bitcount))>>netmask_bitcount);
        return network_as_addr.s_addr == ip_in_network;
}

int main() {
        assert(ip_in_network("1.3.8.255", "1.3.0.0", 16));
        assert(!ip_in_network("1.2.8.255", "1.3.0.0", 16));
        assert(ip_in_network("1.2.8.255", "1.2.8.0", 24));
        assert(!ip_in_network("1.2.9.255", "1.2.8.0", 24));
        assert(ip_in_network("128.143.0.0", "128.143.0.0", 24));
        assert(!ip_in_network("128.143.2.0", "128.143.0.0", 24));
}
