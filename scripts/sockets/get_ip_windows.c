// get_ip_windows.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

int get_local_ip(char *buffer, size_t size) {
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    unsigned long flags = GAA_FLAG_INCLUDE_PREFIX;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;

    // Initialiser Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return -1;
    }

    // Obtenir la taille nécessaire
    dwRetVal = GetAdaptersAddresses(AF_INET, flags, NULL, pAddresses, &dwSize);
    if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
        pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(dwSize);
        if (pAddresses == NULL) {
            printf("Erreur d'allocation de mémoire pour les adresses\n");
            WSACleanup();
            return -1;
        }
        dwRetVal = GetAdaptersAddresses(AF_INET, flags, NULL, pAddresses, &dwSize);
    }

    if (dwRetVal != NO_ERROR) {
        printf("GetAdaptersAddresses failed avec le code : %lu\n", dwRetVal);
        if (pAddresses)
            free(pAddresses);
        WSACleanup();
        return -1;
    }

    pCurrAddresses = pAddresses;
    while (pCurrAddresses) {
        if (pCurrAddresses->OperStatus == IfOperStatusUp) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress;
            while (pUnicast) {
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                    struct sockaddr_in *sa_in = (struct sockaddr_in *)pUnicast->Address.lpSockaddr;
                    const char *addr = inet_ntoa(sa_in->sin_addr);
                    if (strcmp(addr, "127.0.0.1") != 0) {
                        strncpy(buffer, addr, size);
                        free(pAddresses);
                        WSACleanup();
                        return 0;
                    }
                }
                pUnicast = pUnicast->Next;
            }
        }
        pCurrAddresses = pCurrAddresses->Next;
    }

    free(pAddresses);
    WSACleanup();
    return -1;
}
