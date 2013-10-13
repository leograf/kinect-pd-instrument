#include "PdComunication.h"

PdComunication::PdComunication() {
	sock.connectTo(ADDRESS, PORT);
	if (!sock.isOk()) {
		std::cerr << "Error connection to port " << PORT << ": " << sock.errorMessage() << std::endl;
	}
}

void PdComunication::send() {
	if (sock.isOk()) {
		oscpkt::Message msg("/pixeltest"); msg.pushFloat(42.42f);
		oscpkt::PacketWriter pw;
		pw.startBundle().startBundle().addMessage(msg).endBundle().endBundle();
		if (!sock.sendPacket(pw.packetData(), pw.packetSize())) {
			std::cout << "Could not sent packet" << std::endl;
		}
	}
}