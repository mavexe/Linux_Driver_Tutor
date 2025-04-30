#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>

static struct nf_hook_ops nfho; // Структура для Netfilter

// Функция-обработчик пакетов
static unsigned int log_packet_handler(
    void *priv,
    struct sk_buff *skb,
    const struct nf_hook_state *state
) {
    struct iphdr *ip_header;
    struct udphdr *udp_header;
    struct tcphdr *tcp_header;
    char *protocol;

    // Получаем IP-заголовок
    ip_header = (struct iphdr *)skb_network_header(skb);
    
    // Определяем протокол
    switch (ip_header->protocol) {
        case IPPROTO_TCP:
            protocol = "TCP";
            tcp_header = (struct tcphdr *)(skb_transport_header(skb));
            printk(KERN_INFO "Packet: %s [%d -> %d]\n",
                   protocol,
                   ntohs(tcp_header->source),
                   ntohs(tcp_header->dest));
            break;
        case IPPROTO_UDP:
            protocol = "UDP";
            udp_header = (struct udphdr *)(skb_transport_header(skb));
            printk(KERN_INFO "Packet: %s [%d -> %d]\n",
                   protocol,
                   ntohs(udp_header->source),
                   ntohs(udp_header->dest));
            break;
        default:
            protocol = "OTHER";
            printk(KERN_INFO "Packet: %s\n", protocol);
    }

    return NF_ACCEPT; // Пропускаем пакет дальше
}

// Инициализация модуля
static int __init mod_init(void) {
    nfho.hook     = log_packet_handler;    // Функция-обработчик
    nfho.hooknum  = NF_INET_PRE_ROUTING;  // Точка перехвата (PRE_ROUTING)
    nfho.pf       = PF_INET;              // IPv4
    nfho.priority = NF_IP_PRI_FIRST;      // Высокий приоритет

    nf_register_net_hook(&init_net, &nfho); // Регистрация хука

    printk(KERN_INFO "Ethernet Packet Logger loaded\n");
    return 0;
}

// Выгрузка модуля
static void __exit mod_exit(void) {
    nf_unregister_net_hook(&init_net, &nfho);
    printk(KERN_INFO "Ethernet Packet Logger unloaded\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Damir");
