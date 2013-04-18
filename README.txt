
Whassup is a Monitoring Frontend.

License: GPL
Author Rene Storm 2011
Version: 0.8

Requirements:
nagios/Incinga etc.
Livestatus (don't have it, get it)

Installation:
Execute whassup_server.pl on your monitoring server.In some cases you may need to edit your livestatus path and your port.
whassup_server.pl just translate livestatus socket to the network, if you have something like that (maybe you are using xinetd for that), you won't need this part.

execute whassup.exe. Configure on the Config screen. whassup.cfg will be written to your Home folder.
On some systems administrativ rigths may be nessessary.

Have fun,
Rene

