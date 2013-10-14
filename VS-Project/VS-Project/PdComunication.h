#ifndef PD_COMUNICATION_H
#define PD_COMUNICATION_H

#define OSCPKT_OSTREAM_OUTPUT
#include "oscpkt/oscpkt.hh"
#include "oscpkt/udp.hh"

#include "tools.h"
#include "MusicModule.h"

class PdComunication {
public:
	PdComunication();

	void send(std::vector<NoteInformation> noteInformations);
private:
	oscpkt::UdpSocket sock;
};

#endif // PD_COMUNICATION_H