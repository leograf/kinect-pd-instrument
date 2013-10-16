#include "PdComunication.h"

PdComunication::PdComunication() {
	sock.connectTo(ADDRESS, PORT);
	if (!sock.isOk()) {
		std::cerr << "Error connection to port " << PORT << ": " << sock.errorMessage() << std::endl;
	}
}

void PdComunication::send(std::vector<NoteInformation> noteInformations) {
	if (sock.isOk()) {

		for (int n = 0; n < (int) noteInformations.size(); n++) {
			std::stringstream ss;
			ss << "/note" << n;

			oscpkt::Message msg(ss.str());
			msg.pushFloat(noteInformations[n].quantity);
			msg.pushFloat(noteInformations[n].center);

			oscpkt::PacketWriter pw;
			pw.startBundle().addMessage(msg).endBundle();

			if (!sock.sendPacket(pw.packetData(), pw.packetSize())) {
				std::cout << "Could not sent packet" << std::endl;
			}
		}
	}
}