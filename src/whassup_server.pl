#!/usr/bin/env perl
use IO::Socket;
use IO::Socket::UNIX;
use Data::Dumper;

use warnings;
use strict;

my $server_port = 6557;
my $livestatus_socket = "/var/nagios/var/rw/live";


my $server = IO::Socket::INET->new(LocalPort => $server_port,
                                Type      => SOCK_STREAM,
                                Reuse     => 1,
                                Listen    => 10 )
    or die "Couldn't be a tcp server on port $server_port : $@\n";

while (my $client = $server->accept()) {
   #print Dumper($client) . "is the new connection\n";
        my $client_data;
        $client->recv($client_data,10000);

        my $iosocket = IO::Socket::UNIX->new(Peer => $livestatus_socket,
                                                Type     => SOCK_STREAM,
                                                Timeout  => 15 )
                                or die "Couldn't open $livestatus_socket:  $@\n";;
        print $iosocket "$client_data\n";
        while(<$iosocket>)
        {
                printf $client "%s",$_;
        }
        close ($iosocket);
}

close($server);
