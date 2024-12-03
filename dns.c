#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <WinDNS.h>
#include <Windows.h>

int main(int argc, char* argv[]) {
    DNS_STATUS   dnsStat;
    DNS_RECORDW  dnsrRecord; // a linked list.
    PDNS_RECORDW pdnsrRecord = &dnsrRecord;

    for (int i = 0; i < argc; ++i) dnsStat = DnsQuery_UTF8(argv[i], DNS_TYPE_A, DNS_QUERY_USE_TCP_ONLY, NULL, &pdnsrRecord, NULL);
}
