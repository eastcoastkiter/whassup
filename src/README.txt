
Whassup is a Monitoring Frontend, which connects to livestatus.
Whassup does not replace the monitoring web frontend, but gives you very fast access to all core
functionalities and a quick view on everything the happens on your network.

License: GPL
Author Rene Storm 2011
Version: 0.8

Installation:
->INSTALL.txt

Whassup is written in QT Opensource, please take a look into their Licenses qt-project.org

Whassup has a help documentation, please take a look at this too for any advice needed.
Have fun,
Rene

Q/A:
Q: What is the motivation behind whassup?
A:  I was searching for a alerting alternative other than the browser window or email.
    I wanted to take a fast look into whats happening right now, without open browser, changing filters, sorting things.
    Having a small blinking systemtray icon on new incidents sounds cool.

Q: Do I really need a browser the monitoring any more?
A:  Yes, Whassup is not designed to do everything you will need to monitor your network.
    It's just an additional possibility to be quick informed.

Q: How often should I refresh the Service Display?
A:  This depends on your monitoring environment, the amount of services etc.
    A normal request with fcgi takes no longer that a part of a second.
    If you hit "Space", you will create an immidiate request, after this is answer, you can create a new one.
    Whassups depends on your livestatus. If you are accessing logs, your monitroing system will have to
    access the filesystem. This is by default slow.

Q: Why does service tab flicker on refresh?
A:  Whassup clears up all services and reinsert them on refresh. This is not a bug.
    It's a fast way to show, that you got new data. Just replacing changed items will not give you that feeling.

Q: Why can't I see service and host in OK state?
A:  You can, just open "?"->"about" and you will be able to select "OK" state in the view menu.
    This could create large request and who really cares about things that work ;o)?
    But if you combine it with, lets say a hostgroup filter, it could be a nice thing to do.
    Showing "OK" states can be "saved" too.



