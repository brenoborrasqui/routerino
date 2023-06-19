void ENC28J60_Print_packet(unsigned char *packet, int len) {
  if (debug)Serial.println("");
  for (int i = 0; i < len; i++) {
    if (*(packet + i) == '/0') {
      if (debug)Serial.print("FIM");
      return 0;
    }
    if (debug)Serial.print(*(packet + i), HEX);
    if (debug)Serial.print("|");
  }
  if (debug)Serial.println();
}

typedef struct ethernet_frame {
  unsigned char targetMacEther[6];
  unsigned char senderMacEther[6];
  unsigned char frametype[2];
} ether;

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

typedef struct ip_frame {

  struct ethernet_frame ext;

  unsigned char vers_IHL[1];
  unsigned char serviceType[1];
  unsigned char totalLength[2];
  unsigned char identification[2];
  unsigned char flags_fragmOffset[2];
  unsigned char timeLive[1];
  unsigned char protoc[1];
  unsigned char headChecksum[2];
  unsigned char senderIP[4];
  unsigned char targetIP[4];
} FrameIP;

typedef struct frame_ICMP {

  struct ip_frame ext2;

  unsigned char type[1];
  unsigned char code[1];
  unsigned char checksum[2];
  unsigned char mensage[60];
} ICMP;

extern unsigned char arp_request[];
extern ether *ptr;
extern ether *reply;

int polling() {
  while (1)
  {
    if (ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa0) != 0)
    {
      if (debug)Serial.println();
      if (debug)Serial.print("Chip 0 - PACOTES:");
      if (debug)Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa0));
      return placa0;
    }
    else if (ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa1) != 0)
    {
      if (debug)Serial.println();
      if (debug)Serial.print("Chip 1 - PACOTES:");
      if (debug)Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa1));
      return placa1;
    }
    else if (ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa2) != 0)
    {
      if (debug)Serial.println();
      if (debug)Serial.print("Chip 2 - PACOTES:");
      if (debug)Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa2));
      return placa2;
    }
  }
}

void arp_analysis(int chip, unsigned char *packet)
{
  ptr = (ether *)packet;
  int flag = 0;

  for (int i = 0; i < 6; i++)
  {
    if (tabela[chip].MAC[i] == ptr->senderMacEther[i])
    {
      flag = 0;
    }
    else
    {
      flag = 1;
    }
  }

  if (flag == 1)
  {
    for (int i = 0; i < 6; i++)
    {
      tabela[chip].MAC[i] = ptr->senderMacEther[i];
    }
  }
}

void chooseport(int chip) {

  if ( (reply->frametype[0] == 0x08) && (reply->frametype[1] == 0x06) && (reply->targetMacEther[0] = 0xff) && (reply->targetMacEther[0] = 0xff) && (reply->targetMacEther[1] = 0xff) && (reply->targetMacEther[2] = 0xff) && (reply->targetMacEther[3] = 0xff) && (reply->targetMacEther[4] = 0xff) && (reply->targetMacEther[5] = 0xff) )
  {

    if (chip == placa0) {
      ENC28J60_Send_Packet(packet, len, placa1);
      ENC28J60_Send_Packet(packet, len, placa2);
    }
    else if (chip == placa1) {
      ENC28J60_Send_Packet(packet, len, placa0);
      ENC28J60_Send_Packet(packet, len, placa2);
    }
    else if (chip == placa2) {
      ENC28J60_Send_Packet(packet, len, placa0);
      ENC28J60_Send_Packet(packet, len, placa1);
    }
  }
  else
  {
    if (chip == placa0) {
      if ((reply->targetMacEther[0] == tabela[placa1].MAC[0]) && (reply->targetMacEther[1] == tabela[placa1].MAC[1]) && (reply->targetMacEther[2] == tabela[placa1].MAC[2]) && (reply->targetMacEther[3] == tabela[placa1].MAC[3]) && (reply->targetMacEther[4] == tabela[placa1].MAC[4]) && (reply->targetMacEther[5] == tabela[placa1].MAC[5]))
      {
        ENC28J60_Send_Packet(packet, len, placa1);
      }
      else if ((reply->targetMacEther[0] == tabela[placa2].MAC[0]) && (reply->targetMacEther[1] == tabela[placa2].MAC[1]) && (reply->targetMacEther[2] == tabela[placa2].MAC[2]) && (reply->targetMacEther[3] == tabela[placa2].MAC[3]) && (reply->targetMacEther[4] == tabela[placa2].MAC[4]) && (reply->targetMacEther[5] == tabela[placa2].MAC[5]))
      {
        ENC28J60_Send_Packet(packet, len, placa2);
      }
    }

    else if (chip == placa1) {
      if ((reply->targetMacEther[0] == tabela[placa0].MAC[0]) && (reply->targetMacEther[1] == tabela[placa0].MAC[1]) && (reply->targetMacEther[2] == tabela[placa0].MAC[2]) && (reply->targetMacEther[3] == tabela[placa0].MAC[3]) && (reply->targetMacEther[4] == tabela[placa0].MAC[4]) && (reply->targetMacEther[5] == tabela[placa0].MAC[5]))
      {
        ENC28J60_Send_Packet(packet, len, placa0);
      }
      else if ((reply->targetMacEther[0] == tabela[placa2].MAC[0]) && (reply->targetMacEther[1] == tabela[placa2].MAC[1]) && (reply->targetMacEther[2] == tabela[placa2].MAC[2]) && (reply->targetMacEther[3] == tabela[placa2].MAC[3]) && (reply->targetMacEther[4] == tabela[placa2].MAC[4]) && (reply->targetMacEther[5] == tabela[placa2].MAC[5]))
      {
        ENC28J60_Send_Packet(packet, len, placa2);
      }
    }

    else if (chip == placa2) {
      if ((reply->targetMacEther[0] == tabela[placa0].MAC[0]) && (reply->targetMacEther[1] == tabela[placa0].MAC[1]) && (reply->targetMacEther[2] == tabela[placa0].MAC[2]) && (reply->targetMacEther[3] == tabela[placa0].MAC[3]) && (reply->targetMacEther[4] == tabela[placa0].MAC[4]) && (reply->targetMacEther[5] == tabela[placa0].MAC[5]))
      {
        ENC28J60_Send_Packet(packet, len, placa0);
      }
      else if ((reply->targetMacEther[0] == tabela[placa1].MAC[0]) && (reply->targetMacEther[1] == tabela[placa1].MAC[1]) && (reply->targetMacEther[2] == tabela[placa1].MAC[2]) && (reply->targetMacEther[3] == tabela[placa1].MAC[3]) && (reply->targetMacEther[4] == tabela[placa1].MAC[4]) && (reply->targetMacEther[5] == tabela[placa1].MAC[5]))
      {
        ENC28J60_Send_Packet(packet, len, placa1);
      }
    }
  }
}
