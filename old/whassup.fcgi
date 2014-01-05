#!/usr/bin/env perl

use strict;
use warnings;

#use Data::Dumper;
use CGI::Fast qw(:standard);
use URI::Encode;
use IO::Socket::UNIX;

# You have to change this!
my $livestatus_socket = "/usr/local/nagios/var/rw/live";


    while (my $cf = new CGI::Fast)
    {

        my $request = $cf->param('POSTDATA');
        $request =~ s/^data=//;

#       if you want to test with curl, you need to replace newlines
#       $request =~ s/%5Cn/%0A/g;

        my $uri     = URI::Encode->new( { encode_reserved => 0 } );
        my $decoded = $uri->decode($request);



        my $iosocket = IO::Socket::UNIX->new(Peer => $livestatus_socket,
                                                Type     => SOCK_STREAM,
                                                Timeout  => 30 )
                or
                die header(-status=>"500");

        print header;
        print $iosocket $decoded ."\n";

#       should be good, but didn't work
#       shutdown($iosocket,1);

        while(<$iosocket>)
        {
                my $out=$_;
                printf "%s",$out;
        }
        close ($iosocket);
    }
