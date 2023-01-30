typedef struct ethernet_frame {
  unsigned char targetMacEther[6];
  unsigned char senderMacEther[6];
  unsigned char frametype[2];
};

typedef struct arp_frame {

  struct ethernet_frame ext;

  unsigned char hardType[2];
  unsigned char protType[2];
  unsigned char hardSize[1];
  unsigned char protSize[1];
  unsigned char op[2];
  unsigned char senderMac[6];
  unsigned char senderIp[4];
  unsigned char targetMac[6];
  unsigned char targetIp[4];

} ARP;

void ARP_Processing(unsigned char *pkt);
