from __future__ import print_function

from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol


class MulticastPingClient(DatagramProtocol):

    def startProtocol(self):
        # Join the multicast address, so we can receive replies:
        self.transport.joinGroup("239.255.0.1")
        print('subscribed to 239.255.0.1')
        # Send to 228.0.0.5:9999 - all listeners on the multicast address
        # (including us) will receive this message.
        #self.transport.write(b'Client: Ping', ("228.0.0.5", 9999))

    def datagramReceived(self, datagram, address):
        print("Datagram %s received from %s" % (repr(datagram), repr(address)))


reactor.listenMulticast(54321, MulticastPingClient(), listenMultiple=True)
print('started')
reactor.run()
